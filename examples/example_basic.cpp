#include <syntacts>
#include <iostream>
#include <thread>
#include <chrono>

using namespace tact;

void sleep(float sec) {
    if (sec != INF)
        std::this_thread::sleep_for(std::chrono::milliseconds((int)(sec * 1000)));
    else
        std::this_thread::sleep_for(std::chrono::milliseconds((int)(60 * 1000)));
}

Signal g_sig = Sine(100);

int main(int argc, char const *argv[])
{
    // Session s;
    // s.open(8);

    // Signal x = Saw(440) * Sine(10) * ASR(1,3,1);
    // s.playAll(x);
    
    // std::cout << x.length() << std::endl;

    // sleep(x.length());

    return 0;
}
