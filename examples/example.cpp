#include <TactorFX/TactorFX.hpp>
#include <MEL/Communications/MelShare.hpp>  // for plotting
#include <MEL/Devices/Windows/Keyboard.hpp> // for keyboard
#include <MEL/Utility/System.hpp>           // for sleep()
#include <MEL/Math/Random.hpp>              // for random
#include <MEL/Core/Console.hpp>             // for print
#include <MEL/Core/Clock.hpp>
#include <array>

typedef mel::Keyboard KB;
using mel::Key;
using mel::sleep;
using mel::MelShare;
using mel::print;
using mel::random;

#define NUM_CH 6

using namespace tfx;

int main(int argc, char const *argv[])
{
    tfx::initialize(NUM_CH);

    mel::Clock kbClock;

    std::array<Key, NUM_CH> chn_keys = {Key::Num1, Key::Num2, Key::Num3, Key::Num4, Key::Num5, Key::Num6};    
    while (!KB::is_key_pressed(Key::Escape)) {
        for (std::size_t ch = 0; ch < NUM_CH; ++ch) {
            if (KB::is_key_pressed(chn_keys[ch])) {
                float freq     = 50;  // for Evan's tactors
                float amp      = 1.0f; 
                float dur      = 0.5f;
                float a_time   = 0.25f;
                float s_time   = 0.5f;
                float r_time   = 0.25f;
                float mod_freq = 5;
                if (kbClock.get_elapsed_time() > mel::milliseconds(250)) {
                    if (KB::is_key_pressed(Key::S)) {
                        // just a sinwave for dur time
                        auto osc = make<SineWave>(freq, amp);
                        auto cue = make<Cue>(osc, dur);  
                        tfx::playCue(ch, cue);
                    }                
                    else if (KB::is_key_pressed(Key::W)) {
                        // a sawave with an AS envelope
                        auto osc = make<SawWave>(freq, amp);
                        auto env = make<ASR>(a_time, s_time, r_time); 
                        auto cue = make<Cue>(osc, env);                
                        tfx::playCue(ch, cue);
                    }
                    else if (KB::is_key_pressed(Key::C)) {
                        // chaining
                        auto cue = make<Cue>();
                        cue->chain<SquareWave>(freq, amp);
                        cue->chain<SineWave>(mod_freq, amp);
                        cue->chain<ASR>(a_time, s_time, r_time);
                        tfx::playCue(ch, cue);
                    }
                    else if (KB::is_key_pressed(Key::Q)) {
                        // chaining
                        auto cue = make<Cue>();
                        cue->chain<SquareWave>(freq, amp);
                        cue->chain<ASR>(a_time, s_time, r_time);
                        tfx::playCue(ch, cue);
                    }
                    kbClock.restart();
                }
            }
        }
    }      

    tfx::finalize();

    return 0;
}
