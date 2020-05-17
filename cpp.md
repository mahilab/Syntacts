---
layout: page
title: C++ API
permalink: /cpp/
---

## Overview

This tutorial will teach you how to setup and use the Syntacts C++ API. 
You should have first built and installed the Syntacts library to your system. 
For this tutorial we will use [CMake](https://cmake.org/) to manage our project and build. 
However, the process should be similar if you are using an IDE such as Visual Studio or Xcode.

## Requirements

[CMake](https://cmake.org/)

## Basic Setup

1. If you haven't already, [build and install](building.md) Syntacts.
2. Create an empty directory for your project.
3. Create a new `CMakeLists.txt` build script with the following contents:

```
cmake_minimum_required(VERSION 3.13.0)
project(MySyntactsProject VERSION 1.0.0)

find_package(syntacts REQUIRED)

add_executable(myApp main.cpp)
target_link_libraries(myApp syntacts)
```
4. Add a C++ source file `main.cpp` with the following contents:
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

1. After you've created `CMakeLists.txt` and `main.cpp`, open Powershell (or Terminal) in the current directory.
2. Run the following commands:
```shell
mkdir build                      # make out-of-source build folder
cd build                         # change directory
cmake ..                         # generate build files from our CMakeLists.txt file
cmake --build . --config Release # build the project
```
3. When the build completes, find the output file `myApp.exe` in the build folder and run it. You should hear a two second 440 Hz sine wave from your default speakers.


