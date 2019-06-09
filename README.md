# Syntacts
Vibrotactor Envelope Generation for ASIO Supported Sound Cards

## Dependencies

- [portaudio](http://www.portaudio.com/)
- [Steinberg ASIO SDK](https://www.steinberg.net/en/company/developers.html)

## Building portaudio with ASIO
1. Download the `ASIO` SDK from [here](https://www.steinberg.net/en/company/developers.html) and unzip it to a location of your choice
2. In the directory that *contains the ASIOSDK2.3.2 folder*, run the following commands from an admin elevated Command Prompt or Powershell:
```shell
> git clone https://git.assembla.com/portaudio.git 
> cd portaudio
> mkdir build-msvc-asio
> cd build-msvc-asio
> cmake .. -G "Visual Studio 15 2017 Win64" -DPA_USE_ASIO=ON -DPA_DLL_LINK_WITH_STATIC_RUNTIME=OFF
> cmake --build . --target install --config Release
```
3. This will build `portaudio` with `ASIO` enabled and install it in `C:/Program Files/portaudio`

## Building Syntacts

Comming soon...


