#include <tact>
#include <chrono>
#include <thread>
#include <iostream>

using namespace tact;

int main(int argc, char const *argv[])
{

    ///////////////////////////////////////////////////////////////////////////
    // SYNTACTS SYNTAX 
    ///////////////////////////////////////////////////////////////////////////

    Signal sig =  0.5f * Sine(440) * Saw(220) * ASR() << 1 << Triangle(400) << ADSR();
    int n = 100000000;
    volatile double sum = 0;
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < n; ++i) {
        float t = (float)i/(float)n;
        sum += sig.sample(t);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << n / (std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() * 0.000001) / 48000.0 << std::endl;

    ///////////////////////////////////////////////////////////////////////////
    // SIGNALS ON THE STACK
    ///////////////////////////////////////////////////////////////////////////

    sum = 0;
    start = std::chrono::high_resolution_clock::now();
    auto sine = Sine(440);
    auto saw  = Saw(220);
    auto env  = ASR(); 
    for (int i = 0; i < n; ++i) {
        float t = (float)i/(float)n;
        sum += sine.sample(t) * saw.sample(t) * env.sample(t);
    }
    end = std::chrono::high_resolution_clock::now();
    std::cout << n / (std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() * 0.000001) / 48000 << std::endl;

    ///////////////////////////////////////////////////////////////////////////
    // SIGNALS INLINED (BEST CASE)
    ///////////////////////////////////////////////////////////////////////////

    sum = 0;
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < n; ++i) {
        float t = (float)i/(float)n;
        float s = std::sin(TWO_PI*440*t);
        float w =  2 * INV_PI * (220 * PI * std::fmod(t, 1.0f / 220) - HALF_PI);
        sum += s * w * env.sample(t);
    }
    end = std::chrono::high_resolution_clock::now();
    std::cout << n / (std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() * 0.000001) / 48000 << std::endl;

    ///////////////////////////////////////////////////////////////////////////

    return 0;
}
