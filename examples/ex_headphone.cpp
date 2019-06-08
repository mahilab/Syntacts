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

#define NUM_CH 2
#define DEVICE 16

using namespace tfx;

int main(int argc, char const *argv[])
{
    tfx::initialize(DEVICE, NUM_CH);
    DeviceInfo info = tfx::getCurrentDevice();
    mel::print("Device Name: ", info.name);
    mel::print("Device Index: ", info.index);
    mel::print("Max Channels: ",info.maxChannels);

    mel::Clock kbClock;

    std::array<Key, NUM_CH> chn_keys = {Key::Num1, Key::Num2};    
    while (!KB::is_key_pressed(Key::Escape)) {
        if (KB::is_key_pressed(Key::L)) {
            if (kbClock.get_elapsed_time() > mel::milliseconds(250)) {
                    tfx::getAvailableDevices();
                    kbClock.restart();
            }
        }

        else if (KB::is_key_pressed(Key::B) && kbClock.get_elapsed_time() > mel::milliseconds(250)) {
            auto osc = std::make_shared<SineWave>(175.0f);
            // auto mod = std::make_shared<SineWave>(25.0f,1.0f);
            auto env = std::make_shared<ASR>(1.f, 1.f, 1.f, 0.05f);
            auto cue = std::make_shared<Cue>(osc, env);

            for (int i = 1; i < 2; ++i)
                tfx::playCue(i, cue);

            kbClock.restart();
        }

        if (KB::is_key_pressed(Key::U) && kbClock.get_elapsed_time() > mel::milliseconds(250)) {
            tfx::playCue(0, 1, 175, 0, 0, 1.0f, 0, 0.5f, 0);
            kbClock.restart();
        }

        for (int ch = 0; ch < NUM_CH; ++ch) {
            if (KB::is_key_pressed(chn_keys[ch])) {
                float freq     = 175;  // 175 for Evan's tactors
                float amp      = 0.5f; 
                float dur      = 2.0f;
                float a_time   = 1.0f;
                float s_time   = 1.0f;
                float r_time   = 1.0f;
                float mod_freq = 5;
                if (kbClock.get_elapsed_time() > mel::milliseconds(250)) {
                    if (KB::is_key_pressed(Key::S)) {
                        // just a sinwave for dur time
                        auto osc = std::make_shared<SquareWave>(freq);
                        auto cue = std::make_shared<Cue>(osc, dur, amp);  
                        tfx::playCue(ch, cue);
                    }                
                    else if (KB::is_key_pressed(Key::W)) {
                        // a sawave with an AS envelope
                        auto osc = std::make_shared<SineWave>(freq);
                        auto env = std::make_shared<ASR>(a_time, s_time, r_time, amp); 
                        auto cue = std::make_shared<Cue>(osc, env);                
                        tfx::playCue(ch, cue);
                    }
                    else if (KB::is_key_pressed(Key::C)) {
                        // chaining
                        auto cue = std::make_shared<Cue>();
                        cue->chain<SquareWave>(freq);
                        cue->chain<SineWave>(mod_freq);
                        cue->chain<ASR>(a_time, s_time, r_time, amp);
                        tfx::playCue(ch, cue);
                    }
                    else if (KB::is_key_pressed(Key::Q)) {
                        // chaining
                        auto cue = std::make_shared<Cue>();
                        cue->chain<SquareWave>(freq);
                        cue->chain<ASR>(a_time, s_time, r_time, amp);
                        tfx::playCue(ch, cue);
                    }
                    else if (KB::is_key_pressed(Key::T)) {
                        // chaining
                        auto cue = std::make_shared<Cue>();
                        cue->chain<TriWave>(freq);
                        cue->chain<ASR>(a_time, s_time, r_time, amp);
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
