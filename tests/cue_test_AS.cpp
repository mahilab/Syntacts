#include <MEL/Communications/MelShare.hpp>  // for plotting
#include <MEL/Devices/Windows/Keyboard.hpp> // for keyboard
#include <MEL/Utility/System.hpp>           // for sleep()
#include <MEL/Math/Random.hpp>              // for random
#include <MEL/Core/Console.hpp>             // for print

#include <thread>
#include <mutex>
#include <functional>
#include <array>
#include <memory>
#include <atomic>

typedef mel::Keyboard KB;
using mel::Key;
using mel::sleep;
using mel::MelShare;
using mel::print;
using mel::random;

// define constants
#define PI     3.14159265358f
#define TWO_PI 6.28318530718f
#define NUM_CH 6

//=============================================================================
// CUES
//=============================================================================

/// Cue Abstract Base Class
class Cue {
public:
    /// Override to get the next sample (return 0 by default)
    virtual float nextSample() { 
        return 0.0f; 
    } 

};


/// Sinavewave Cue Impl.
class SinWave : public Cue {
public:
    /// Constructor
    SinWave(float freq, float amp, float dur, float a_time) : 
        m_freq(freq), m_amp(amp), m_dur(dur), m_cur(0.0f), m_a_time(a_time) { }
    /// Gets the next sample
    float nextSample() override {
        float sample = m_cur < m_dur ? m_amp * (1-std::exp(-3*m_cur / m_a_time)) * std::sin(TWO_PI * m_freq * m_cur) : 0.0f;
        m_cur += 0.001f; // 1ms window
        return sample;
    }
private:
    float m_freq, m_amp, m_dur, m_cur, m_a_time;
};


/// Sawwave Cue Impl
class SawWave : public Cue {
public:
    /// Constructor
    SawWave(float freq, float amp, float dur, float a_time) : 
        m_freq(freq), m_amp(amp), m_dur(dur), m_cur(0.0f), m_a_time(a_time) { }
    /// Gets the next sample
    float nextSample() override {
        float sample = m_cur < m_dur ? -2 / PI * m_amp * (1-std::exp(-3*m_cur / m_a_time)) * std::atan(std::cos(PI * m_freq * m_cur) / std::sin(PI * m_freq * m_cur)) : 0.0f;
        m_cur += 0.001f; // 1ms window
        return sample;
    }
private:
    float m_freq, m_amp, m_dur, m_cur, m_a_time;
};

//=============================================================================
// AUDIO THEAD
//=============================================================================

/// Typedef Cue Ptr
typedef std::shared_ptr<Cue> CuePtr;

/// Global Cue Array (Indexed by Channel)
std::array<CuePtr, NUM_CH> g_cues;

/// Global Stop Flag
std::atomic<bool> g_stop(false);

/// Mutex that gaurds g_cues
std::mutex g_mutex;

/// Simulated Audio Thread (for demonstration only)
void audio_thread_func() {
    print("Audio Thread Started");
    std::vector<double> output_buffer(NUM_CH);
    MelShare ms("tactorFX");
    while (!g_stop) {
        // mutex protected scope
        {
            // lock mutex using RA-II lockgaurd (unlocked when we exit scope b/c lock dies)
            std::lock_guard<std::mutex> lock(g_mutex);
            for (std::size_t i = 0; i < NUM_CH; ++i)
                output_buffer[i] = g_cues[i]->nextSample();
        }
        // write buffer
        ms.write_data(output_buffer);
        // sleep
        sleep(mel::milliseconds(1)); // 1ms window
    }
    print("Audio Thread Ended");
}

//=============================================================================
// TACTORFX USER INTEFACE?
//=============================================================================

// we wil want to namespace our interface
namespace tfx {

void play_cue(std::size_t chn, CuePtr cue) {
    // lock mutex using RA-II lockgaurd (unlocked when function ends b/c lock dies)
    std::lock_guard<std::mutex> lock(g_mutex);
    g_cues[chn] = cue;
    print("Played a Cue!");
}

} // namespace tfx

//=============================================================================
// MAIN THREAD
//=============================================================================

int main(int argc, char const *argv[])
{
    print("Main Thread Started");
    // init g_cues with empty cues
    for (auto& cue : g_cues)
        cue = std::make_shared<Cue>();       
    // start audio thread (equivalent to Pa_Initialize)
    std::thread audio_thread(audio_thread_func);
    // keys associated with each channel
    std::array<Key, NUM_CH> chn_keys = {Key::Num1, Key::Num2, Key::Num3, Key::Num4, Key::Num5, Key::Num6};    
    // start main loop
    while (!g_stop) {
        // exit if requested
        if (KB::is_key_pressed(Key::Escape))
            g_stop = true;

        for (std::size_t ch = 0; ch < NUM_CH; ++ch) {
            if (KB::is_key_pressed(chn_keys[ch])) {
                float freq = (float)random(1, 5);  // small so we can see it on melscope
                float amp  = (float)random(0.5, 1.0); 
                float dur  = (float)random(5, 10);
                float a_time = (float)random(1,3);
                if (KB::is_key_pressed(Key::S)) {
                    CuePtr cue = std::make_shared<SinWave>(freq, amp, dur, a_time);
                    tfx::play_cue(ch, cue);
                }                
                else if (KB::is_key_pressed(Key::W)) {
                    CuePtr cue = std::make_shared<SawWave>(freq, amp, dur, a_time);
                    tfx::play_cue(ch, cue);
                }
            }
        }
        // sleep so key presses aren't spammed (less than perfect)
        sleep(mel::milliseconds(100));
    }    
    audio_thread.join();
    print("Main Thread Ended");
    return 0;
}
