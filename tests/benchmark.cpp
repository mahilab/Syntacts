#include "Syntacts/Syntacts.hpp"
#include <chrono>
#include <iostream>

using namespace tact;

int main(int argc, char const *argv[])
{
    auto osc = std::make_shared<SineWave>(175);
    auto mod = std::make_shared<SineWave>(25);
    auto env = std::make_shared<ADSR>(20,20,20,20);
    auto cue = std::make_shared<Cue>(osc, mod, env);
    volatile float x;

    auto start = std::chrono::high_resolution_clock::now();
    for (std::size_t i = 0; i < 60 * 44100; ++i) {
        x = cue->nextSample();
    }
    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "Elapsed time in seconds : " 
		      << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
		      << " ms";

    return 0;
}
