// system_test.cpp
// Written to test Syntacts system's physical properties 
// Brandon Cambio & Evan Pezent, Sept 2019

#include <MEL/Daq/Quanser/Q8Usb.hpp>
#include <MEL/Math/Waveform.hpp>
#include <MEL/Core/Timer.hpp>
#include <MEL/Communications/MelShare.hpp>
#include <MEL/Core/Console.hpp>
#include <MEL/Core/Clock.hpp>
#include <MEL/Logging/Log.hpp>
#include <MEL/Logging/Csv.hpp>
#include <MEL/Devices/Windows/Keyboard.hpp>
#include <Syntacts/Syntacts.hpp>
#include <chrono>
#include <array>
#include <functional>
#include <iostream>

using namespace mel;
using namespace tact;

#define NUM_CH 24

ctrl_bool g_stop(false);

bool handler(CtrlEvent event) {
    if (event == CtrlEvent::CtrlC)
        g_stop = true;
    return true;
}

int main(int argc, char* argv[]) {

    // register CTRL-C handler
    register_ctrl_handler(handler);

    std::vector<std::string> header = {"Iteration","Time (us)","AI[0]","AI[1]","AI[2]","AI[3]","AI[4]","AI[5]","AI[6]","AI[7]"};

    uint64 iter = 0; 

    Q8Usb q8;
    if (!q8.open())
        return 1;

    if (!q8.enable())
        return 1;

    Timer timer(hertz(3000));
    Time t;
    Clock clock;

    tact::initialize(NUM_CH);
    DeviceInfo info = tact::getCurrentDevice();
    mel::print(info.name);
    mel::print(info.index);
    mel::print(info.maxChannels);
    mel::print("Syntacts Initialized");

    // Waveform info
    int ch          = 1;        // Channel
    float dur       = 1.0f;     // Signal duration 
    float freq      = 175.0f;   // Signal frequency
    float amp       = 0.085f;     // Signal amplitude
    float mod_freq  = 5.0f;    // Modulation Frequency
    float a_time    = 0.2f;     // Attack time
    float d_time    = 0.1f;     // Decay time
    float s_time    = 0.5f;     // Sustain time
    float r_time    = 0.2f;     // Release time 
    
    auto osc = std::make_shared<SineWave>(freq);
    auto mod = std::make_shared<SineWave>(mod_freq);
    auto pulse = std::make_shared<PulseTrain>(mod_freq, 0.25f);
    auto env = std::make_shared<BasicEnvelope>(dur, amp);
    auto cue = std::make_shared<Cue>(osc, env);

    std::array<std::array<double, 10>, 4500> data;
    Keyboard::wait_for_key(Key::Num1); // Added to pause to ensure 
    while (!g_stop){
        for(int i = 0; i < 10; i++){
            // mel::print("Test " + std::to_string(i) + " begun");
            std::string filepath = "testData/20190923/_3000_sine_basic_1.0_16_"+ std::to_string(amp) +"_WDM_test_" + std::to_string(i) + ".csv";
           
            clock.restart();
            q8.AO[7] = 10;
            q8.update_output();
            tact::play(ch, cue);
            while (clock.get_elapsed_time() < seconds((double)dur + 0.5f)){
                t = clock.get_elapsed_time();
                uint64 elap = t.as_microseconds();
                q8.AI.update();
                data[iter] = {(double) iter, (double) elap, q8.AI[0], q8.AI[1], q8.AI[2], q8.AI[3], q8.AI[4], q8.AI[5], q8.AI[6], q8.AI[7]};
                iter++;
                timer.wait();
            }
            csv_write_row(filepath,header);
            csv_append_rows(filepath, data);
            mel::print("Data successfully saved!");
            q8.AO[7] = 0;
            q8.update_output();
            iter = 0;
        }
        g_stop = true;
    }
    tact::finalize();   
    return 0;
}
