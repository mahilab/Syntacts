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
        if (KB::is_key_pressed(Key::L)) {
            if (kbClock.get_elapsed_time() > mel::milliseconds(250)) {
                    //tfx::listDevices();
                    kbClock.restart();
            }
        }

        else if (KB::is_key_pressed(Key::B) && kbClock.get_elapsed_time() > mel::milliseconds(250)) {
            auto osc = make<SineWave>(175.0f, 0.05f);
            // auto mod = make<SineWave>(25.0f,1.0f);
            auto env = make<ASR>(1.f, 1.f, 1.f);
            auto cue = make<Cue>(osc, env);

            for (int i = 1; i < 2; ++i)
                tfx::playCue(i, cue);

            kbClock.restart();
        }

        for (int ch = 0; ch < NUM_CH; ++ch) {
            if (KB::is_key_pressed(chn_keys[ch])) {
                float freq     = 175;  // 175 for Evan's tactors
                float amp      = 0.05f; 
                float dur      = 2.0f;
                float a_time   = 1.0f;
                float s_time   = 1.0f;
                float r_time   = 1.0f;
                float mod_freq = 5;
                if (kbClock.get_elapsed_time() > mel::milliseconds(250)) {
                    if (KB::is_key_pressed(Key::S)) {
                        // just a sinwave for dur time
                        auto osc = make<SquareWave>(freq, amp);
                        auto cue = make<Cue>(osc, dur);  
                        tfx::playCue(ch, cue);
                    }                
                    else if (KB::is_key_pressed(Key::W)) {
                        // a sawave with an AS envelope
                        auto osc = make<SineWave>(freq, amp);
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
                    else if (KB::is_key_pressed(Key::T)) {
                        // chaining
                        auto cue = make<Cue>();
                        cue->chain<TriWave>(freq, amp);
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
