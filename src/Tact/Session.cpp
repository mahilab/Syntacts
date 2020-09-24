#include "misc/SPSCQueue.h"
#include <Tact/Session.hpp>
#include <cassert>
#include "portaudio.h"
#include "pa_asio.h"
#include "pa_win_wasapi.h"
#include "pa_win_ds.h"
#include <algorithm>
#include <functional>
#include <thread>
#include <iostream>
#include <fstream>
#include <set>
#include <numeric>
#include <array>

namespace tact {

///////////////////////////////////////////////////////////////////////////////
// IMPLEMENTATION
///////////////////////////////////////////////////////////////////////////////

// NOTES:
// - DO NOT INSTANTIATE SIGNALS IN THE AUDIO THREAD (MEMORY POOLS ARE NOT THREAD SAFE)

namespace {

constexpr int    QUEUE_SIZE        = 1024;
constexpr int    FRAMES_PER_BUFFER = 0;

static std::array<double,13> STANDARD_SAMPLE_RATES = {
    8000, 9600, 11025, 12000, 16000, 22050, 24000, 32000,
    44100, 48000, 88200, 96000, 192000
};

using namespace rigtorp;

struct Voice {
    Signal signal;
    double time  = 0;
    bool stopped = true;
    inline double step(double dt) {
        double s = signal.sample(time) * (!stopped);
        time += dt;
        return s;
    }    
};

/// Channel structure
class Channel {
public:
    std::array<Voice,SYNTACTS_MAX_VOICES> voices;
    Signal  signal;
    double  sampleLength = 0.0;
    double  volume       = 1.0;
    double  pitch        = 1.0;
    double  level        = 0.0;
    bool    paused       = false;
    bool    stopped      = true;
   
    void fillBuffer(float* buffer, unsigned long frames) {
        // interp volume
        double nextVolume = volume;
        double volumeIncr = (nextVolume - lastVolume) / frames;
        volume = lastVolume;   
        // interp pitch
        double nextPitch = pitch;
        double pitchIncr = (nextPitch - lastPitch) / frames;
        pitch = lastPitch;

        if (paused || stopped) {
            for (unsigned long f = 0; f < frames; ++f) {
                buffer[f] = 0;
                level     = 0;
            }
        }
        else {
            // fill buffer
            double max_level = 0;
            for (unsigned long f = 0; f < frames; ++f) {
                pitch += pitchIncr;
                volume += volumeIncr;
                double dt = sampleLength * pitch;
                double output = stepVoices(dt) * volume;
                double abs_out = std::abs(output);
                max_level = abs_out > max_level ? abs_out : max_level;
                buffer[f] = static_cast<float>(output);
            }
            level = max_level; // sum_output / frames;
        }
        stopped = activeVoices() == 0;
        volume     = nextVolume;
        lastVolume = nextVolume;
        pitch      = nextPitch;
        lastPitch  = nextPitch;
    }

    inline void play(Signal sig) {
        stopped = false;
        paused = false;
        for (auto& v : voices) {
            if (v.stopped) {
                v.signal = std::move(sig);
                v.stopped = false;
                v.time = 0;
                return;
            }
        }
        voices[0].signal  = std::move(sig);
        voices[0].stopped = false;
        voices[0].time    = 0;
    }

    inline void stop() {
        for (auto& v : voices) {
            v.stopped = true;
            v.time   = 0;
        }
        paused = true;
    }

    inline double stepVoices(double dt) {
        double sum = 0;
        for (auto& v : voices)
            sum += v.step(dt);
        return sum;
    }

    inline int activeVoices() {
        int count = 0;
        for (auto& v : voices) {
            if (v.time > v.signal.length()) {
                v.stopped = true;
                v.time    = 0;
            }
        }
        for (auto& v : voices) {
            if (!v.stopped)
                count++;
        }        
        return count;
    }
private:
    double  lastVolume   = 1.0;
    double  lastPitch    = 1.0;
};

/// Interface for commands sent through command queue
struct Command {
    int channel;
    std::atomic_flag flag = ATOMIC_FLAG_INIT;

    Command() { flag.test_and_set(std::memory_order_acquire); }

    void perform(Channel& channel) {
        performImpl(channel);
        flag.clear(std::memory_order_release);
    }

