#include <stdio.h>
#include <math.h>
#include "portaudio.h"
#include <iostream>
#include <atomic>
#include <MEL/Core.hpp>
#include <MEL/Devices/Windows/Keyboard.hpp>

std::atomic_bool g_flag;

struct MyData {
    float phase;
    float freq;
    float chan;
    int ch; 
};

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

    // PA_LOGAPI_ENTER_PARAMS( "Pa_OpenDefaultStream" );
    // PA_LOGAPI(("\tPaStream** stream: 0x%p\n", stream ));
    // PA_LOGAPI(("\tint inputChannelCount: %d\n", inputChannelCount ));
    // PA_LOGAPI(("\tint outputChannelCount: %d\n", outputChannelCount ));
    // PA_LOGAPI(("\tPaSampleFormat sampleFormat: %d\n", sampleFormat ));
    // PA_LOGAPI(("\tdouble sampleRate: %g\n", sampleRate ));
    // PA_LOGAPI(("\tunsigned long framesPerBuffer: %d\n", framesPerBuffer ));
    // PA_LOGAPI(("\tPaStreamCallback *streamCallback: 0x%p\n", streamCallback ));
    // PA_LOGAPI(("\tvoid *userData: 0x%p\n", userData ));


    if( inputChannelCount > 0 )
    {
        hostApiInputParameters.device = 5;

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
        hostApiOutputParameters.device = 5;
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

    // PA_LOGAPI(("Pa_OpenDefaultStream returned:\n" ));
    // PA_LOGAPI(("\t*(PaStream** stream): 0x%p", *stream ));
    // PA_LOGAPI(("\tPaError: %d ( %s )\n", result, Pa_GetErrorText( result ) ));

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

    int ch = data->ch;

    for (i = 0; i < framesPerBuffer; i++)
    {
        // *out++ = data->phase;  /* left */
        // *out++;

        if (g_flag) {
            out[6*i] =  0.0;
            out[6*i+1] = 0.0;
            out[6*i+2] = 0.0;
            out[6*i+3] = 0.0;
            out[6*i+4] = 0.0;
            out[6*i+5] = 0.0;
        }

        else if (ch == 0) {
            out[6*i] = data->phase;
            out[6*i+1] = 0.0;
            out[6*i+2] = 0.0;
            out[6*i+3] = 0.0;
            out[6*i+4] = 0.0;
            out[6*i+5] = 0.0;
        }
        else if (ch == 1) {
            out[6*i]   = 0.0;
            out[6*i+1] = data->phase;
            out[6*i+2] = 0.0;
            out[6*i+3] = 0.0;
            out[6*i+4] = 0.0;
            out[6*i+5] = 0.0;
        }
        else if (ch == 2) {
            out[6*i]   = 0.0;
            out[6*i+1] = 0.0;
            out[6*i+2] = data->phase;
            out[6*i+3] = 0.0;
            out[6*i+4] = 0.0;
            out[6*i+5] = 0.0;
        }
        else if (ch == 3) {
            out[6*i]   = 0.0;
            out[6*i+1] = 0.0;
            out[6*i+2] = 0.0;
            out[6*i+3] = data->phase;
            out[6*i+4] = 0.0;
            out[6*i+5] = 0.0;
        }
        else if (ch == 4) {
            out[6*i]   = 0.0;
            out[6*i+1] = 0.0;
            out[6*i+2] = 0.0;
            out[6*i+3] = 0.0;
            out[6*i+4] = data->phase;
            out[6*i+5] = 0.0;
        }
        else if (ch == 5) {
            out[6*i]   = 0.0;
            out[6*i+1] = 0.0;
            out[6*i+2] = 0.0;
            out[6*i+3] = 0.0;
            out[6*i+4] = 0.0;
            out[6*i+5] = data->phase;
        }
        /* Generate simple sawtooth phaser that ranges between -1.0 and 1.0. */

        data->phase += 2.0f * data->freq / 44100.0f;

        /* When signal reaches top, drop back down. */
        if (data->phase >= 1.0f)
            data->phase -= 2.0f;
    }
    return 0;
}

/*******************************************************************/
static MyData data0;
static MyData data1;
static MyData data2;
static MyData data3;
static MyData data4;
static MyData data5;

using namespace mel;

int main(void)
{
    PaStream* stream0;
    PaStream* stream1;
    PaStream* stream2;
    PaStream* stream3;
    PaStream* stream4;
    PaStream* stream5;

    g_flag = true;

    /* Initialize our data for use by callback. */
    data0.phase = 0.0f;
    data1.phase = 0.0f;
    data2.phase = 0.0f;
    data3.phase = 0.0f;
    data4.phase = 0.0f;
    data5.phase = 0.0f;

    data0.freq = 175;
    data1.freq = 175;
    data2.freq = 175;
    data3.freq = 175;
    data4.freq = 175;
    data5.freq = 175;
    
    // Do not change these, assigns data to channels
    data0.ch = 0;
    data1.ch = 1;
    data2.ch = 2;
    data3.ch = 3;
    data4.ch = 4;
    data5.ch = 5;



    /* Initialize library before making any other calls. */
    Pa_Initialize();

    auto defIdx = Pa_GetDefaultOutputDevice();
    std::cout << "Default Device Index: " << defIdx << std::endl;
    
    auto info = Pa_GetDeviceInfo(defIdx);
    std::cout << "Default Device Name: " << info->name << std::endl;
    std::cout << "Default Device Max Output Channels: " << info->maxOutputChannels << std::endl;


    std::cout << Pa_GetErrorText(Pa_OpenOurStream(&stream0, 0, 6, paFloat32, 44100, 256, sawCallback, &data0)) << std::endl;
    std::cout << Pa_GetErrorText(Pa_OpenOurStream(&stream1, 0, 6, paFloat32, 44100, 256, sawCallback, &data1)) << std::endl;
    std::cout << Pa_GetErrorText(Pa_OpenOurStream(&stream2, 0, 6, paFloat32, 44100, 256, sawCallback, &data2)) << std::endl;
    std::cout << Pa_GetErrorText(Pa_OpenOurStream(&stream3, 0, 6, paFloat32, 44100, 256, sawCallback, &data3)) << std::endl;
    std::cout << Pa_GetErrorText(Pa_OpenOurStream(&stream4, 0, 6, paFloat32, 44100, 256, sawCallback, &data4)) << std::endl;
    std::cout << Pa_GetErrorText(Pa_OpenOurStream(&stream5, 0, 6, paFloat32, 44100, 256, sawCallback, &data5)) << std::endl;
    
    // Start all streams simultaneously
    Pa_StartStream(stream0);
    Pa_StartStream(stream1);
    Pa_StartStream(stream2);
    Pa_StartStream(stream3);
    Pa_StartStream(stream4);
    Pa_StartStream(stream5);


    Timer timer(hertz(1000));
    while (timer.get_elapsed_time() < seconds(10)) {
        if (Keyboard::is_key_pressed(Key::Space))
            g_flag = false;
        else 
            g_flag = true;
    }




    // Stop and close all streams 
    Pa_StopStream(stream0); 
    Pa_CloseStream(stream0); 

    Pa_StopStream(stream1); 
    Pa_CloseStream(stream1); 

    Pa_StopStream(stream2); 
    Pa_CloseStream(stream2);

    Pa_StopStream(stream3); 
    Pa_CloseStream(stream3); 

    Pa_StopStream(stream4); 
    Pa_CloseStream(stream4); 

    Pa_StopStream(stream5); 
    Pa_CloseStream(stream5);  


    Pa_Terminate();
    return 0;
}
