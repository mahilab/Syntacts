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

>**Note:** Syntacts uses **git submodules**. Make sure you use the `--recurse-submodules` option when cloning, otherwise your compliation will fail due to missing libraries!

## Building on Windows

Open PowerShell **as an administrator** in a directory of your choice and run the following commands:

```shell
> git clone --recurse-submodules https://github.com/mahilab/Syntacts 
> cd Syntacts
> wget https://www.steinberg.net/asiosdk -outfile "3rdparty/asiosdk.zip"
> Expand-Archive -Path "3rdparty/asiosdk.zip" -DestinationPath "3rdparty"
> mkdir build
> cd build
> cmake .. -G "Visual Studio 16 2019" -A x64
> cmake --build . --target install --config Release
```

This will build **Syntacts** in its entirety and then install it to your system (likely in `C:\Program Files\Syntacts` or `C:\Program Files (x86)\Syntacts`). 

>- The reason we require you to manually download the ASIO SDK with `wget` is because Steinberg's licensing agreement forbids that we distribute it. You can skip these two steps if you don't plan on using ASIO (though, you defintely should if you can!).
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

# Syntacts Amplifier

The Syntacts amplifier is a purpose built, 8-channel amplifier designed for driving vibrotactors from audio sources. The amp uses eight fully differential, Class AB `TI-TPA6211A1-Q1` power amplifiers for a completely noise free operation. Two variants are available: one with an AES-59 DB25 input, great for connecting to high-end devices like the MOTU 24Ao; and one with four 3.5 mm phone inputs, perfect for use with generic audio output or surround sound cards. Both require a 5V power supply and output amplified signal through a universal 2x8 0.1" ptich header. The amp will drive loads greater than 3 ohms, and we have successfully tested it with a vairety of LRAs from [Digikey](https://www.digikey.com/products/en/motors-solenoids-driver-boards-modules/motors-ac-dc/178?k=&pkeyword=&sv=0&pv110=419682&sf=0&FV=-8%7C178&quantity=&ColumnSort=0&page=1&pageSize=25), EAI's [C2 and C3](https://www.eaiinfo.com/tactor-info) voice coil actuators, and Nanoport's [TacHammer](https://nanoport.io/haptics/) actuators. 

CAD/EDA files, bill of materials, and documentation for both variants can be found below. The files have been formated so that you can upload or send them directly to your favorite board house. Need a recommendation? We have used [Sunstone Circuits](https://www.sunstone.com/) for basic PCB printing, and both [Sierra Circuits](https://www.protoexpress.com/) and [Screaming Circuits](https://www.screamingcircuits.com/) for turn-key printing and assembly with great results. Both designs are licensed under the [TAPR Open Hardware License](https://tapr.org/the-tapr-open-hardware-license/). Feel free to modify and/or redistribute the board designs within the confines of the license, but understand that we not liable for any damage or harm the amplifier may cause to you or your equipment. *It is your responsibility safely integrate the amplifier into your projects*.  

|**Version**|**v1.2**|**v3.1**|
|---|---|---|
|**Image**|<img src="https://raw.githubusercontent.com/wiki/mahilab/Syntacts/images/amps/v1.2.png" width="200">|<img src="https://raw.githubusercontent.com/wiki/mahilab/Syntacts/images/amps/v3.1.png" width="200">|
|**Channels**|8|8|
|**Input**|AES-59 DB25|3.5 mm TRS (4)|
|**Output**|2x8 0.1" Pitch Header|2x8 0.1" Pitch Header|
|**Dimensions**|2.375" x 3.5"|2.375" x 3.5"|
|**CAD / BOM**| [syntacts_amp_v1.2.0.zip](https://raw.githubusercontent.com/wiki/mahilab/Syntacts/amps/syntacts_amp_v1.2.0.zip) | [syntacts_amp_v3.1.0.zip](https://raw.githubusercontent.com/wiki/mahilab/Syntacts/amps/syntacts_amp_v3.1.0.zip) | 
|**Manual**|Comming Soon|Comming Soon|
|**License**|[TAPR Open Hardware License](https://tapr.org/the-tapr-open-hardware-license/)|[TAPR Open Hardware License](https://tapr.org/the-tapr-open-hardware-license/)|
