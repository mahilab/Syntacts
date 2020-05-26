---
layout: page
title: Building
permalink: /building/
---

## Introduction

This page will walk you through the process of building the Syntacts libraries and GUI from the C++ source files. Syntacts uses [CMake](https://cmake.org/) as its build system, which is scripted through a file named `CMakeLists.txt`. The build process creates three binary objects:

- `syntacts.lib`, the main Syntacts C++ static library
- `syntacts-c.dll`, the Syntacts library exported to a C dynamic library (basis for language bindings)
- `syntacts-gui.exe`, the Syntacts GUI application

When the build completes, the binaries and Syntacts C++ headers will be installed to your system for use. Note that if you don't intend to use the C++ API, you can skip this build process by downloading the latest compiled [Release](https://github.com/mahilab/Syntacts/releases).

## Requirements

- [git](https://git-scm.com/)
- [CMake](https://cmake.org/)

> Make sure **git** and **CMake** are added to your system PATH. To add CMake to your PATH on macOS, open a Terminal and enter the following command:
  ```shell
  >  PATH="/Applications/CMake.app/Contents/bin":"$PATH"
  ```

## Building on Windows

- The recommended C++ compiler on Windows is **Microsoft Visual C++ (MSVC)**. You can acquire the compiler by installing the [Visual Studio](https://visualstudio.microsoft.com/) IDE or the [standalone MSVC build tools](https://visualstudio.microsoft.com/visual-cpp-build-tools/).
- Open PowerShell **as an administrator** in a directory of your choice and run the following commands:
```shell
git clone --recurse-submodules https://github.com/mahilab/Syntacts 
cd Syntacts
mkdir build
cd build
cmake .. -G "Visual Studio 16 2019" -A x64
cmake --build . --target install --config Release
```
- When the build completes, **Syntacts**  will be installed to `C:\Program Files (x86)\Syntacts`. 

> You may need to specify a different CMake generator with `-G` if you have a another version of Visual Studio installed (e.g. `cmake .. -G "Visual Studio 15 2017 Win64"`). 

## Building on macOS

- The recommended C++ compiler on macOS is the Apple supplied Clang compiler. Syntacts has been tested on macOS Catalina with Apple Clang 11.0.0 (clang-1100.0.0.33.17). To check if you have Clang, open a Terminal and enter the command **clang --version**. You may also consider using a mainline version of [Clang](https://clang.llvm.org/).
- Open a Terminal in a directory of your choice and run the following commands:
```shell
> git clone --recurse-submodules https://github.com/mahilab/Syntacts 
> cd Syntacts
> mkdir build && cd build
> cmake .. -DCMAKE_BUILD_TYPE="Release"
> sudo cmake --build . --target install
```
- When the build completes, **Syntacts**  will be installed to `/user/local/...`.
