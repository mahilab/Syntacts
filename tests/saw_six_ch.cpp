#include <stdio.h>
#include <math.h>
#include "portaudio.h"
#include <iostream>
#include <atomic>
#include <MEL/Core.hpp>
#include <MEL/Devices/Windows/Keyboard.hpp>

struct MyData {
    float phase;
    float freq;
};

/* This is our own way of opening the stream to not a default. 
On Brandon's computer, the ASIO I/O device number is 14. To find, run "devs.exe" 
and look for the ASIO device with sufficient outputs for your stream */

PaError Pa_OpenOurStream( PaStream** stream,
                              int inputChannelCount,
                              int outputChannelCount,
                              PaSampleFormat sampleFormat,
                              double sampleRate,
                              unsigned long framesPerBuffer,
                              PaStreamCallback *streamCallback,
                              void *userData )
{
    PaError result;
    PaStreamParameters hostApiInputParameters, hostApiOutputParameters;
    PaStreamParameters *hostApiInputParametersPtr, *hostApiOutputParametersPtr;

    if( inputChannelCount > 0 )
    {
        hostApiInputParameters.device = 14;

		if( hostApiInputParameters.device == paNoDevice )
			return paDeviceUnavailable;

        hostApiInputParameters.channelCount = inputChannelCount;
        hostApiInputParameters.sampleFormat = sampleFormat;
        /* defaultHighInputLatency is used below instead of
           defaultLowInputLatency because it is more important for the default
           stream to work reliably than it is for it to work with the lowest
           latency.
         */
        hostApiInputParameters.suggestedLatency =
             Pa_GetDeviceInfo( hostApiInputParameters.device )->defaultHighInputLatency;
        hostApiInputParameters.hostApiSpecificStreamInfo = NULL;
        hostApiInputParametersPtr = &hostApiInputParameters;
    }
    else
    {
        hostApiInputParametersPtr = NULL;
    }

    if( outputChannelCount > 0 )
    {
        hostApiOutputParameters.device = 14;
		if( hostApiOutputParameters.device == paNoDevice )
			return paDeviceUnavailable;

        hostApiOutputParameters.channelCount = outputChannelCount;
        hostApiOutputParameters.sampleFormat = sampleFormat;
        /* defaultHighOutputLatency is used below instead of
           defaultLowOutputLatency because it is more important for the default
           stream to work reliably than it is for it to work with the lowest
           latency.
         */
        hostApiOutputParameters.suggestedLatency =
             Pa_GetDeviceInfo( hostApiOutputParameters.device )->defaultHighOutputLatency;
        hostApiOutputParameters.hostApiSpecificStreamInfo = NULL;
        hostApiOutputParametersPtr = &hostApiOutputParameters;
    }
    else
    {
        hostApiOutputParametersPtr = NULL;
    }


    result = Pa_OpenStream(
                 stream, hostApiInputParametersPtr, hostApiOutputParametersPtr,
                 sampleRate, framesPerBuffer, paNoFlag, streamCallback, userData );
    return result;
}

int sawCallback(const void *inputBuffer, void *outputBuffer,
                       unsigned long framesPerBuffer,
                       const PaStreamCallbackTimeInfo *timeInfo,
                       PaStreamCallbackFlags statusFlags,
                       void *userData)
{
    /* Cast data passed through stream to our structure. */
    MyData *data = (MyData *)userData;
    float *out = (float *)outputBuffer;
    unsigned int i;
    (void)inputBuffer; /* Prevent unused variable warning. */

    for (i = 0; i < framesPerBuffer; i++)
    {
     
        out[6*i]   = data->phase;
        out[6*i+1] = data->phase;
        out[6*i+2] = data->phase;
        out[6*i+3] = data->phase;
        out[6*i+4] = data->phase;
        out[6*i+5] = data->phase;

        data->phase += 2.0f * data->freq / 44100.0f;

        /* When signal reaches top, drop back down. */
        if (data->phase >= 1.0f)
            data->phase -= 2.0f;
    }
    return 0;
}

/*******************************************************************/
static MyData data0;

using namespace mel;

int main(void)
{
    PaStream* stream0;

    /* Initialize our data for use by callback. */
    data0.phase = 0.0f;
    data0.freq = 175;
    
    /* Initialize library before making any other calls. */
    Pa_Initialize();

    auto defIdx = Pa_GetDefaultOutputDevice();
    std::cout << "Default Device Index: " << defIdx << std::endl;
    
    auto info = Pa_GetDeviceInfo(defIdx);
    std::cout << "Default Device Name: " << info->name << std::endl;
    std::cout << "Default Device Max Output Channels: " << info->maxOutputChannels << std::endl;


    std::cout << Pa_GetErrorText(Pa_OpenOurStream(&stream0, 0, 6, paFloat32, 44100, 256, sawCallback, &data0)) << std::endl;
    
    /* Start stream */
    Pa_StartStream(stream0);

    /* Run for 10 seconds */
    Clock clock;
    while (clock.get_elapsed_time() < seconds(10)) {
    }

    // Stop and close stream 
    Pa_StopStream(stream0); 
    Pa_CloseStream(stream0); 

    Pa_Terminate();
    return 0;
}
