---
layout: page
title: Syntacts Draw Example Project
permalink: /tutorials/draw/
---

## Introduction

In this tutorial, we will create a GUI interface that allows us to "draw" 2D tactile animations on the 24-tactor [Syntacts Array](/hardware). We will leverage Syntact's Spatializer to blend the amplitudes of adjacent tactors so that we can create the illusion of continuous motion. The program will be written in C++ using the [mahi-gui](https://github.com/mahilab/mahi-gui) library. The Syntacts Array is driven by a MOTU 24Ao audio interface. Even if you don't have a MOTU 24Ao or the array, this tutorial is still a good example of creating user interfaces around Syntacts. Completed code is [available on GitHub](https://github.com/mahilab/SyntactsDraw).

![GUI](https://raw.githubusercontent.com/wiki/mahilab/Syntacts/images/tut-draw/draw.gif)

## Requirements

- C++ compiler
- [CMake](https://cmake.org/).
- [MOTU 24Ao](https://motu.com/products/avb/24ai-24ao)
- [Syntacts Array](/hardware)

## Setting Up with CMake

We will use CMake as our build system. Our build script, `CMakeLists.txt` is given below. Using `FetchContent`, we can automatically retrieve mahi-gui and Syntacts from their GitHub repositories. Finally, we will make an executable from a source file named `draw.cpp`, and link it to mahi-gui and Syntacts.

```cmake
# CMakeLists.txt
cmake_minimum_required(VERSION 3.7)
project(SyntactsDraw VERSION 0.1.0 LANGUAGES CXX)
include(FetchContent) 

# fetch mahi-gui
FetchContent_Declare(mahi-gui GIT_REPOSITORY https://github.com/mahilab/mahi-gui.git) 
FetchContent_MakeAvailable(mahi-gui)

# fetch syntacts (could also use find_package if installed)
set(SYNTACTS_BUILD_GUI OFF CACHE BOOL "" FORCE)
set(SYNTACTS_BUILD_C_DLL OFF CACHE BOOL "" FORCE)
set(SYNTACTS_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SYNTACTS_BUILD_TESTS OFF CACHE BOOL "" FORCE)
FetchContent_Declare(syntacts GIT_REPOSITORY https://github.com/mahilab/Syntacts.git)
FetchContent_MakeAvailable(syntacts)

add_executable(draw draw.cpp)
target_link_libraries(draw mahi::gui syntacts)
```

## Making an Application Window

First, we make our source file `draw.cpp` in the same directory as `CMakeLists.txt`. We begin by including all the necessary library headers and using declarations, and defining a few constants:

```cpp
// draw.cpp
#include <syntacts>
#include <Mahi/Gui.hpp>
#include <Mahi/Util.hpp>

using namespace tact;
using namespace mahi::gui;
using namespace mahi::util;

constexpr int WIDTH  = 300; // window width
constexpr int HEIGHT = 865; // window height

constexpr int COLS   = 3;   // array columns
constexpr int ROWS   = 8;   // array rows
```

Next, we will subclass `Application` from `mahi-gui`. `Application` provides us with a window and OpenGL context. We don't actually need to know anything about OpenGL for this tutorial, because we will be using `ImGui` (bundled with `mahi-gui`) to create our GUI widgets and 2D graphics. 

```cpp
/// Syntacts Array Drawing Application
class SyntactsDraw : public Application {
public:
    /// Constructor
    SyntactsDraw() : Application(WIDTH,HEIGHT,"SyntactsDraw",false) {
        // set ImGui theme and disable viewports (multi-window)
        ImGui::StyleColorsMahiDark3();
        ImGui::DisableViewports();
    }
private:
    /// Called once per frame
    void update() override {
        // setup ImGui window
        ImGui::SetNextWindowPos({0,0}, ImGuiCond_Always);
        ImGui::SetNextWindowSize({WIDTH,HEIGHT}, ImGuiCond_Always);
        ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar;
        ImGui::Begin("Syntacts Draw", nullptr, flags);

        // ALL APPLICATION CODE TO GO HERE

        ImGui::End();
    }
};

// main, program entry point
int main(int argc, char const *argv[]) {
    SyntactsDraw app;
    app.run();
    return 0;
}
```

## Initializing Syntacts
