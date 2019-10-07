#ifndef PA_USE_ASIO
    #define PA_USE_ASIO 1
#endif

#include <Syntacts/Syntacts.hpp>
#include <Syntacts/Detail/SPSCQueue.hpp>
#include "Helpers.hpp"
#include "portaudio.h"
#include <atomic>
#include <mutex>
#include <iostream>
#include <cassert>
#include <utility>

using namespace rigtorp;

namespace tact {

// private namespace
namespace {    

/// Instruction to que a cue
struct Instruction {
    int channel;
    std::shared_ptr<Cue> cue;
};

/// A Cue and its current time
struct CueAndTime {
    std::shared_ptr<Cue> cue;
    float time = 0.0f;
};

// Globals
SPSCQueue<Instruction> g_queue(DEFAULT_QUEUE_SIZE);          
int g_channelCount;                         ///< number of channels specified
std::vector<CueAndTime> g_cues;             ///< cues and times
PaStream* g_stream;                         ///< portaudio stream
bool g_syntacts_initialized = false;        ///< syntacts initialized?
bool g_pa_initialized  = false;             ///< portadio initialized? 
DeviceInfo g_currentDevice = DeviceInfo({-1,"none","none",0});
float g_sampleLength = DEFAULT_SAMPLE_LENGTH;

/// Pops new instructions off the queue into the cue vector
void processQueue() {
    while (g_queue.front()) {
        auto i = *g_queue.front();
        g_cues[i.channel].cue = i.cue;
        g_cues[i.channel].time = 0.0f;
        g_queue.pop();
    }
}

// portaudio callback method
int pa_callback(const void *inputBuffer, void *outputBuffer,
                       unsigned long framesPerBuffer,
                       const PaStreamCallbackTimeInfo *timeInfo,
                       PaStreamCallbackFlags statusFlags,
                       void *userData)
{
    processQueue();
    /* Cast data passed through stream to our structure. */    
    float *out = (float *)outputBuffer;
    (void)inputBuffer; /* Prevent unused variable warning. */   
    for (unsigned long f = 0; f < framesPerBuffer; f++) {
        for (std::size_t c = 0; c < g_channelCount; ++c) {
            // out[g_channelCount * f + c] = g_cues[c].first->nextSample();
            out[g_channelCount * f + c] = g_cues[c].cue->sample(g_cues[c].time);
            g_cues[c].time += g_sampleLength;
        }
    }
    return 0;
}  

int initPortaudio() {
    if (!g_pa_initialized) {
        int result = Pa_Initialize();
        if (result != paNoError)
            return result;
        g_pa_initialized = true;
    }
    return paNoError;
}

DeviceInfo makeDeviceInfo(int deviceIndex) {
    auto pa_dev_info = Pa_GetDeviceInfo(deviceIndex);
    auto pa_api_info = Pa_GetHostApiInfo(pa_dev_info->hostApi);
    DeviceInfo info;
    info.index       = deviceIndex;
    info.name        = pa_dev_info->name;
    info.api         = pa_api_info->name;
    info.maxChannels = pa_dev_info->maxOutputChannels;
    return info;
}

} // private namespace

//==============================================================================
// C++11 INTERFACE
//==============================================================================

std::vector<DeviceInfo> getAvailableDevices() {
    initPortaudio();
    std::vector<DeviceInfo> infos;
    for (int i = 0; i < Pa_GetDeviceCount(); ++i) {
        if (Pa_GetHostApiInfo(Pa_GetDeviceInfo(i)->hostApi)->type == paASIO){            
            infos.push_back(makeDeviceInfo(i));
        }   
    }
    return infos;
}

DeviceInfo getDefaultDevice() {
    auto infos = getAvailableDevices();
    if (infos.empty()) {
        DeviceInfo info;
        info.index = -1;
        info.name  = "none";
        info.api   = "none";
        info.maxChannels = 0;
        return info;
    }
    else {
        return infos[0];
    }
}

DeviceInfo getCurrentDevice() {
    return g_currentDevice;
}

int initialize(int device, int channelCount, int sampleRate) {
    // return if syntacts already initialized
    if (g_syntacts_initialized)
        return SyntactsError_AlreadyIntialized;
    // intitialize portaudio
    int result = initPortaudio();    
    if (result != paNoError)
        return result;
    // set current device
    if (device < 0) 
        g_currentDevice = getDefaultDevice();
    else 
        g_currentDevice = makeDeviceInfo(device);
    // set channel count
    if (channelCount < 0) 
        g_channelCount = g_currentDevice.maxChannels;
    else if (channelCount <= g_currentDevice.maxChannels) 
        g_channelCount = channelCount;
    else 
        return SyntactsError_InvalidChannelCount;
    // set sample length
    g_sampleLength = 1.0f / sampleRate;
    // init g_cues with empty cues
    g_cues.resize(g_channelCount);
    for (auto& cue : g_cues) {
        cue.cue = std::make_shared<Cue>();      
    }   
    // open portaudio stream
    PaStreamParameters hostApiOutputParameters;
    PaStreamParameters* hostApiOutputParametersPtr;        
    if (g_channelCount > 0) {
        hostApiOutputParameters.device = g_currentDevice.index;
		if (hostApiOutputParameters.device == paNoDevice)
			return paDeviceUnavailable;
        hostApiOutputParameters.channelCount = g_channelCount;
        hostApiOutputParameters.sampleFormat = paFloat32;
        hostApiOutputParameters.suggestedLatency = Pa_GetDeviceInfo( hostApiOutputParameters.device )->defaultLowOutputLatency;
        hostApiOutputParameters.hostApiSpecificStreamInfo = NULL;
        hostApiOutputParametersPtr = &hostApiOutputParameters;
    }
    else {
        hostApiOutputParametersPtr = NULL;
    }   
    result = Pa_OpenStream(&g_stream, nullptr, hostApiOutputParametersPtr, sampleRate, 0, paNoFlag, pa_callback, nullptr );
    if (result != paNoError)
        return result;    
    result = Pa_StartStream(g_stream);
    if (result != paNoError)
        return result;
    g_syntacts_initialized = true;
    return 0;
}

int finalize() {
    if (!g_syntacts_initialized)
        return SyntactsError_NotInitialized;
    Pa_StopStream(g_stream); 
    Pa_CloseStream(g_stream); 
    Pa_Terminate();
    g_pa_initialized = false;
    g_syntacts_initialized = false;
    return SyntactsError_NoError;
}

int play(int channel, std::shared_ptr<Cue> cue) {
    // failture conditions
    if(!g_syntacts_initialized)
        return SyntactsError_NotInitialized;
    if(!(channel < g_channelCount))
        return SyntactsError_InvalidChannel;
    Instruction x;
    x.channel = channel;
    x.cue = cue;
    g_queue.try_push(x);
    return SyntactsError_NoError;
} 

int playAll(std::shared_ptr<Cue> cue) {
    for (int i = 0; i < g_channelCount; ++i) {
        auto ret = play(i, cue);
        if (ret != SyntactsError_NoError)
            return ret;
    }
    return SyntactsError_NoError;
}

int stop(int channel) {
    return play(channel, std::make_shared<Cue>());
}

int stopAll() {
    return playAll(std::make_shared<Cue>());
}

int save(std::shared_ptr<Cue> cue, std::string filePath, AudioFileFormat format, int sampleRate) {
    AudioFile<float> file;
    AudioFile<float>::AudioBuffer buffer;
    buffer.resize(1);
    buffer[0].resize(cue->sampleCount(sampleRate));
    float sampleLength = 1.0f / sampleRate;
    float t = 0;
    for (auto& sample : buffer[0]) {
        sample = cue->sample(t);  
        t += sampleLength;
    }
    if (!file.setAudioBuffer(buffer))
        return SyntactsError_AudioFileBufferFail;
    file.setBitDepth(24);
    file.setSampleRate(sampleRate);
    if (!file.save(filePath, format))
        return SyntactsError_AudioFileSaveFail;
    return SyntactsError_NoError;
}

//==============================================================================
// ANSI C INTEFACE (DLL BINDINGS)
//==============================================================================

int initializeChannels(int channelCount) {
    return initialize(-1, channelCount, DEFAULT_SAMPLE_RATE);
}

int initializeDefault() {
    return initialize(-1,-1, DEFAULT_SAMPLE_RATE);
}

SYNTACTS_API int play(int channel,    // channel              [0 to N]
                 int oscType,   // oscillator type      [0=none, 1=sin, 2=sqr, 3=saw, 4=tri]
                 float oscFreq, // oscillator frequency [Hz]
                 int modType,   // modulator type       [0=none, 1=sin, 2=sqr, 3=saw, 4=tri]
                 float modFreq, // modulator frequency  [Hz]
                 float amp,     // envelope amplitude   [0 to 1]
                 float A,       // attack time          [s]
                 float S,       // sustain time         [s]
                 float R)       // release time         [s]
{
    // failure conditions
    if(!g_syntacts_initialized)
        return SyntactsError_NotInitialized;
    if(!(channel < g_channelCount))
        return SyntactsError_InvalidChannel;
    if (oscType == 0 && modType == 0)
        return SyntactsError_NoWaveform;    
    /// make envelope
    std::shared_ptr<Envelope> env;
    if (A == 0.0f && R == 0.0f)
        env = std::make_shared<BasicEnvelope>(S, amp);
    else
        env = std::make_shared<ASR>(A,S,R, amp);
    // make oscillator
    std::shared_ptr<Oscillator> osc;
    if (oscType == 1)
        osc = std::make_shared<SineWave>(oscFreq); 
    else if (oscType == 2)
        osc = std::make_shared<SquareWave>(oscFreq);
    else if (oscType == 3)
        osc = std::make_shared<SawWave>(oscFreq);
    else if (oscType = 4)
        osc = std::make_shared<TriWave>(oscFreq);
    // make modulator
    std::shared_ptr<Oscillator> mod;
    if (modType == 1)
        mod = std::make_shared<SineWave>(modFreq); 
    else if (modType == 2)
        mod = std::make_shared<SquareWave>(modFreq);
    else if (modType == 3)
        mod = std::make_shared<SawWave>(modFreq);
    else if (modType == 4)
        mod = std::make_shared<TriWave>(modFreq);
    // generate cue
    std::shared_ptr<Cue> cue;
    if ((oscType > 0) && (modType > 0)) 
        cue = std::make_shared<Cue>(osc, mod, env);
    else if (oscType > 0) 
        cue = std::make_shared<Cue>(osc, env);
    else if (modType > 0)
        cue = std::make_shared<Cue>(mod, env);
    else
        return SyntactsError_NoWaveform;
    /// play the cue
    tact::play(channel, cue);
    return SyntactsError_NoError; 
}


} // namespace tact