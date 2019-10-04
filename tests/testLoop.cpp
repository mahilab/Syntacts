// system_test.cpp
// Written to test Syntacts system's physical properties 
// Brandon Cambio & Evan Pezent, Sept 2019

#include <MEL/Daq/Quanser/QPid.hpp>
#include <MEL/Daq/Quanser/Q8Usb.hpp>
#include <MEL/Math/Waveform.hpp>
#include <MEL/Core/Timer.hpp>
#include <MEL/Communications/MelShare.hpp>
#include <MEL/Core/Console.hpp>
#include <MEL/Core/Clock.hpp>
#include <MEL/Logging/Log.hpp>
#include <MEL/Logging/Csv.hpp>
#include <MEL/Math/Functions.hpp>
#include <MEL/Devices/Windows/Keyboard.hpp>
#include <Syntacts/Syntacts.hpp>
#include <chrono>
#include <array>
#include <functional>
#include <iostream>
#include <MEL/Utility/Options.hpp>

using namespace mel;
using namespace tact;

#define NUM_CH 8

ctrl_bool g_stop(false);

bool handler(CtrlEvent event) {
    if (event == CtrlEvent::CtrlC)
        g_stop = true;
    return true;
}

int main(int argc, char* argv[]) {

    // register CTRL-C handler
    register_ctrl_handler(handler);

    std::vector<std::string> header = {"Iteration","Time (us)","Signal","Accelerometer"};

    QPid qpid;
    if (!qpid.open())
        return 1;

    if (!qpid.enable())
        return 1;

    qpid.AI.set_channel_numbers({0,1});

    tact::initialize(14, NUM_CH);
    DeviceInfo info = tact::getCurrentDevice();
    mel::print(info.name);
    mel::print(info.index);
    mel::print(info.maxChannels);
    mel::print("Syntacts Initialized");

    // Waveform info
    int ch          = 1;        // Channel
    double dur      = 1.0f;     // Signal duration 
    double wait     = 0.5f;      // wait duration
    float freq      = 175.0f;   // Signal frequency
    float amp       = 0.8f;     // Signal amplitude
    float mod_freq  = 5.0f;    // Modulation Frequency
    float a_time    = 0.2f;     // Attack time
    float d_time    = 0.1f;     // Decay time
    float s_time    = 0.5f;     // Sustain time
    float r_time    = 0.2f;     // Release time 
    int sampleRate  = 50000;
    int iterations  = 10;
    int buffer = 16;
    int samples     = sampleRate * (dur + wait);
    
    auto osc = std::make_shared<SineWave>(freq);
    auto mod = std::make_shared<SineWave>(mod_freq);
    auto pulse = std::make_shared<PulseTrain>(mod_freq, 0.25f);
    auto env = std::make_shared<BasicEnvelope>(dur, amp);
    auto cue = std::make_shared<Cue>(osc, env);

    Timer timer(hertz(sampleRate));
    timer.set_acceptable_miss_rate(1.0);
    Time t;
    Clock clock;

    std::vector<std::array<double, 4>> data(samples);

    std::vector<std::vector<double>> latencies(iterations);

    sleep(seconds(1));

    std::string fileprefix = "testData/20191004/DirectSound2/sine_basic_d1.0_a" + std::to_string(amp) + "_b" + std::to_string(buffer) + "/";
    
    for(int i = 0; i < iterations; i++){
        std::string filepath = fileprefix + "test_" + std::to_string(i) + ".csv";        
        clock.restart();
        tact::play(ch, cue);
        for (int j = 0; j < samples; ++j) {
            t = clock.get_elapsed_time();
            uint64 elap = t.as_microseconds();
            qpid.AI.update();
            data[j] = {(double) j, (double) elap, qpid.AI[0], qpid.AI[1]};
            timer.wait();
        }

        // calc latency
        std::size_t idx = 0;
        for (int s = 0; s < samples; ++s) {
            double sig = std::abs(data[s][2]) ;
            if (sig > 0.005) {
                latencies[i] = {data[s-1][1]};
                print("Latency:" , latencies[i][0]);
                break;
            }
        }


        csv_write_row(filepath,header);
        csv_append_rows(filepath, data);
        mel::print("Data successfully saved!");
    }

    csv_write_rows(fileprefix + "latencies.csv",latencies);    

    tact::finalize();   
    return 0;
}
