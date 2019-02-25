#include <MEL/Communications/MelShare.hpp>  // for plotting
#include <MEL/Devices/Windows/Keyboard.hpp> // for keyboard
#include <MEL/Utility/System.hpp>           // for sleep()
#include <MEL/Math/Random.hpp>              // for random
#include <MEL/Core/Console.hpp>             // for print

#include <stdio.h>
#include <math.h>
#include "portaudio.h"
#include <iostream>
#include <atomic>
#include <MEL/Core.hpp>
#include <MEL/Devices/Windows/Keyboard.hpp>
#include <cmath>
#include <MEL/Utility/System.hpp>

#include <thread>
#include <mutex>
#include <functional>
#include <array>
#include <memory>
#include <atomic>

typedef mel::Keyboard KB;
using mel::Key;
using mel::sleep;
using mel::MelShare;
using mel::print;
using mel::random;

// define constants
#define PI     3.14159265358f
#define TWO_PI 6.28318530718f
#define NUM_CH 6

//=============================================================================
// Stream function to send to ASIO device (14 on Brandon's)
//=============================================================================

PaError Pa_OpenOurStream( PaStream** stream,
                              int inputChannelCount,
                              int outputChannelCount,
                              PaSampleFormat sampleFormat,
                              double sampleRate,
                              unsigned long framesPerBuffer,
                              PaStreamCallback *streamCallback,
                              void *userData )
{
    PaError result;
    PaStreamParameters hostApiInputParameters, hostApiOutputParameters;
    PaStreamParameters *hostApiInputParametersPtr, *hostApiOutputParametersPtr;

    if( inputChannelCount > 0 )
    {
        hostApiInputParameters.device = 14;

		if( hostApiInputParameters.device == paNoDevice )
			return paDeviceUnavailable;

        hostApiInputParameters.channelCount = inputChannelCount;
        hostApiInputParameters.sampleFormat = sampleFormat;
        /* defaultHighInputLatency is used below instead of
           defaultLowInputLatency because it is more important for the default
           stream to work reliably than it is for it to work with the lowest
           latency.
         */
        hostApiInputParameters.suggestedLatency =
             Pa_GetDeviceInfo( hostApiInputParameters.device )->defaultHighInputLatency;
        hostApiInputParameters.hostApiSpecificStreamInfo = NULL;
        hostApiInputParametersPtr = &hostApiInputParameters;
    }
    else
    {
        hostApiInputParametersPtr = NULL;
    }

    if( outputChannelCount > 0 )
    {
        hostApiOutputParameters.device = 14;
		if( hostApiOutputParameters.device == paNoDevice )
			return paDeviceUnavailable;

        hostApiOutputParameters.channelCount = outputChannelCount;
        hostApiOutputParameters.sampleFormat = sampleFormat;
        /* defaultHighOutputLatency is used below instead of
           defaultLowOutputLatency because it is more important for the default
           stream to work reliably than it is for it to work with the lowest
           latency.
         */
        hostApiOutputParameters.suggestedLatency =
             Pa_GetDeviceInfo( hostApiOutputParameters.device )->defaultHighOutputLatency;
        hostApiOutputParameters.hostApiSpecificStreamInfo = NULL;
        hostApiOutputParametersPtr = &hostApiOutputParameters;
    }
    else
    {
        hostApiOutputParametersPtr = NULL;
    }


    result = Pa_OpenStream(
                 stream, hostApiInputParametersPtr, hostApiOutputParametersPtr,
                 sampleRate, framesPerBuffer, paNoFlag, streamCallback, userData );
    return result;
}

//=============================================================================
// Base Classes
//=============================================================================

/// Cue Abstract Base Class
class Cue {
public:
    /// Override to get the next sample (return 0 by default)
    virtual float nextSample() { 
        return 0.0f; 
    } 

};

class Envelope {
public:
    /// Override to get the next sample (return 0 by default)
    virtual float nextSample() { 
        return 0.0f; 
    } 

};

