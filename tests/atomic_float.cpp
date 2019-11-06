#include <atomic>
#include <iostream>
#include <vector>
#include <thread>
#include <carnot>
#include <Syntacts/Syntacts.hpp>

using namespace tact;

class SW : public Source {

    float sample(float t) const override {
        return (float)std::sin(6.28318530717959 * 880 * (double)t);
    }
};

int main(int argc, char const *argv[])
{
    tact::initialize();
    auto frq = create<Scalar>(440);
    auto osc = create<SineWave>(frq);
    auto mod = create<SineWave>(5);
    auto env = create<ASR>(0.001,5,0.001);
    auto cue = create<Cue>();
    cue->setEnvelope(create<AmplitudeEnvelope>(5,1));
    cue->chain(create<SW>());

    tact::play(0, cue);
    exportToWave(cue, "cue.wav");

    carnot::Clock clock;
    while (clock.getElapsedTime().asSeconds() < cue->getEnvelope()->getDuration()) {

    }
    tact::finalize();
    return 0;
}