    void wait() {
        while (flag.test_and_set(std::memory_order_acquire)) { 
        }
    }

    virtual void performImpl(Channel& channel) = 0;
};

struct Play : public Command {
    virtual void performImpl(Channel& channel) override {
        channel.play(std::move(signal));
    }
    Signal signal;
};

struct Stop : public Command {
    virtual void performImpl(Channel& channel) override {
        channel.stop();
    }
};

struct SetPause : public Command {
    virtual void performImpl(Channel& channel) override {
        channel.paused = paused;
    }
    bool paused;
};

struct SetVolume : public Command {
    virtual void performImpl(Channel& channel) override {
        channel.volume = volume;
    }
    double volume;
};

struct GetVolume : public Command {
    virtual void performImpl(Channel& channel) override {
        volume = channel.volume;
    }
    double volume;
};

struct SetPitch : public Command {
    virtual void performImpl(Channel& channel) override {
        channel.pitch = pitch;
    }
    double pitch;
};

struct GetPitch : public Command {
    virtual void performImpl(Channel& channel) override {
        pitch = channel.pitch;
    }
    double pitch;
};

struct GetLevel : public Command {
    virtual void performImpl(Channel& channel) override {
        level = channel.level;
    }
    double level;
};

} // private namespace

Device::Device() :
    index(-1),
    name("N/A"),
    isDefault(false),
    api(API::Unknown),
    apiName("N/A"),
    isApiDefault(false),
    maxChannels(0),
    sampleRates({}),
    defaultSampleRate(0)
{ }

/// Session Implementation
class Session::Impl {
public:

    Impl() :
        m_stream(nullptr),
        m_commands(QUEUE_SIZE),
        m_device()
    {

        // initialize PortAudio
        int result = Pa_Initialize();
        assert(result == paNoError);
        // build list of available devices
        for (int i = 0; i < Pa_GetDeviceCount(); ++i) {
            if (Pa_GetDeviceInfo(i)->maxOutputChannels > 0) {   
                m_devices.emplace(i, makeDevice(i));
            }   
        }
        // clean up devices
        correctMMENames();
        removeDigitalDevices();
        tidyNames();
        s_count++;
    }

    ~Impl() {
        if (isOpen())
            close();
        int result = Pa_Terminate();
        assert(result == paNoError);
        s_count--;
    }

    std::vector<int> channelNumbers;

    int open(const Device& device, int channels, double sampleRate) {

        // return if already open
        if (isOpen())
            return SyntactsError_AlreadyOpen;

        if (device.index == -1 || device.api == API::Unknown)
            return SyntactsError_InvalidDevice;

        // generat list of channel numbers
        channelNumbers.resize(channels);
        std::iota(channelNumbers.begin(), channelNumbers.end(), 1);

        if (channels == 0)
            channels = device.maxChannels;

        PaStreamParameters params;
        params.device = device.index;
        params.channelCount = channels;
        params.suggestedLatency = Pa_GetDeviceInfo(params.device)->defaultLowOutputLatency;
        params.hostApiSpecificStreamInfo = nullptr;
        params.sampleFormat = paFloat32 | paNonInterleaved;

        sampleRate = sampleRate == 0 ? device.defaultSampleRate : sampleRate;

        if (Pa_IsFormatSupported(nullptr, &params, sampleRate) != paFormatIsSupported)
            return SyntactsError_InvalidSampleRate;

        // resize vector of channels
        m_channels.clear();
        m_channels.resize(channels);
        for (auto& c : m_channels) 
            c.sampleLength = 1.0 / sampleRate;
        // open stream
        int result;
        result = Pa_OpenStream(&m_stream, nullptr, &params, sampleRate, FRAMES_PER_BUFFER, paNoFlag, callback, this);
        if (result != paNoError)
            return result;  
        result = Pa_StartStream(m_stream);
        if (result != paNoError)
            return result;
        // set device/sampel rate
        m_device = device;
        m_sampleRate = sampleRate;
        return SyntactsError_NoError;
    }

    int close() {
        if (!isOpen())
            return SyntactsError_NotOpen;
        int result = Pa_CloseStream(m_stream);
        if (result != paNoError) {
            return result;
        }
        m_device = Device();
        m_channels.clear();
        m_sampleRate = 0;
        m_stream = nullptr;
        return SyntactsError_NoError;
    }

