#include <atomic>
#include <iostream>
#include <vector>
#include <thread>
#include <carnot>
#include <Syntacts/Syntacts.hpp>
#include <fstream>
#include <Syntacts/Session.hpp>

using namespace tact;

class Amp : public Oscillator {
public:
    using Oscillator::Oscillator;
    float sample(float t) const override {
        static double fI = 1000;
        static double fF = 200;
        static double d  = 5;
        double f = fI + 0.5f*(fF-fI)/d * t;
        return std::sin(6.28318530717959 * f * t);
    }
    SERIALIZE(PARENT(Oscillator));
};

template <typename T>
int exportToCsv(const T& sampler, std::string filePath, int sampleRate = 44100) {
    std::ofstream s;
    s.open(filePath);
    int n = 220500;
    double sampleLength = 1.0 / sampleRate;
    double t = 0;
    for (std::size_t i = 0; i < n; ++i) {
        float sample = sampler->sample(static_cast<float>(t));
        s << sample << std::endl;
        t += sampleLength;
    }
    s.close();
    return 0;
}

int main(int argc, char const *argv[])
{
    Session session;
    session.open();

    std::cout << session.getCurrentDevice().name << std::endl;

    auto frq = create<Ramp>(2200,220,5);
    auto osc = create<SineWave>(frq);
    auto mod = create<SineWave>(20);
    auto env = create<Envelope>(5);
    // auto cue = create<Cue>(osc, env);
    // auto cue = create<Cue>(create<Chirp>(1000, (200.f-1000.f)/5.f), 5);
    auto cue = create<Cue>(create<Amp>(),5);
    // auto cue = create<Chirp>()
    // cue->chain<SineWave>(5);

    // save(cue,"aWeirdOne");

    exportToCsv(cue, "cue.csv");
    exportToWave(cue, "hmm.wav");

    session.play(0, cue);
    // session.play(1, cue);    
    carnot::Clock clk;
    float t = 0;
    while (t < 1.0f) {
        // session.setVolume(0, 1.0f - t);
        // session.setVolume(1, t);
        t = clk.getElapsedTime().asSeconds() / env->getDuration();
        // std::cout << frq->sample(clk.getElapsedTime().asSeconds()) << std::endl;
        // carnot::sleep(carnot::milliseconds(10));
    }
    carnot::sleep(carnot::seconds(1));
    return 0;
}