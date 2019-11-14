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