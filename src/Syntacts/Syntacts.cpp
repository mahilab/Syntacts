#include <Syntacts/Syntacts.hpp>
#include <Syntacts/Detail/SPSCQueue.hpp>
#include "Helpers.hpp"
#include "portaudio.h"
#include <atomic>
#include <mutex>
#include <iostream>
#include <cassert>

#ifndef SAMPLE_RATE
    #define SAMPLE_RATE 44100
#endif

#ifndef BUFFER_SIZE
    #define BUFFER_SIZE 256
#endif

#ifndef PA_USE_ASIO
    #define PA_USE_ASIO 1
#endif

#ifndef QUE_SIZE
    #define QUE_SIZE 256
#endif

using namespace rigtorp;

namespace syntacts {

// private namespace
namespace {    

/// Instruction to que a cue
struct Instruction {
    int channel;
    std::shared_ptr<Cue> cue;
};

SPSCQueue<Instruction> g_que(QUE_SIZE);          
std::size_t g_numCh;                        ///< number of channels specified
std::vector<std::shared_ptr<Cue>> g_cues;   ///< cues
PaStream* g_stream;                         ///< portaudio stream
bool g_syntacts_initialized = false;        ///< syntacts initialized?
bool g_pa_initialized  = false;             ///< portadio initialized? 
DeviceInfo g_currentDevice = DeviceInfo({-1,"none",0});

/// Pops new instructions off the que into the cue vector
void processQue() {
    while (g_que.front()) {
        auto i = *g_que.front();
        g_cues[i.channel] = i.cue;
        g_que.pop();
    }
}

// portaudio callback method
int pa_callback(const void *inputBuffer, void *outputBuffer,
                       unsigned long framesPerBuffer,
                       const PaStreamCallbackTimeInfo *timeInfo,
                       PaStreamCallbackFlags statusFlags,
                       void *userData)
{
    // std::lock_guard<std::mutex> lock(g_mutex);
    processQue();
    /* Cast data passed through stream to our structure. */    
    float *out = (float *)outputBuffer;
    (void)inputBuffer; /* Prevent unused variable warning. */   
    // lock mutex using RA-II lockgaurd (unlocked when we exit scope b/c lock dies)
    for (unsigned long f = 0; f < framesPerBuffer; f++) {
        for (std::size_t c = 0; c < g_numCh; ++c) {
            out[g_numCh * f + c] = g_cues[c]->nextSample();
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

DeviceInfo makeDeviceInfo(int device) {
    auto pa_info = Pa_GetDeviceInfo(device);
    auto pa_type = Pa_GetHostApiInfo(pa_info->hostApi)->type;

    DeviceInfo info;
    info.index = device;
    info.name  = pa_info->name;
    info.maxChannels = pa_info->maxOutputChannels;
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

int initialize() {
    auto deviceInfo = getDefaultDevice();
    return initialize(deviceInfo.index, deviceInfo.maxChannels);
}

int initialize(int channelCount) {
    auto deviceInfo = getDefaultDevice();
    if (channelCount > deviceInfo.maxChannels)
        return SyntactsError_InvalidChannelCount;
    return initialize(deviceInfo.index, channelCount);
}


int initialize(int device, int channelCount) {
    if (g_syntacts_initialized)
        return SyntactsError_AlreadyIntialized;
    if (device < 0)
        return SyntactsError_InvalidDevice;
    // intitialize portaudio
    int result = initPortaudio();    
    if (result != paNoError)
        return result;
    g_currentDevice = makeDeviceInfo(device);

    g_numCh = channelCount;

    // init g_cues with empty cues
    g_cues.resize(channelCount);
    for (auto& cue : g_cues) {
        cue = std::make_shared<Cue>();      
    }   

    PaStreamParameters hostApiOutputParameters;
    PaStreamParameters* hostApiOutputParametersPtr;        
    if (channelCount > 0) {
        hostApiOutputParameters.device = device;
		if (hostApiOutputParameters.device == paNoDevice)
			return paDeviceUnavailable;
        hostApiOutputParameters.channelCount = channelCount;
        hostApiOutputParameters.sampleFormat = paFloat32;
        hostApiOutputParameters.suggestedLatency = Pa_GetDeviceInfo( hostApiOutputParameters.device )->defaultHighOutputLatency;
        hostApiOutputParameters.hostApiSpecificStreamInfo = NULL;
        hostApiOutputParametersPtr = &hostApiOutputParameters;
    }
    else {
        hostApiOutputParametersPtr = NULL;
    }   
    result = Pa_OpenStream(&g_stream, nullptr, hostApiOutputParametersPtr, SAMPLE_RATE, BUFFER_SIZE, paNoFlag, pa_callback, nullptr );
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
    if(!(channel < g_numCh))
        return SyntactsError_InvalidChannel;
    Instruction x;
    x.channel = channel;
    x.cue = cue;
    g_que.try_push(x);
    return SyntactsError_NoError;
} 

int playAll(std::shared_ptr<Cue> cue) {
    for (int i = 0; i < g_numCh; ++i) {
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

//==============================================================================
// ANSI C INTEFACE (DLL BINDINGS)
//==============================================================================

int initializeChannels(int channelCount) {
    return initialize(channelCount);
}

int initializeCustom(int device, int channelCount) {
    return initialize(device, channelCount);
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
    // failture conditions
    if(!g_syntacts_initialized)
        return SyntactsError_NotInitialized;
    if(!(channel < g_numCh))
        return SyntactsError_InvalidChannel;
    if (oscType == 0 && modType == 0)
        return SyntactsError_NoWaveform;    
    /// make envelope
    std::shared_ptr<Envelope> env;
    if (A == 0.0f && R == 0.0f)
        env = std::make_shared<Envelope>(S, amp);
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
    syntacts::play(channel, cue);
    return SyntactsError_NoError; 
}


} // namespace syntacts