class Oscillator {
public:
    /// Override to get the next sample (return 0 by default)
    virtual float nextSample() { 
        return 0.0f; 
    } 

};

/// Typedef All Ptr
typedef std::shared_ptr<Cue> CuePtr;
typedef std::shared_ptr<Oscillator> OscPtr;
typedef std::shared_ptr<Envelope> EnvPtr;

//=============================================================================
// Envelope Classes
//=============================================================================

// /// AS Envelope Impl.
class AS : public Envelope {
public:
    /// Constructor
    AS(float dur, float a_time) : 
        m_dur(dur), m_cur(0.0f), m_a_time(a_time) { }
    /// Gets the next sample
    float nextSample() override {
        float sample = m_cur < m_dur ? (1-std::exp(-3*m_cur / m_a_time))  : 0.0f;
        m_cur += 1.0f/44100.0f; /* steps forward one sample*/ 
        return sample;
    }
private:
    float m_dur, m_cur, m_a_time;
};

// SR Envelope Impl.
class SR : public Envelope {
public:
    /// Constructor
    SR(float dur, float a_time, float s_time) : 
        m_dur(dur), m_cur(0.0f), m_a_time(a_time) { }
    /// Gets the next sample
    float nextSample() override {
        float sample = m_cur < m_dur ? (1-1/std::exp(-m_cur-m_dur))  : 0.0f; /*math is wrong, fix on Monday*/
        m_cur += 1.0f/44100.0f; /* steps forward one sample*/ 
        return sample;
    }
private:
    float m_dur, m_cur, m_a_time, m_s_time;
};

//=============================================================================
// Oscillator Classes
//=============================================================================

/// Sine wave Cue Impl.
class SinWave : public Oscillator {
public:
    /// Constructor
    SinWave(float freq, float amp, float dur) : 
        m_freq(freq), m_amp(amp), m_dur(dur), m_cur(0.0f) { }
    /// Gets the next sample
    float nextSample() override {
        float sample = m_cur < m_dur ? m_amp * std::sin(TWO_PI * m_freq * m_cur) : 0.0f;
        m_cur += 1.0f/44100.0f; /* steps forward one sample*/ 
        return sample;
    }
private:
    float m_freq, m_amp, m_dur, m_cur, m_a_time;
};


/// Saw wave Cue Impl
class SawWave : public Oscillator {
public:
    /// Constructor
    SawWave(float freq, float amp, float dur) : 
        m_freq(freq), m_amp(amp), m_dur(dur), m_cur(0.0f) { }
    /// Gets the next sample
    float nextSample() override {
        float sample = m_cur < m_dur ? -2 / PI * m_amp * std::atan(std::cos(PI * m_freq * m_cur) / std::sin(PI * m_freq * m_cur)) : 0.0f;
        m_cur += 1.0f/44100.0f; /* steps forward one sample*/ 
        return sample;
    }
private:
    float m_freq, m_amp, m_dur, m_cur;
};


class CompoundOsc : public Oscillator {
public:
    CompoundOsc(OscPtr osc1, OscPtr osc2) : m_osc1(osc1), m_osc2(osc2) { }

    float nextSample() override {
        return m_osc1->nextSample() * m_osc2->nextSample();
    }

private:
    OscPtr m_osc1, m_osc2;
};

//=============================================================================
// Cue Class
//=============================================================================

class CompoundCue : public Cue {
public:
    CompoundCue(OscPtr osc, EnvPtr env) : m_osc(osc), m_env(env) { }

    float nextSample() {
        return m_osc->nextSample() * m_env->nextSample();
    }

private:
    OscPtr m_osc; EnvPtr m_env;
};

//=============================================================================
// AUDIO THEAD
//=============================================================================


/// Global Cue Array (Indexed by Channel)
std::array<CuePtr, NUM_CH> g_cues;

/// Global Stop Flag
std::atomic<bool> g_stop(false);

/// Mutex that gaurds g_cues
std::mutex g_mutex;

/// Simulated Audio Thread (for demonstration only)

