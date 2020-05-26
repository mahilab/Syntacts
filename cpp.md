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

- After you've created `CMakeLists.txt` and `main.cpp`, open Powershell (or Terminal) in the current directory.
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

Using Syntacts begins with creating a `Session`, or an audio-context that manages communication with an output device. Devices available for opening can be enumerated and queried for information using a `Session`: 

```cpp
Session session;

for (auto& d : session.getAvailableDevices()) {
    auto& dev = d.second;
    std::cout << dev.index << std::endl;
    std::cout << dev.name  << std::endl;
}
```

Devices can be opened using the `open` function. The no-argument overload will open the system default audio device (usually the main speakers). Non-default devices can be opened by passing a device index argument:

```cpp
session.open();   // opens default device
session.close();  // closes current device
session.open(22); // opens device 22
```

You can retrieve your device's index by iterating the available devices as in the above example, or by opening the [Syntacts GUI](gui.md) and viewing the available devices there. 

> **Warning:** Device indices are NOT persistent. They may change when devices are power cycled, plugged/unplugged, or when other devices are connected to the PC. However, generally they will not change if the devices connected to the PC have not changed. If you need a more robust method of opening your device, consider iterating available devices and checking against string `name`, which *is* persistent. 

|Relevant Header(s)|Relevant Examples(s)|
|---|---|
|[Session.hpp](https://github.com/mahilab/Syntacts/blob/master/include/Tact/Session.hpp)|[example_devices.cpp](https://github.com/mahilab/Syntacts/blob/master/examples/example_devices.cpp)|

# Signals

*Coming soon*

# Sequences

*Coming soon*

# Spatializers

*Coming soon*

