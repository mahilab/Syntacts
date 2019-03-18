#pragma once

#include <TactorFX/Config.hpp>
#include <TactorFX/Generator.hpp>
#include <TactorFX/Oscillator.hpp>
#include <TactorFX/Envelope.hpp>
#include <TactorFX/Cue.hpp>
#include <string>

namespace tfx {  

/// Struct holding ASIO device information
struct TFX_API DeviceInfo {
    int index;         ///< device index
    std::string name;  ///< device name
    int maxChannels;   ///< maximum number of output channels
};    

#ifndef TFX_ANSI_C    

//==============================================================================
// C++11 INTERFACE
//==============================================================================

/// Intializes TactorFX Library with default device and output max channels  
TFX_API int initialize();

/// Initializes TactorFX Library with a specified number of channels
TFX_API int initialize(int channelCount);

/// Initializes TactorFX Library with a specified number of channels and specific device number
TFX_API int initialize(int device, int channelCount);

/// Finalizes TactorFX Libary
TFX_API void finalize();

/// Get the default device
TFX_API DeviceInfo getDefaultDevice();

/// Get the current device
TFX_API DeviceInfo getCurrentDevice();

/// Stops all running cues
TFX_API void stopAllCues();

/// Gets list of avaialable ASIO devices
TFX_API std::vector<DeviceInfo> getAvailableDevices();

/// Plays a Cue on a specified channel asynchronously
TFX_API void playCue(int channel, std::shared_ptr<Cue> cue);  

#else

//==============================================================================
// ANSI C INTEFACE (DLL BINDINGS)
//==============================================================================

extern "C" {

TFX_API int initialize();
TFX_API int initializeChannels(int channelCount);
TFX_API int initializeCustom(int device, int channelCount);
TFX_API void finalize();

TFX_API int playCue(int channel,   // channel              [0 to N]
                    int oscType,   // oscillator type      [0=none, 1=sin, 2=sqr, 3=saw, 4=tri]
                    float oscFreq, // oscillator frequency [Hz]
                    float oscAmp,  // oscillator amplitude [0 to 1]
                    int modType,   // modulator type       [0=none, 1=sin, 2=sqr, 3=saw, 4=tri]
                    float modFreq, // modulator frequency  [Hz]
                    float modAmp,  // modulator amplitude  [0 to 1]
                    float A,       // attack time          [s]
                    float S,       // sustain time         [s]
                    float R);      // release time         [s]

TFX_API void stopAllCues();

}; // extern "C"

TFX_API int initialize(int channelCount);
TFX_API int initialize(int device, int channelCount);
TFX_API DeviceInfo getDefaultDevice();
TFX_API DeviceInfo getCurrentDevice();
TFX_API std::vector<DeviceInfo> getAvailableDevices();

#endif // TFX_ANSI_C

} // namespace tfx