    bool isOpen() const {
        return m_stream != nullptr && Pa_IsStreamActive(m_stream) == 1;
    }

    bool isPlaying(int channel) {
        return !m_channels[channel].paused && !m_channels[channel].stopped; 
    }

    bool isPaused(int channel) {
        return m_channels[channel].paused; 
    }

    int play(int channel, Signal signal) {
        if (!isOpen())
            return SyntactsError_NotOpen;
        if (!(channel < m_channels.size()))
            return SyntactsError_InvalidChannel;
        auto command = std::make_shared<Play>();
        command->signal = std::move(signal);
        command->channel = channel;
        bool success = m_commands.try_push(std::move(command));
        assert(success);
        return SyntactsError_NoError;
    }

    int stop(int channel) {
        if (!isOpen())
            return SyntactsError_NotOpen;
        if (!(channel < m_channels.size()))
            return SyntactsError_InvalidChannel;
        auto command = std::make_shared<Stop>();
        command->channel = channel;   
        bool success = m_commands.try_push(std::move(command));
        assert(success);
        return SyntactsError_NoError;     
    }

    int pause(int channel, bool paused) {
        if (!isOpen())
            return SyntactsError_NotOpen;
        if (!(channel < m_channels.size()))
            return SyntactsError_InvalidChannel;
        auto command = std::make_shared<SetPause>();
        command->channel = channel;   
        command->paused  = paused;
        bool success = m_commands.try_push(std::move(command));
        assert(success);
        return SyntactsError_NoError;       
    }

    int setVolume(int channel, double volume) {
        
        auto command = std::make_shared<SetVolume>();
        command->channel = channel;
        command->volume  = clamp01(volume);
        bool success = m_commands.try_push(std::move(command));
        assert(success);
        return SyntactsError_NoError; 
    }

    double getVolume(int channel) {
        if (!isOpen())
            return 0;
        if (!(channel < m_channels.size()))
            return 0;
        if constexpr (std::atomic<double>::is_always_lock_free) 
            return m_channels[channel].volume; // this *should* be thread safe, TBD
        else {
            auto command = std::make_shared<GetVolume>();
            bool success = m_commands.try_push(std::move(command));
            assert(success);
            return command->volume;
        } 
    }

    int setPitch(int channel, double pitch) {
        if (!isOpen())
            return SyntactsError_NotOpen;
        if (!(channel < m_channels.size()))
            return SyntactsError_InvalidChannel;
        auto command = std::make_shared<SetPitch>();
        command->channel = channel;
        command->pitch   = pitch;
        bool success = m_commands.try_push(std::move(command));
        assert(success);
        return SyntactsError_NoError;       
    }

    double getPitch(int channel) {
        if (!isOpen())
            return 1;
        if (!(channel < m_channels.size()))
            return 1;
        if constexpr (std::atomic<double>::is_always_lock_free) 
            return m_channels[channel].pitch;
        else {
            auto command = std::make_shared<GetPitch>();
            bool success = m_commands.try_push(std::move(command));
            assert(success);
            return command->pitch;
        } 
    }

    double getLevel(int channel) {
        if (!isOpen())
            return 0;
        if (!(channel < m_channels.size()))
            return 0;
        if constexpr (std::atomic<double>::is_always_lock_free) 
            return m_channels[channel].level;
        else {        
            auto command = std::make_shared<GetLevel>();
            bool success = m_commands.try_push(std::move(command));
            assert(success);
            return command->level;
        }
    }

    const Device& getCurrentDevice() const {
        return m_device;
    }

    const Device& getDefaultDevice() const {
        int def = Pa_GetDefaultOutputDevice();
        assert(def != paNoDevice);
        if (m_devices.count(def))
            return m_devices.at(def);
        else
            return m_devices.begin()->second;
    }

    const std::map<int, Device>& getAvailableDevices() const {
        return m_devices;
    }

    int getChannelCount() const {
        if (isOpen())
            return (int)m_channels.size();
        else
            return 0;
    }

    double getSampleRate() const {
        return m_sampleRate;
    }

    double getCpuLoad() const {
        if (isOpen())
            return Pa_GetStreamCpuLoad(m_stream);
        return 0;
    }

