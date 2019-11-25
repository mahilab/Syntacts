// #define TACT_USE_MALLOC
#include <syntacts>
#include <chrono>
#include <thread>
#include <iostream>

using namespace tact;

void sleep(float sec) {
    std::this_thread::sleep_for(std::chrono::milliseconds((int)(sec * 1000)));
}

int main(int argc, char const *argv[])
{   
    Sequence s = 2 << ASR() * Sine(440);
    Sequence t = 3 << ADSR() * Saw(220) << 100;

    t << s;

    std::cout << s.length() << std::endl;
    std::cout << t.length() << std::endl;

    return 0;
}
