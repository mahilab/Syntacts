#pragma once

#include <TactorFX/Generator.hpp>
#include <TactorFX/Oscillator.hpp>
#include <TactorFX/Envelope.hpp>
#include <TactorFX/Cue.hpp>

namespace tfx {    

/// Initializes TactorFX Library with a specified number of channels
int initialize(std::size_t channelCount);

/// Initializes TactorFX Library with a specified number of channels and specific device number
int initialize(std::size_t channelCount, int device_num);

/// List ASIO device numbers, names, and max output channels
void listDevices();

/// Finalizes TactorFX Libary
void finalize();

/// Plays a Cue on a specified channel asynchronously
void playCue(std::size_t channel, Ptr<Cue> cue);     

/// Makes a TactoFX type
template <typename T, typename ...Args>
Ptr<T> make(Args ... args);
    
} // namespace tfx

#include <TactorFX/Detail.inl>