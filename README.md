<img src="https://raw.githubusercontent.com/mahilab/Syntacts/master/logo/logo_text.png" width="500">

Syntacts is an audio-based, haptic rendering framework. The library makes generating tactile vibrations though audio devices as simple as possible, by:

1) Exposing low level access to virtually any hardware and driver (notably ASIO for low latency performance).
2) Providing easy-to-use classes and mechanisms to generate complex vibration waveforms and sequences.
3) Allowing for cue spatialization of multi-channel tactile arrays.
4) Providing bindings for your favorite languages, including Python and C#, as well as example integration with Unity Engine.

# Syntacts Quick Start

### If you came for the GUI and/or Python/C#/Unity bindings...
- Check the [Releases](https://github.com/mahilab/Syntacts/releases) page for the latest pre-compiled binaries. 
- Run the GUI exectuable. On Windows, you may receive a "Windows protected your PC" screen. Click "More info", then "Run anyway".
- Bindings:
  - For **Python**, run `example.py`
  - For **C#**, see the included `README.md`
  - For **Unity**, open `Demo.unity`

### If you came for the C++ version...
1) Get the source code from the latest [Release](https://github.com/mahilab/Syntacts/releases) or by pulling `master`
2) Build and install **Syntacts** for your system (see directions below)
3) Use the [template](https://github.com/mahilab/Syntacts/tree/master/template) to make a new Syntacts project with CMake

### Having issues?
- Head over to the [Issues](https://github.com/mahilab/Syntacts/issues) page and tell us your problem.
- If you get a `VCRUNTIME140.dll missing` or similar issue, first try installing the latest [MSVC redistributable](https://support.microsoft.com/en-us/help/2977003/the-latest-supported-visual-c-downloads)

# Building Syntacts from C++ Source Code

## Requirements

- [git](https://git-scm.com/)
- [CMake](https://cmake.org/)
- C++17 Compiler (e.g. [MSVC](https://visualstudio.microsoft.com/vs/) or [Clang](https://clang.llvm.org/))

## Building on Windows

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

>- Syntacts uses **git submodules**. Make sure you use the `--recurse-submodules` option when cloning, otherwise your compliation will fail due to missing libraries!
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
