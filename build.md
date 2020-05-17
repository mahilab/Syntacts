---
layout: page
title: Building
permalink: /building/
---

## Overview

This page will walk you through the process of building the Syntacts libraries and GUI from the C++ source files. Syntacts uses [CMake](https://cmake.org/) as its build system, which is scripted through a file named `CMakeLists.txt`. The build creates three binary objects:

|---|---|
|`syntacts.lib`|main Syntacts C++ static library|
|`syntacts-c.dll`|Syntacts C dynamic library which serves the basis of the C# and Python bindings|
|`syntacts-gui.exe`|Syntacts GUI application|

When the build completes, the above binaries, the Syntacts C++ headers, and  the other language binding will be installed to your system for use.

## Requirements

- C++17 compiler
- [git](https://git-scm.com/)
- [CMake](https://cmake.org/)

>- Makre sure **git** and **CMake** are added to your system PATH

## Building on Windows

1. 

Open PowerShell **as an administrator** in a directory of your choice and run the following commands:

```shell
> git clone --recurse-submodules https://github.com/mahilab/Syntacts 
> cd Syntacts
> mkdir build
> cd build
> cmake .. -G "Visual Studio 16 2019" -A x64
> cmake --build . --target install --config Release
```

This will build **Syntacts** in its entirety and then install it to your system (likely in `C:\Program Files\Syntacts` or `C:\Program Files (x86)\Syntacts`). 

>- Syntacts uses **git submodules**. Make sure you use the `--recurse-submodules` option when cloning, otherwise your compilation will fail due to missing libraries!
>- You may need to specify a different CMake generator with `-G` if you have a another version of Visual Studio installed (e.g. `cmake .. -G "Visual Studio 15 2017 Win64"`)

## Building on macOS

Open a Terminal in a directory of your choice and run the following commands:

```shell
> git clone --recurse-submodules https://github.com/mahilab/Syntacts 
> cd Syntacts
> mkdir build && cd build
> cmake .. -DCMAKE_BUILD_TYPE="Release"
> sudo cmake --build . --target install
```

This will build **Syntacts** in its entirety and then install it to your system (likely in `/user/local/...`). 

>- Check your macOS and Clang versions. Syntacts makes use of C++17 features, particularly std::filesystem. You may need to update macOS and/or Apple Clang (e.g. with the command `xcode-select --install`). You may also consider using a mainline version of [Clang](https://clang.llvm.org/). Syntacts has been tested on macOS Catalina with Apple Clang 11.0.0 (clang-1100.0.0.33.17).
