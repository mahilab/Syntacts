#include <Tact/Util.hpp>
#include <thread>
#include <chrono>

namespace tact {

namespace {
std::chrono::time_point<std::chrono::high_resolution_clock> g_tic, g_toc;
}

void sleep(double seconds, double max) {
    seconds = std::abs(seconds);
    max = std::abs(max);
    if (seconds != INF)
        std::this_thread::sleep_for(std::chrono::nanoseconds((std::size_t)(seconds * 1000000000)));
    else
        std::this_thread::sleep_for(std::chrono::nanoseconds((std::size_t)(max * 1000000000)));
}


void tic() {
    g_tic = std::chrono::high_resolution_clock::now();
}

double toc() {
    g_toc = std::chrono::high_resolution_clock::now();
    auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(g_toc - g_tic).count();
    return ns / 1000000000.0;
}

};