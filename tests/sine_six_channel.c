#include <stdio.h>
#include <math.h>
#include "portaudio.h"

#define NUM_SECONDS   (5)
#define SAMPLE_RATE   (44100)
#define FRAMES_PER_BUFFER  (64)

#ifndef M_PI
#define M_PI  (3.14159265)
#endif

#define TABLE_SIZE   (200)
typedef struct
{
    float sine[TABLE_SIZE];
    int phase_0;
    int phase_1;
    int phase_2;
    int phase_3;
    int phase_4;
    int phase_5;
    char message[20];
}
paTestData;


/* This routine will be called by the PortAudio engine when audio is needed.
** It may called at interrupt level on some machines so don't do anything
** that could mess up the system like calling malloc() or free().
*/
static int patestCallback( const void *inputBuffer, void *outputBuffer,
                            unsigned long framesPerBuffer,
                            const PaStreamCallbackTimeInfo* timeInfo,
                            PaStreamCallbackFlags statusFlags,
                            void *userData )
{
    paTestData *data = (paTestData*)userData;
    float *out = (float*)outputBuffer;
    unsigned long i;

    (void) timeInfo; /* Prevent unused variable warnings. */
    (void) statusFlags;
    (void) inputBuffer;
    
    for( i=0; i<framesPerBuffer; i++ )
    {
        out[6*i]   =      data->sine[data->phase_0];  /* left */
        out[6*i+1] = 0.4f*data->sine[data->phase_1];  /* right */
        out[6*i+2] = 0.5f*data->sine[data->phase_2];
        out[6*i+3] = 0.6f*data->sine[data->phase_3];
        out[6*i+4] = 0.7f*data->sine[data->phase_4];
        out[6*i+5] = 0.8f*data->sine[data->phase_5];

        data->phase_0 += 2;
        if( data->phase_0 >= TABLE_SIZE ) data->phase_0 -= TABLE_SIZE;
        data->phase_1 += 1;
        if( data->phase_1 >= TABLE_SIZE ) data->phase_1 -= TABLE_SIZE;
        data->phase_2 += 1;
        if( data->phase_2 >= TABLE_SIZE ) data->phase_2 -= TABLE_SIZE;
        data->phase_3 += 1;
        if( data->phase_3 >= TABLE_SIZE ) data->phase_3 -= TABLE_SIZE;
        data->phase_4 += 1;
        if( data->phase_4 >= TABLE_SIZE ) data->phase_4 -= TABLE_SIZE;
        data->phase_5 += 1;
        if( data->phase_5 >= TABLE_SIZE ) data->phase_5 -= TABLE_SIZE;

        
    }
    
    return paContinue;
}

/*
 * This routine is called by portaudio when playback is done.
 */
static void StreamFinished( void* userData )
{
   paTestData *data = (paTestData *) userData;
   printf( "Stream Completed: %s\n", data->message );
}

/*******************************************************************/
int main(void);
int main(void)
{
    PaStreamParameters outputParameters;
    PaStream *stream;
    PaError err;
    paTestData data;
    int i;

    printf("PortAudio Test: output sine wave. SR = %d, BufSize = %d\n", SAMPLE_RATE, FRAMES_PER_BUFFER);

    /* initialise sinusoidal wavetable */
    for( i=0; i<TABLE_SIZE; i++ )
    {
        data.sine[i] = (float) sin( ((double)i/(double)TABLE_SIZE) * M_PI * 2. );
    }
    data.phase_0 = data.phase_1 = data.phase_2 = data.phase_3 = data.phase_4 = data.phase_5 = 0;
    
    err = Pa_Initialize();
    if( err != paNoError ) goto error;

    outputParameters.device = 14; /* default output device */
    if (outputParameters.device == paNoDevice) {
      fprintf(stderr,"Error: No default output device.\n");
      goto error;
    }
    outputParameters.channelCount = 6;       /* stereo output */
    outputParameters.sampleFormat = paFloat32; /* 32 bit floating point output */
    outputParameters.suggestedLatency = 0.0; //Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    err = Pa_OpenStream(
              &stream,
              NULL, /* no input */
              &outputParameters,
              SAMPLE_RATE,
              FRAMES_PER_BUFFER,
              paClipOff,      /* we won't output out of range samples so don't bother clipping them */
              patestCallback,
              &data );
    if( err != paNoError ) goto error;

    sprintf( data.message, "No Message" );
    err = Pa_SetStreamFinishedCallback( stream, &StreamFinished );
    if( err != paNoError ) goto error;

    err = Pa_StartStream( stream );
    if( err != paNoError ) goto error;

    printf("Play for %d seconds.\n", NUM_SECONDS );
    Pa_Sleep( NUM_SECONDS * 1000 );

    err = Pa_StopStream( stream );
    if( err != paNoError ) goto error;

    err = Pa_CloseStream( stream );
    if( err != paNoError ) goto error;

    Pa_Terminate();
    printf("Test finished.\n");
    
    return err;
error:
    Pa_Terminate();
    fprintf( stderr, "An error occured while using the portaudio stream\n" );
    fprintf( stderr, "Error number: %d\n", err );
    fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
    return err;
}
