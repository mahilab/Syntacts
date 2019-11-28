#include <syntacts>
#include <iostream>
#include <numeric>

using namespace tact;

void display(double t, int n, float sum, const std::string& benchmark) {
    std::cout << std::endl;
    std::cout << " Benchmark: " << benchmark << std::endl;
    std::cout << " Time:      " << t << " s" << std::endl;
    std::cout << " Frequency: " << n / t / 1000 << " kHz" << std::endl;
    std::cout << " Channels:  " << (n / t) / 48000 << std::endl;
    std::cout << " Sum:       " << sum << std::endl;
}

int main(int argc, char const *argv[])
{
    int n = 100000000;        // one billion benchmark iterations
    volatile float sum = 0; // benchmark accumulator to trick compiler

    int bufferSize = 32;
    std::vector<float> sBuffer(bufferSize);
    std::vector<float> tBuffer(bufferSize);

    auto env = Envelope();
  
    Signal sig = Sine(2 * PI * 175 * Time() + 2 * Sine(10)) * env;
    tic();
    float lenN = sig.length() / n;
    for (int i = 0; i < n; ++i) {
        auto t = i * lenN;
        sum += sig.sample(t);
    }
    display(toc(), n, sum, "Manual");

    sig = SineFM(175,10,2) * env;
    sum = 0;
    tic();
    for (int i = 0; i < n; ++i) {
        auto t = i * lenN;
        sum += sig.sample(t);
    }
    display(toc(), n, sum, "Auto");

    sig = Expression("sin(2*pi*175*t+2*sin(2*pi*10*t))") * env;
    sum = 0;
    tic();
    for (int i = 0; i < n; ++i) {
        auto t = i * lenN;
        sum += sig.sample(t);
    }
    display(toc(), n, sum, "Expression");
   
    sum = 0;

    sum = 0;
    tic();
    for (int i = 0; i < n; ++i) {
        auto t = i * lenN;
        sum += std::sin(TWO_PI * 175 * t + 2 * std::sin(2.0f * PI * 10 * t)) * env.sample(t);
    }
    display(toc(), n, sum, "Best Case");  

    ///////////////////////////////////////////////////////////////////////////

    sum = 0;
    tic();
    for (int i = 0; i < n; ++i) {
        sig = Sine(i) * Saw(i) * Envelope(5);
        auto t = i * lenN / n;
        sum += sig.sample(t);
    }
    display(toc(), n, sum, "Allocation");

    return 0;
}
