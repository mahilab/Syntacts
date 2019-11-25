#include <syntacts>
#include <chrono>
#include <thread>
#include <iostream>
#include <numeric>

using namespace tact;

std::chrono::time_point<std::chrono::high_resolution_clock> g_tic, g_toc;

void tic() {
    g_tic = std::chrono::high_resolution_clock::now();
}

double toc() {
    g_toc = std::chrono::high_resolution_clock::now();
    auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(g_toc - g_tic).count();
    return ns / 1000000000.0;
}

void display(double t, int n, float sum, const std::string& benchmark) {
    std::cout << "==================================================" << std::endl;
    std::cout << " Benchmark: " << benchmark << std::endl;
    std::cout << " Sum:       " << sum << std::endl;
    std::cout << " Time:      " << t << " s" << std::endl;
    std::cout << " Frequency: " << n / t / 1000 << " kHz" << std::endl;
    std::cout << " Channels:  " << (n / t) / 48000 << std::endl;
    std::cout << "==================================================" << std::endl;
}

int main(int argc, char const *argv[])
{
    int n = 100000000;        // one billion benchmark iterations
    volatile float sum = 0; // benchmark accumulator to trick compiler

    int bufferSize = 32;
    std::vector<float> sBuffer(bufferSize);
    std::vector<float> tBuffer(bufferSize);
  
    /////////////////////////////////////////////////////////////////////////

    // Signal sig = 0.5f * Saw(440) * ( 0.5f * Sine(120) + 0.5 * Square(100) ) 
    //                   * ADSR(1,1,1,1,1,0.5,Curves::Exponential::In(),Curves::Exponential::InOut(),Curves::Exponential::Out());

    // tic();
    // float lenN = sig.length() / n;
    // int v = 0;
    // for (int i = 0; i < n; ++i) {
    //     auto t = i * lenN;
    //     sum += sig.sample(t);
    //     v++;
    // }
    // std::cout << v << std::endl;
    // display(toc(), n, sum, "Single Sample");
   
    // sum = 0;
    
    // tic();
    // for (int i = 0; i < n / bufferSize; ++i) {
    //     for (int j = 0; j < bufferSize; ++ j)
    //         tBuffer[j] = (i*bufferSize + j) * lenN;
    //     sig.sample(&tBuffer[0], &sBuffer[0], bufferSize);
    //     for (const auto& smp : sBuffer)
    //         sum += smp;
    // }
    // std::cout << v << std::endl;
    // display(toc(), n, sum, "Multi Sample");

    // ///////////////////////////////////////////////////////////////////////////
    // sig = Signal();
    // for (int i = 0; i < 19; ++i)
    //     sig = sig * Scalar(5) * Envelope(1);    
    // lenN = sig.length() / n;
    // tic();
    // for (int i = 0; i < n; ++i) {
    //     auto t = i * lenN;
    //     sum += sig.sample(t);
    // }
    // display(toc(), n, sum, "Deep Signal Single");

    // /////////////////////////////////////////////////////////////////////////

    // tic();
    // for (int i = 0; i < n; ++i) {
    //     Signal x = Envelope(i) * Sine(i);
    // }
    // display(toc(), n, 0, "Allocations");

    return 0;
}
