#pragma once

#include <Syntacts/Config.hpp>
#include <Syntacts/Source.hpp>
#include <Syntacts/Oscillator.hpp>
#include <Syntacts/Envelope.hpp>
#include <Syntacts/Cue.hpp>
#include <Syntacts/AudioFile.hpp>
#include <string>

namespace tact {  

constexpr int   DEFAULT_QUEUE_SIZE    = 256;
constexpr int   DEFAULT_SAMPLE_RATE   = 44100;
constexpr float DEFAULT_SAMPLE_LENGTH = 1.0f / DEFAULT_SAMPLE_RATE;

/// Return errors
enum SyntactsError : int {
    SyntactsError_NoError             =  0,
    SyntactsError_InvalidDevice       = -1,
    SyntactsError_NotInitialized      = -2,
    SyntactsError_AlreadyIntialized   = -3,
    SyntactsError_InvalidChannel      = -4,
    SyntactsError_InvalidChannelCount = -5,
    SyntactsError_NoWaveform          = -6,
    SyntactsError_AudioFileBufferFail = -7,
    SyntactsError_AudioFileSaveFail   = -8,
    SyntactsError_ControlPanelFail    = -9
};

/// Struct holding ASIO device information
struct SYNTACTS_API DeviceInfo {
    DeviceInfo();
    DeviceInfo(int, std::string, std::string, int, bool, bool);
    int index;         ///< device index
    std::string name;  ///< device name
    std::string api;   ///< device API
    int maxChannels;   ///< maximum number of output channels
    bool default;      ///< is the device the default output device of all APIs?
    bool defaultApi;   ///< is the device the default output device for its API?
};    

#ifndef SYNTACTS_ANSI_C    

//==============================================================================
// C++11 INTERFACE
//==============================================================================

/// Initializes Syntacts Library
SYNTACTS_API int initialize(int deviceIndex = -1, int channelCount = -1, int sampleRate = DEFAULT_SAMPLE_RATE);

/// Finalizes Syntacts Library
SYNTACTS_API int finalize();

/// Get the default ASIO device
SYNTACTS_API DeviceInfo getDefaultDevice();

/// Get the current ASIO device
SYNTACTS_API DeviceInfo getCurrentDevice();

/// Gets list of available ASIO devices
SYNTACTS_API std::vector<DeviceInfo> getAvailableDevices();

/// Plays a Cue on a specified channel asynchronously
SYNTACTS_API int play(int channel, std::shared_ptr<Cue> cue);  

/// Plays a Cue on a specified channel asynchronously
SYNTACTS_API int play(int channel,   // channel              [0 to N]
                      int oscType,   // oscillator type      [0=none, 1=sin, 2=sqr, 3=saw, 4=tri]
                      float oscFreq, // oscillator frequency [Hz]
                      int modType,   // modulator type       [0=none, 1=sin, 2=sqr, 3=saw, 4=tri]
                      float modFreq, // modulator frequency  [Hz]
                      float amp,     // envelope amplitude   [0 to 1]
                      float A,       // attack time          [s]
                      float S,       // sustain time         [s]
                      float R);      // release time         [s]

/// Plays a Cue on all channels
SYNTACTS_API int playAll(std::shared_ptr<Cue> cue);

/// Stops running cue on a single channel
SYNTACTS_API int stop(int channel);

/// Stops all running cues
SYNTACTS_API int stopAll();

/// Saves a Cue to an audio file
SYNTACTS_API int exportToWave(std::shared_ptr<Cue> cue, std::string filePath, int sampleRate = DEFAULT_SAMPLE_RATE);

/// Opens the ASIO Control Panel
SYNTACTS_API int openControlPanel(int deviceIndex, void* windowHandle = nullptr);

#else

//==============================================================================
// ANSI C INTEFACE (FOR DLL BASED BINDINGS)
//==============================================================================

SYNTACTS_API DeviceInfo getDefaultDevice();
SYNTACTS_API DeviceInfo getCurrentDevice();
SYNTACTS_API std::vector<DeviceInfo> getAvailableDevices();

extern "C" {

SYNTACTS_API int initialize(int deviceIndex, int channelCount, int sampleRate);
SYNTACTS_API int initializeChannels(int channelCount);
SYNTACTS_API int initializeDefault();
SYNTACTS_API int finalize();

SYNTACTS_API int play(int channel,   // channel              [0 to N]
                 int oscType,   // oscillator type      [0=none, 1=sin, 2=sqr, 3=saw, 4=tri]
                 float oscFreq, // oscillator frequency [Hz]
                 float oscAmp,  // oscillator amplitude [0 to 1]
                 int modType,   // modulator type       [0=none, 1=sin, 2=sqr, 3=saw, 4=tri]
                 float modFreq, // modulator frequency  [Hz]
                 float modAmp,  // modulator amplitude  [0 to 1]
                 float A,       // attack time          [s]
                 float S,       // sustain time         [s]
                 float R);      // release time         [s]

SYNTACTS_API int stop(int channel);
SYNTACTS_API int stopAll();

}; // extern "C"

#endif // SYNTACTS_ANSI_C

} // namespace tact