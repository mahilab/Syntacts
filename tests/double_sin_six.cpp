#include <stdio.h>
#include <math.h>
#include "portaudio.h"
#include <iostream>
#include <atomic>
#include <MEL/Core.hpp>
#include <MEL/Devices/Windows/Keyboard.hpp>
#include <cmath>
#include <MEL/Utility/System.hpp>


struct MyData {
    float sin_phase;
    float sin_freq;
    float saw_freq;
    float saw_phase;
    float rise;
    float rise_time;
    float amp;
};

/* This is our own way of opening the stream to not a default. 
On Brandon's computer, the ASIO I/O device number is 14. On Evan's computer, it is device XX. 
To find, run "devs.exe" and look for the ASIO device with sufficient outputs for your stream */

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

int sineCallback(const void *inputBuffer, void *outputBuffer,
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
     
        out[6*i]   = data->amp * sin(data->saw_phase) * sin(data->sin_phase) * (1-exp (-data->rise));
        out[6*i+1] = data->amp * sin(data->saw_phase) * sin(data->sin_phase) * (1-exp (-data->rise));
        out[6*i+2] = data->amp * sin(data->saw_phase) * sin(data->sin_phase) * (1-exp (-data->rise));
        out[6*i+3] = data->amp * sin(data->saw_phase) * sin(data->sin_phase) * (1-exp (-data->rise));
        out[6*i+4] = data->amp * sin(data->saw_phase) * sin(data->sin_phase) * (1-exp (-data->rise));
        out[6*i+5] = data->amp * sin(data->saw_phase) * sin(data->sin_phase) * (1-exp (-data->rise));

        data->rise += data->rise_time/44100/3;
        data->sin_phase += 2.0f * 3.14159265358979323846f * data->sin_freq / 44100.0f;
        data->saw_phase += 2.0f * 3.14159265358979323846f * data->saw_freq / 44100.0f;
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
    data0.sin_phase = 0.0f;
    data0.sin_freq = 175;
    data0.saw_freq = 10;
    data0.saw_phase = 0.0f;
    data0.rise_time = 1; 
    data0.amp = 1.0f;
    
    /* Initialize library before making any other calls. */
    Pa_Initialize();

    std::cout << Pa_GetErrorText(Pa_OpenOurStream(&stream0, 0, 6, paFloat32, 44100, 256, sineCallback, &data0)) << std::endl;
    
    /* Start stream */
    Pa_StartStream(stream0);

    /* Run for 10 seconds */
    Clock clock;
    while (clock.get_elapsed_time() < seconds(10)) {
        mel::sleep(milliseconds(100));
    }

    // Stop and close stream 
    Pa_StopStream(stream0); 
    Pa_CloseStream(stream0); 

    Pa_Terminate();
    return 0;
}