int callback(const void *inputBuffer, void *outputBuffer,
                       unsigned long framesPerBuffer,
                       const PaStreamCallbackTimeInfo *timeInfo,
                       PaStreamCallbackFlags statusFlags,
                       void *userData)
{
    std::lock_guard<std::mutex> lock(g_mutex);
    /* Cast data passed through stream to our structure. */    
    float *out = (float *)outputBuffer;
    (void)inputBuffer; /* Prevent unused variable warning. */   
    // lock mutex using RA-II lockgaurd (unlocked when we exit scope b/c lock dies)
    for (unsigned long f = 0; f < framesPerBuffer; f++) {
        for (std::size_t c = 0; c < NUM_CH; ++c) {
            out[NUM_CH * f + c] = g_cues[c]->nextSample();
        }
    }
    return 0;
}

/*******************************************************************/



//=============================================================================
// TACTORFX USER INTEFACE?
//=============================================================================

// we wil want to namespace our interface
namespace tfx {

void play_cue(std::size_t chn, CuePtr cue) {
    // lock mutex using RA-II lockgaurd (unlocked when function ends b/c lock dies)
    std::lock_guard<std::mutex> lock(g_mutex);
    g_cues[chn] = cue;
    
    print("Played a Cue!");
}

} // namespace tfx

//=============================================================================
// MAIN THREAD
//=============================================================================

int main(int argc, char const *argv[])
{
    print("Main Thread Started");
    // init g_cues with empty cues
    for (auto& cue : g_cues)
        cue = std::make_shared<Cue>();     
    PaStream* stream0;
    /* Initialize library before making any other calls. */
    Pa_Initialize();
    std::cout << Pa_GetErrorText(Pa_OpenOurStream(&stream0, 0, 6, paFloat32, 44100, 256, callback, nullptr)) << std::endl;
    Pa_StartStream(stream0);
    // keys associated with each channel
    std::array<Key, NUM_CH> chn_keys = {Key::Num1, Key::Num2, Key::Num3, Key::Num4, Key::Num5, Key::Num6};    
    // start main loop
    while (!g_stop) {
        // exit if requested
        if (KB::is_key_pressed(Key::Escape))
            g_stop = true;

        for (std::size_t ch = 0; ch < NUM_CH; ++ch) {
            if (KB::is_key_pressed(chn_keys[ch])) {
                float freq = 175;  // for Evan's tactors
                float amp  = 1.0f; 
                float dur  = 0.5f;
                float a_time = 0.25f;
                float s_time = 0.75f;
                float mod_freq = 5;
                
                if (KB::is_key_pressed(Key::S)) {
                    OscPtr osc = std::make_shared<SinWave>(freq, amp, dur);
                    EnvPtr env = std::make_shared<AS>(dur, a_time);
                    CuePtr cue = std::make_shared<CompoundCue>(osc,env);
                    tfx::play_cue(ch, cue);
                }                
                else if (KB::is_key_pressed(Key::W)) {
                    OscPtr osc = std::make_shared<SawWave>(freq, amp, dur);
                    EnvPtr env = std::make_shared<AS>(dur, a_time);
                    CuePtr cue = std::make_shared<CompoundCue>(osc,env);
                    tfx::play_cue(ch, cue);
                }
                else if (KB::is_key_pressed(Key::C)) {
                    OscPtr osc1 = std::make_shared<SinWave>(freq, amp, dur);
                    OscPtr osc2 = std::make_shared<SinWave>(mod_freq, amp, dur);
                    OscPtr osc3 = std::make_shared<CompoundOsc>(osc1, osc2);
                    EnvPtr env = std::make_shared<AS>(dur, a_time);
                    CuePtr cue = std::make_shared<CompoundCue>(osc3,env);
                    tfx::play_cue(ch, cue);
                }
            }
        }
        // sleep so key presses aren't spammed (less than perfect)
        sleep(mel::milliseconds(100));
    }    
    // Stop and close stream 
    Pa_StopStream(stream0); 
    Pa_CloseStream(stream0); 
    Pa_Terminate();

    print("Main Thread Ended");
    return 0;
}
