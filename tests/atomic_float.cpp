#include <atomic>
#include <iostream>
#include <vector>
#include <thread>
#include <carnot>
#include <Syntacts/Syntacts.hpp>
#include <fstream>

using namespace tact;

template <typename T>
int exportToCsv(T& sampler, std::string filePath, int sampleRate = 44100) {
    std::ofstream s;
    s.open(filePath);
    int n = 352800;
    std::cout << n << std::endl;
    float sampleLength = 1.0f / sampleRate;
    float t = 0;
    for (std::size_t i = 0; i < n; ++i) {
        float sample = sampler->sample(t);
        s << sample << std::endl;
        t += sampleLength;
    }
    s.close();
    return 0;
}

class SW : public Source {
public:
    float sample(float t) const override {
        return sin(6.28318530717959f * 880 * t);
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

    auto benv = create<AmplitudeEnvelope>(8,0.5);
    cue->setEnvelope(benv);
    auto sw = create<SW>();
    cue->chain(sw);

    tact::play(0, cue);
    exportToWave(cue, "cue.wav");
    exportToCsv(cue, "cue.csv");

    carnot::Clock clock;
    while (clock.getElapsedTime().asSeconds() < cue->getEnvelope()->getDuration()) {

    }
    tact::finalize();
    return 0;
}
