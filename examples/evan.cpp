#include "portaudio.h"
#include <string>
#include <iostream>

template <typename T>
void print(const T& printable) {
    std::cout << T << std::endl;
}

int abortWithMessage(const std::string& message) {
    std::cout << message << std::endl;
    return 1;
}

int main(int argc, char const *argv[])
{
    // initialize portaudio
    if (Pa_Initialize() != paNoError)
        return abortWithMessage("Could not initialize portaudio");

    auto num_devices = Pa_GetDeviceCount();
    std::cout << "Num. Devices: " << num_devices << std::endl;

    // terminate portaudio
    if (Pa_Terminate() != paNoError)
        return abortWithMessage("Could not terminate portaudio");

}
