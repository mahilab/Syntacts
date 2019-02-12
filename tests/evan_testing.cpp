#include <stdio.h>
#include <math.h>
#include "portaudio.h"
#include <iostream>

struct MyData {
    float phase;
    float freq;
    float chan;
};

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
        *out++ = data->phase;  /* left */
        *out++;
        /* Generate simple sawtooth phaser that ranges between -1.0 and 1.0. */

        data->phase += 2.0f * data->freq / 44100.0f;

        /* When signal reaches top, drop back down. */
        if (data->phase >= 1.0f)
            data->phase -= 2.0f;
    }
    return 0;
}

/*******************************************************************/
static MyData data1;
static MyData data2;

int main(void);
int main(void)
{
    PaStream* stream1;
    PaStream* stream2;

    /* Initialize our data for use by callback. */
    data1.phase = 0.0f;
    data2.phase = 0.0f;

    data1.freq  = 50;
    data2.freq  = 100;

    /* Initialize library before making any other calls. */
    Pa_Initialize();
    Pa_OpenDefaultStream(&stream1, 0, 2, paFloat32, 44100, 256, sawCallback, &data1);
    //Pa_OpenDefaultStream(&stream2, 0, 2, paFloat32, 44100, 256, sawCallback, &data2);
    Pa_StartStream(stream1);
    Pa_Sleep(5 * 1000);
    // Pa_StartStream(stream2);
    // Pa_Sleep(2 * 1000);
    Pa_StopStream(stream1); 
    Pa_CloseStream(stream1); 
    // Pa_Sleep(2 * 1000);
    // Pa_StopStream(stream2); 
    // Pa_CloseStream(stream2); 

    Pa_Terminate();
    return 0;
}
