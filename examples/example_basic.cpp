#include <syntacts>
#include <iostream>
#include <thread>
#include <chrono>
#include <typeinfo>

using namespace tact;

void sleep(float sec) {
    if (sec != INF)
        std::this_thread::sleep_for(std::chrono::milliseconds((int)(sec * 1000)));
    else
        std::this_thread::sleep_for(std::chrono::milliseconds((int)(60 * 1000)));
}

std::chrono::time_point<std::chrono::high_resolution_clock> g_tic, g_toc;

void tic() {
    g_tic = std::chrono::high_resolution_clock::now();
}

double toc() {
    g_toc = std::chrono::high_resolution_clock::now();
    auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(g_toc - g_tic).count();
    return ns / 1000000000.0;
}

// Ramp Time = Ramp(0, 1);

int main(int argc, char const *argv[])
{
    
    auto in = 175 * PI * 5 * Time() + 10 * Sine(2);

    Signal x = Square(Signal(in)) * ADSR();

    Library::exportSignal(x,"x",FileFormat::CSV);

    std::cout << Signal::pool().blocksUsed() << std::endl;

    return 0;
}
