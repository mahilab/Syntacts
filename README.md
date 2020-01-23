<img src="https://raw.githubusercontent.com/mahilab/Syntacts/master/logo/logo_text.png" width="500">

## Building from Source

### Requirements

- [git](https://git-scm.com/)
- [CMake](https://cmake.org/)
- C++17 Compiler (e.g. [MSVC](https://visualstudio.microsoft.com/vs/))

### Building portaudio with ASIO support

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
> cmake --build . --target install --config Debug
```

This will build `Syntacts` in its entirity in both Release and Debug configurations, and then install them to your system (likely in `C:\Program Files (x86)\Syntacts`, but possibly elsewhere if you have CMake configured differently). A few details to note:

- The `git clone` option `--recurse-submodules` is required because we use git submodules. If you don't pass this option, then libraries in `3rdparty` **will not** be cloned and your compliation will fail.
- The reason we require you to manually download ASIO with `wget` is because Steinberg's licensing agreement forbids that we distribute it. You can skip these two steps if you don't plan on using ASIO (though, you defintely should if you can!).
- You may need to specify a different CMake generator with `-G` if you have a another version of Visual Studio installed (e.g. `cmake .. -G "Visual Studio 15 2017 Win64"`)