    static int count() {
        return s_count;
    }

    void performCommands() {
        while (m_commands.front()) {
            auto command = *m_commands.front();
            command->perform(m_channels[command->channel]);
            m_commands.pop();
        }
    }

    static int callback(const void *inputBuffer, void *outputBuffer,
                 unsigned long framesPerBuffer,
                 const PaStreamCallbackTimeInfo *timeInfo,
                 PaStreamCallbackFlags statusFlags,
                 void *userData)
    {
        Session::Impl* session = (Session::Impl*)userData;
        auto& channels = session->m_channels;
        session->performCommands();
        (void)inputBuffer;     
        float** out = (float**)outputBuffer;
        for (std::size_t c = 0; c < channels.size(); ++c) {
            channels[c].fillBuffer(out[c], framesPerBuffer);
        }
        return paContinue;
    }

    void openControlPanel(int index) {
#if PA_USE_ASIO
        PaAsio_ShowControlPanel(index, nullptr);
#endif
    }

    Device makeDevice(int index) const {
        auto pa_dev_info = Pa_GetDeviceInfo(index);
        auto pa_api_info = Pa_GetHostApiInfo(pa_dev_info->hostApi);

        std::vector<int> sampleRates;
        sampleRates.reserve(STANDARD_SAMPLE_RATES.size());

        PaStreamParameters params;
        params.device = index;
        params.channelCount = pa_dev_info->maxOutputChannels;
        params.suggestedLatency = Pa_GetDeviceInfo(params.device)->defaultLowOutputLatency;
        params.hostApiSpecificStreamInfo = nullptr;
        params.sampleFormat = paFloat32 | paNonInterleaved;


        for (auto& s : STANDARD_SAMPLE_RATES) {
            if (Pa_IsFormatSupported(nullptr, &params, s) == paFormatIsSupported)
                sampleRates.push_back(static_cast<int>(s));
        }

        Device dev;
        dev.index = index;
        dev.name = pa_dev_info->name;
        dev.isDefault = index == Pa_GetDefaultOutputDevice();
        dev.api      =   static_cast<API>(pa_api_info->type);
        dev.apiName = pa_api_info->name;
        dev.isApiDefault = index == Pa_GetHostApiInfo( pa_dev_info->hostApi )->defaultOutputDevice;
        dev.maxChannels = pa_dev_info->maxOutputChannels;
        dev.sampleRates = std::move(sampleRates);
        dev.defaultSampleRate = static_cast<int>(pa_dev_info->defaultSampleRate);
        return dev;
    }

    void tidyNames() {
        static std::vector<std::string> apiRemoves = {"Windows "};
        for (auto& d : m_devices) {
            for (auto& r : apiRemoves) {
                auto found = d.second.apiName.find(r);
                if (found != std::string::npos)
                    d.second.apiName.erase(found, r.length());
            }
        }
    }

    void correctMMENames() {
        // correct MME names
        std::vector<std::string*> mme;
        std::set<std::string> notMME;
        for (auto& d : m_devices) {
            if (d.second.api == API::MME)
                mme.push_back(&d.second.name);
            else
                notMME.insert(d.second.name);
        }
        for (auto& cur : mme) {
            for (auto& alt : notMME) {
                if (alt.find(*cur) == 0) {
                    *cur = alt;
                }
            }
        } 
    }

    void removeDigitalDevices() {
        static std::vector<std::string> digitalStrings = {"SPDIF","S/PDIF","Optic","optic","digital","Digital"};
        for (auto dev = m_devices.begin(); dev != m_devices.end();) {
            bool remove = false;
            for (auto& digi : digitalStrings) {
                if (dev->second.name.find(digi) != std::string::npos) {
                    remove = true;
                    break;
                }
            }
            if (remove)
                m_devices.erase(dev++);
            else
                ++dev;
        }
    }

    Device m_device;
    std::map<int, Device> m_devices;

    std::vector<Channel> m_channels;

    SPSCQueue<std::shared_ptr<Command>> m_commands;
    PaStream* m_stream;

    double m_sampleRate = 0;

