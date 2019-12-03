#ifndef PA_USE_ASIO
    #define PA_USE_ASIO 1
#endif

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

namespace tact {

///////////////////////////////////////////////////////////////////////////////
// IMPLEMENTATION
///////////////////////////////////////////////////////////////////////////////

// NOTES:
// - DO NOT INSTANTIATE SIGNALS IN THE AUDIO THREAD (MEMORY POOLS ARE NOT THREAD SAFE)

namespace {

constexpr int    QUEUE_SIZE        = 256;
constexpr int    FRAMES_PER_BUFFER = 0;

static std::vector<double> STANDARD_SAMPLE_RATES = {
    8000.0, 9600.0, 11025.0, 12000.0, 16000.0, 22050.0, 24000.0, 32000.0,
    44100.0, 48000.0, 88200.0, 96000.0, 192000.0
};

using namespace rigtorp;

/// Channel structure
class Channel {
public:
    Signal signal;
    double time   = 0.0; 
    double sampleLength = 0.0;
    float  volume = 1.0f;
    float  lastVolume = 1.0f;
    float  pitch  = 1.0f;
    float  lastPitch = 1.0f;
    bool   paused = true;
    
    static float tBuff[2048];

    void fillBuffer(float* buffer, unsigned long frames) {
        // interp volume
        float nextVolume = volume;
        float volumeIncr = (nextVolume - lastVolume) / frames;
        volume = lastVolume;   
        // interp pitch
        float nextPitch = pitch;
        float pitchIncr = (nextPitch - lastPitch) / frames;
        pitch = lastPitch;

        if (paused) {
            for (unsigned long f = 0; f < frames; ++f) 
                buffer[f] = 0;
        }
        else {
            // fill time buffer and 
            for (unsigned long f = 0; f < frames; ++f) {
                pitch += pitchIncr;
                tBuff[f] = static_cast<float>(time);
                time += sampleLength * pitch;
            }
            // multisample signal
            signal.sample(tBuff, buffer, (int)frames);
            // apply volume
            for (unsigned long f = 0; f < frames; ++f) {
                volume += volumeIncr;
                buffer[f] *= volume;
            }
        }
        if (time > signal.length()) {
            paused = true;
        }

        volume     = nextVolume;
        lastVolume = nextVolume;
        pitch      = nextPitch;
        lastPitch  = nextPitch;
    }
};

float Channel::tBuff[2048];

/// Interface for commands sent through command queue
struct Command {
    int channel;
    virtual void perform(Channel& channel) = 0;
};

/// Commands to play a new cue
struct Play : public Command {
    virtual void perform(Channel& channel) override {
        channel.paused = false;
        channel.time   = 0;
        channel.signal = std::move(signal);
    }
    Signal signal;
};

/// Command to stop a channel
struct Stop : public Command {
    virtual void perform(Channel& channel) override {
        channel.paused = true;
        channel.time   = 0;
        channel.signal = std::move(Signal());
    }
};

/// Command to pause a channel
struct Pause : public Command {
    virtual void perform(Channel& channel) override {
        channel.paused = paused;
    }
    bool paused;
};

/// Command to set channel volume
struct Volume : public Command {
    virtual void perform(Channel& channel) override {
        channel.volume = volume;
    }
    float volume;
};

/// Command to set channel volume
struct Pitch : public Command {
    virtual void perform(Channel& channel) override {
        channel.pitch = pitch;
    }
    float pitch;
};

} // private namespace

/// Session Implementation
class Session::Impl {
public:

    Impl() :
        m_stream(nullptr),
        m_commands(QUEUE_SIZE),
        m_device({-1,"N/A",false,-1,"N/A",false,0})
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

        // set device
        m_device = device;

        // generat list of channel numbers
        channelNumbers.resize(channels);
        std::iota(channelNumbers.begin(), channelNumbers.end(), 1);

        PaStreamParameters params;
        params.device = device.index;
        params.channelCount = channels;
        params.suggestedLatency = Pa_GetDeviceInfo(params.device)->defaultLowOutputLatency;
        params.hostApiSpecificStreamInfo = nullptr;
        params.sampleFormat = paFloat32 | paNonInterleaved;

