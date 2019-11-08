#include <atomic>
#include <iostream>
#include <vector>
#include <thread>
#include <carnot>
#include <Syntacts/Syntacts.hpp>
#include <fstream>
#include <Syntacts/Session.hpp>

using namespace tact;

class Amp : public Envelope {
public:
    using Envelope::Envelope;
    float sample(float t) const override {
        std::lock_guard<std::mutex> lock(mtx);
        float temp = value;
        locks++;
        return temp;
    }

    void set(float newValue) {
        std::lock_guard<std::mutex> lock(mtx);
        value = newValue;
        locks++;
    }

    float value = 0.0f;
    mutable std::mutex mtx;
    mutable int locks = 0;
};

template <typename T>
int exportToCsv(T& sampler, std::string filePath, int sampleRate = 44100) {
    std::ofstream s;
    s.open(filePath);
    int n = 352800;
    std::cout << n << std::endl;
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

    auto frq = create<Ramp>(440,220,5);
    auto osc = create<SineWave>(frq);
    auto mod = create<SineWave>(20);
    auto env = create<ASR>(1,3,1);
    auto cue = create<Cue>(osc, mod, env);
    cue->chain<SineWave>(10);

    session.play(0, cue);
    session.play(1, cue);    
    session.play(2, cue);
    session.play(3, cue);
    session.play(4, cue);
    session.play(5, cue);
    session.play(6, cue);
    session.play(7, cue);
    carnot::Clock clk;
    float t = 0;
    while (t < 1.0f) {
        session.setVolume(0, 1.0f - t);
        session.setVolume(1, t);
        t = clk.getElapsedTime().asSeconds() / env->getDuration();
        std::cout << session.getCpuLoad() << std::endl;
        carnot::sleep(carnot::milliseconds(10));
    }
    carnot::sleep(carnot::seconds(1));
    return 0;
}