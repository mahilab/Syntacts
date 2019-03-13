#pragma once

#include <TactorFX/Generator.hpp>
#include <TactorFX/Oscillator.hpp>
#include <TactorFX/Envelope.hpp>
#include <TactorFX/Cue.hpp>

namespace tfx {  

/// Struct holding ASIO device information
struct DeviceInfo {
    int index;         ///< device index
    std::string name;          ///< device name
    int maxChannels;   ///< maximum number of output channels
};

/// Intializes TactorFX Library with default device and output max channels  
int initialize();

/// Initializes TactorFX Library with a specified number of channels
int initialize(int channelCount);

/// Initializes TactorFX Library with a specified number of channels and specific device number
int initialize(int device, int channelCount);

/// Finalizes TactorFX Libary
void finalize();

/// Gets list of avaialable ASIO devices
std::vector<DeviceInfo> getAvailableDevices();

/// Get the default device
DeviceInfo getDefaultDevice();

/// Get the current device
DeviceInfo getCurrentDevice();

/// Plays a Cue on a specified channel asynchronously
void playCue(int channel, Ptr<Cue> cue);   

/// Stops all running cues
void stopAllCues();

/// Makes a TactoFX type
template <typename T, typename ...Args>
Ptr<T> make(Args ... args);
    
} // namespace tfx

#include <TactorFX/Detail.inl>