        m_sampleRate = sampleRate == 0 ? Pa_GetDeviceInfo(params.device)->defaultSampleRate : sampleRate;

        std::cout << m_sampleRate << std::endl;
        std::cout << params.device << std::endl;
        std::cout << params.channelCount << std::endl;
        std::cout << params.suggestedLatency << std::endl;


        params.channelCount = 8;
        if (Pa_IsFormatSupported(nullptr, &params, m_sampleRate) != paFormatIsSupported)
        {
            std::cout << "aint supported" << std::endl;
        }

        // resize vector of channels
        m_channels.clear();
        m_channels.resize(channels);
        for (auto& c : m_channels) 
            c.sampleLength = 1.0 / m_sampleRate;
        // open stream
        int result;
        result = Pa_OpenStream(&m_stream, nullptr, &params, m_sampleRate, FRAMES_PER_BUFFER, paNoFlag, callback, this);
        if (result != paNoError)
            return result;  
        result = Pa_StartStream(m_stream);
        if (result != paNoError)
            return result;
        return SyntactsError_NoError;
    }

    int close() {
        if (!isOpen())
            return SyntactsError_NotOpen;
        Pa_CloseStream(m_stream);
        m_device = {-1,"N/A",false,-1,"N/A",false,0};
        m_channels.clear();
        m_sampleRate = 0;
        return SyntactsError_NoError;
    }

    bool isOpen() const {
        return Pa_IsStreamActive(m_stream) == 1;
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
        auto command = std::make_shared<Pause>();
        command->channel = channel;   
        command->paused  = paused;
        bool success = m_commands.try_push(std::move(command));
        assert(success);
        return SyntactsError_NoError;       
    }

    int setVolume(int channel, float volume) {
        if (!isOpen())
            return SyntactsError_NotOpen;
        if (!(channel < m_channels.size()))
            return SyntactsError_InvalidChannel;
        auto command = std::make_shared<Volume>();
        command->channel = channel;
        command->volume  = clamp01(volume);
        bool success = m_commands.try_push(std::move(command));
        assert(success);
        return SyntactsError_NoError; 
    }

    int setPitch(int channel, float pitch) {
        if (!isOpen())
            return SyntactsError_NotOpen;
        if (!(channel < m_channels.size()))
            return SyntactsError_InvalidChannel;
        auto command = std::make_shared<Pitch>();
        command->channel = channel;
        command->pitch   = pitch;
        bool success = m_commands.try_push(std::move(command));
        assert(success);
        return SyntactsError_NoError;       
    }

    const Device& getCurrentDevice() const {
        return m_device;
    }

    const Device& getDefaultDevice() const {
        int def = Pa_GetDefaultOutputDevice();
        assert(def != paNoDevice);
        return m_devices.at(def);
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
        // Session::Impl* session = (Session::Impl*)userData;
        // auto& channels = session->m_channels;
        // session->performCommands();
        // (void)inputBuffer;     
        // float** out = (float**)outputBuffer;
        // for (std::size_t c = 0; c < channels.size(); ++c) {
        //     channels[c].fillBuffer(out[c], framesPerBuffer);
        // }
        return paContinue;
    }

    void openControlPanel(int index) {
        PaAsio_ShowControlPanel(index, nullptr);
    }

    Device makeDevice(int index) {
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

        return Device{index, 
                    pa_dev_info->name, 
                    index == Pa_GetDefaultOutputDevice(),
                    pa_api_info->type,
                    pa_api_info->name, 
                    index == Pa_GetHostApiInfo( pa_dev_info->hostApi )->defaultOutputDevice,
                    pa_dev_info->maxOutputChannels,
                    std::move(sampleRates),
                    static_cast<int>(pa_dev_info->defaultSampleRate)};
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
            if (d.second.apiIndex == paMME)
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

int Session::open(int index) {
    return open(getAvailableDevices().at(index));
}

int Session::open(int index, int channelCount, double sampleRate) {
    return open(getAvailableDevices().at(index),channelCount, sampleRate);
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

int Session::setVolume(int channel, float volume) {
    return m_impl->setVolume(channel, volume);
}


int Session::setPitch(int channel, float pitch) {
    return m_impl->setPitch(channel, pitch);
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