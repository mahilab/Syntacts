#include <TactorFX/TactorFX.hpp>
#include "portaudio.h"
#include <atomic>
#include <mutex>


namespace tfx {

// private namespace
namespace {

std::size_t g_num_ch;         ///< number of channels specified
std::vector<Ptr<Cue>> g_cues; ///< cues
std::mutex g_mutex;           ///< mutex
PaStream* g_stream;           ///< portaudio stream

// portaudio callback method
int callback(const void *inputBuffer, void *outputBuffer,
                       unsigned long framesPerBuffer,
                       const PaStreamCallbackTimeInfo *timeInfo,
                       PaStreamCallbackFlags statusFlags,
                       void *userData)
{
    std::lock_guard<std::mutex> lock(g_mutex);
    /* Cast data passed through stream to our structure. */    
    float *out = (float *)outputBuffer;
    (void)inputBuffer; /* Prevent unused variable warning. */   
    // lock mutex using RA-II lockgaurd (unlocked when we exit scope b/c lock dies)
    for (unsigned long f = 0; f < framesPerBuffer; f++) {
        for (std::size_t c = 0; c < g_num_ch; ++c) {
            out[g_num_ch * f + c] = g_cues[c]->nextSample();
        }
    }
    return 0;
}
    

} // private namespace


bool initialize(std::size_t channelCount) {
    return true;
}

bool finalize() {
    return true;
}

void playCue(std::size_t channel, Ptr<Cue> cue) {

} 

} // namespace tfx