    static int s_count;
};

int Session::Impl::s_count = 0;

///////////////////////////////////////////////////////////////////////////////
// PUBLIC INTERFACE
///////////////////////////////////////////////////////////////////////////////

Session::Session() : 
    m_impl((std::make_unique<Session::Impl>())) 
{ }

Session::~Session() {
    
}

int Session::open() {
    return open(getDefaultDevice());
}

int Session::open(const Device& device) {
    return m_impl->open(device, device.maxChannels, 0);
}

int Session::open(const Device& device, int channelCount, double sampleRate) {
    return m_impl->open(device, std::min(channelCount, device.maxChannels), sampleRate);
}

int Session::open(API api) {
    if (api == API::Unknown)
        return SyntactsError_InvalidAPI;
    for (auto& dev : getAvailableDevices()) {
        if (dev.second.api == api && dev.second.isApiDefault)
            return open(dev.second);
    }
    return SyntactsError_InvalidDevice;
}

int Session::open(int index) {
    if (getAvailableDevices().count(index) > 0)
        return open(getAvailableDevices().at(index));
    else
        return SyntactsError_InvalidDevice;
}

int Session::open(int index, int channelCount, double sampleRate) {
    if (getAvailableDevices().count(index) > 0)
        return open(getAvailableDevices().at(index),channelCount, sampleRate);
    else
        return SyntactsError_InvalidDevice;
}

int Session::open(const std::string& name, API api) {
    if (api == API::Unknown)
        return SyntactsError_InvalidAPI;
    for (auto& dev : getAvailableDevices()) {
        if (dev.second.name == name && dev.second.api == api)
            return open(dev.second);
    }
    return SyntactsError_InvalidDevice;
}

int Session::close() {
    return m_impl->close();
}

bool Session::isOpen() const {
    return m_impl->isOpen();
}

int Session::play(int channel, Signal signal) {
    return m_impl->play(channel, std::move(signal));
}

bool Session::isPlaying(int channel) {
    return m_impl->isPlaying(channel);
}

bool Session::isPaused(int channel) {
    return m_impl->isPaused(channel);
}

int Session::playAll(Signal signal) {
    for (int i = 0; i < getChannelCount(); ++i) {
        if (int ret = play(i, signal) != SyntactsError_NoError)
            return ret;
    }
    return SyntactsError_NoError;
}

int Session::stop(int channel) {
    return m_impl->stop(channel);
}

int Session::stopAll() {
    for (int i = 0; i < getChannelCount(); ++i) {
        if (int ret = stop(i) != SyntactsError_NoError)
            return ret;
    }
    return SyntactsError_NoError;
}

int Session::pause(int channel) {
    return m_impl->pause(channel, true);
}

int Session::pauseAll() {
    for (int i = 0; i < getChannelCount(); ++i) {
        if (int ret = pause(i) != SyntactsError_NoError)
            return ret;
    }
    return SyntactsError_NoError;
}

int Session::resume(int channel) {
    return m_impl->pause(channel, false);
}

int Session::resumeAll() {
    for (int i = 0; i < getChannelCount(); ++i) {
        if (int ret = resume(i) != SyntactsError_NoError)
            return ret;
    }
    return SyntactsError_NoError;
}

int Session::setVolume(int channel, double volume) {
    return m_impl->setVolume(channel, volume);
}

double Session::getVolume(int channel) {
    return m_impl->getVolume(channel);
}


int Session::setPitch(int channel, double pitch) {
    return m_impl->setPitch(channel, pitch);
}

double Session::getPitch(int channel) {
    return m_impl->getPitch(channel);
}

double Session::getLevel(int channel) {
    return m_impl->getLevel(channel);
}

const Device& Session::getCurrentDevice() const {
    return m_impl->getCurrentDevice();
}

const Device& Session::getDefaultDevice() const {
    return m_impl->getDefaultDevice();
}

const std::map<int, Device>& Session::getAvailableDevices() const {
    return m_impl->getAvailableDevices();
}

int Session::getChannelCount() const {
    return m_impl->getChannelCount();
}

double Session::getSampleRate() const {
    return m_impl->getSampleRate();
}

double Session::getCpuLoad() const {
    return m_impl->getCpuLoad();
}

int Session::count() {
    return Impl::count();
}

void Session::openControlPanel(int index) {
    m_impl->openControlPanel(index);
}

}; // namespace tact