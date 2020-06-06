---
layout: page
title: C++ API
permalink: /cpp/
---

## Introduction

This tutorial will teach you how to setup and use the Syntacts C++ API. 
You should have first built and installed the Syntacts library to your system. 
For this tutorial we will use [CMake](https://cmake.org/) to manage our project and build. 
However, the process should be similar if you are using an IDE such as Visual Studio or Xcode.

## Requirements

- C++ compiler
- [CMake](https://cmake.org/)

## Basic Setup

- If you haven't already, [build and install](building.md) Syntacts.
- Create an empty directory for your project.
- Create a new `CMakeLists.txt` build script with the following contents:

```cmake
# create a new CMake project
cmake_minimum_required(VERSION 3.13.0)
project(MySyntactsProject VERSION 1.0.0)
# find the installed Syntact libraries
find_package(syntacts REQUIRED)
# create an executable from main.cpp and link it against Syntacts
add_executable(myApp main.cpp)
target_link_libraries(myApp syntacts)
```

- Add a C++ source file `main.cpp` with the following contents:

```cpp
#include <syntacts>

using namespace tact;

int main(int argc, char const *argv[])
{
    Session s;
    s.open();
    s.playAll(Sine(440));
    sleep(2);
    return 0;
}
```

## Building

- After you've created `CMakeLists.txt` and `main.cpp`, open Powershell (or Terminal) in the directory that contains those files.
- Make sure CMake is added to your system PATH.
- Run the following commands:

```shell
mkdir build                      # make out-of-source build folder
cd build                         # change directory
cmake ..                         # generate build files from our CMakeLists.txt file
cmake --build . --config Release # build the project
```

- When the build completes, find the output file `myApp.exe` in the build folder and run it. You should hear a two second 440 Hz sine wave from your default speakers.

## API Overview

# Session

- Using Syntacts begins with creating a `Session`, or an audio-context that manages communication with an output device. 
- Devices available for opening can be enumerated and queried for information using a `Session`: 

```cpp
Session session;

for (auto& d : session.getAvailableDevices()) {
    auto& dev = d.second;
    std::cout << dev.index << std::endl;
    std::cout << dev.name  << std::endl;
}
```

- Devices can be opened using the `open` function. 
- The no-argument overload will open the system default audio device (usually the main speakers). 
- Non-default devices can be opened by passing a device index argument or the device name and preferred API:

```cpp
// open default device
session.open();   
session.close();  
// open device by index
session.open(22); 
session.close();  
// open device by name and preferred API
session.open("MOTU Pro Audio", API::ASIO);
session.close();
```

- Device indices represent the combination of a device and an API it supports. 
- Therefore, one device may have multiple indices each with a different API. 
- You can retrieve your device's index and/or name by iterating as in the above example, or by opening the [Syntacts GUI](gui.md) and viewing the information there.

> **Warning:** Device indices are NOT persistent. They may change when devices are power cycled, plugged/unplugged, or when other devices are connected to the PC. If you need a persistent method of opening a device, use the **name + API** version of `open`.

|Relevant Header(s)|Relevant Examples(s)|
|---|---|
|[Session.hpp](https://github.com/mahilab/Syntacts/blob/master/include/Tact/Session.hpp)|[example_devices.cpp](https://github.com/mahilab/Syntacts/blob/master/examples/example_devices.cpp)|

# Signals

- Vibration waveforms are represented by one or more Signals.
- In Syntacts, you can create Signals of different behaviors and lengths.
- Sine, Square, Saw, and Triangle classes are available in Syntacts. These implement typical oscillators with normalized amplitude and infinite duration. 
- Basic oscillators can be created using simple functions:

```cpp
Signal sin = Sine(10); // 10 Hz Sine wave
Signal sqr = Square(100); // 100 Hz Square wave
Signal saw = Saw(440); // 440 Hz Saw wave (audible and ok for speakers)
Signal tri = Triangle(440); // 440 Hz Triangle wave
```

- Envelope and ASR (Attack, Sustain, Release) define amplitude modifiers with finite duration:

```cpp
// This is a basic envelope that specifies amplitude (0.9), and duration (0.5 sec)
Signal bas = Envelope(0.9, 0.5);
// This is an attack (0.1 sec), sustain (0.1 sec), release (0.1 sec) envelope. The sustain amplitude is 1.0. 
// Envelopes can interpolate between amplitudes with different curves, this example uses a smooth s-curve and linear.
Signal asr = ASR(0.1, 0.1, 0.1, 1.0, Curves::Smootheststep(), Curves::Linear());
```

- Signals can be mixed using basic arithmetic. Multiplying and adding Signals can be thought of as an element-wise operation between two vectors.
    - Multiplying two Signals creates a new Signal of duration equal to the    shortest operand.  
    - Adding two Signals creates a new Signal of duration equal to the longest operand.
- Gain and bias can also be applied to Signals with scalar operands.
- Additionally, some signals can accept other Signals as their input arguments.
- Below are basic examples of mixing the Signals from above:

```cpp
Signal sig1 = sqr * sin; // duration is infinite
Signal sig2 = sig1 * asr; // duration is 0.3 seconds
Signal sig3 = 0.5 * (sqr + sin) * asr;
```

- Custom Signals can be created through classes that define the functions `sample` and `length`.
- Signals can be played and stopped:

```cpp
// play Signals on channel 0 and 1
session.play(0, sig1); // plays sig 1 on channel 0
sleep(3); // sig1 plays for 3 seconds
session.stop(0) // stop sig1

session.play(1, sig2); // plays sig2 on channel 1
sleep(sig2.length()); // sig2 plays for its length of 0.3 seconds
// Do not need to stop sig2 because it is finite
```

> **Note:** If testing Signals on a computer, make sure the frequency is high enough to be audible for speakers (ie. 440 Hz). If using tactors, many tactors are closer to 150-250 Hz.

|Relevant Header(s)|Relevant Examples(s)|
|---|---|
|[Signal.hpp](https://github.com/mahilab/Syntacts/blob/master/include/Tact/Signal.hpp)|[example_signals.cpp](https://github.com/mahilab/Syntacts/blob/master/examples/example_signals.cpp)|

# Sequences

- Multiple Signals can be ordered in time using Sequences.
- You can concatenate Signals using the insertion, or left-shift, operator.
- Delay and pause are achieved through the insertion of positive scalar operands.
- Negative scalar operands move the insertion points backwards in time, allowing the overlay or fading of Signals into each other.
- Below is a basic example of Sequences:

```cpp
Signal sigA = Sine(440) * ASR(1,1,1); // create 3 second Signal
Signal sigB = Square(440) * ADSR(1,1,1,1); // create 4 second Signal

Sequence seq1 = sigA << sigB; // 7 second Sequence with sigA before sigB
Sequence seq2 = 1 << sigA << 2 << sigB; // 1 sec delay and 2 sec pause, 10 sec Sequence
Sequence seq3 = sigA << -1 << sigB; // 1 sec fade/overlay between sigA and sigB, 6 sec sequence
```

- Sequences can also be concatenated:

```cpp
Sequence seq4 = seq1 << seq2 << seq3; // Sequence of seq1, seq2, and seq3
// Note this will also modify seq1

// Play seq4 on channel 0 for its length of 23 seconds
session.play(0, seq4);
sleep(seq4.length());
```

- Noise can be added to a Sequence using the `insert` function:

```cpp
seq2.insert(Noise() * Envelope(1), 4); // 1 s of noise starts at the 4 second mark of seq2

session.play(0, seq2); // play seq2 on channel 0
sleep(seq2.length())
```

|Relevant Header(s)|Relevant Examples(s)|
|---|---|
|[Sequence.hpp](https://github.com/mahilab/Syntacts/blob/master/include/Tact/Sequence.hpp)|[example_sequences.cpp](https://github.com/mahilab/Syntacts/blob/master/examples/example_sequences.cpp)|

# Spatializers

- Spatializers map multiple channels to a normalized continuous 1D or 2D spatial representation.
- You can configure a virtual grid to match the physical layout of a tactor array.
- You can then set a virtual target coordinate and radius to play and blend multiple tactors at once.
- Channel positions can be set individually or as uniformly spaced grids.
- Below is an example of creating a spatializer: 

```cpp

```

