#include <Syntacts/Syntacts.hpp>
#include <chrono>
#include <thread>

using namespace tact;

class SW : public Source {

    float sample(float t) const override {
        return std::sin(6.28318530717959e+0f * 440 * t);
    }
};

int main(int argc, char const *argv[])
{
    tact::initialize(4);

    // auto rmp = create<Ramp>(175.0f, 250.0f, 3.0f);
    // auto tri = create<SineWave>(rmp);
    // auto sqr  = create<SineWave>(25.0f);
    // auto env = create<ADSR>(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.5f, create<Tween::Smootheststep>(), create<Tween::Exponential::Out>(), create<Tween::Sinusoidal::In>());
    // auto cue  = create<Cue>(tri, sqr, env);
    // save(cue, "myCue.json", SerialFormat::JSON);
    // save(cue, "myCue.tact", SerialFormat::Binary);

    // Ptr<Cue> cueLoaded;
    // load(cueLoaded, "myCue.tact", SerialFormat::Binary);

    // int iterations = 10'000'000;
    // float sum = 0;
    // auto start = std::chrono::high_resolution_clock::now();
    // for (int i = 0; i < iterations; ++i) {
    //     float t = (float)i/(float)iterations;
    //     sum += cue->sample(t) - cueLoaded->sample(t);
    // }
    // auto end = std::chrono::high_resolution_clock::now();

    // std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;

    // std::cout << sum << std::endl;

    auto frq = create<Scalar>(440);
    auto osc = create<SineWave>(frq);
    auto mod = create<SineWave>(10);
    auto env = create<ASR>(1,18,1);
    auto cue = create<Cue>();
    cue->setEnvelope(create<AmplitudeEnvelope>(10,1));
    cue->chain(create<SineWave>(440));

    tact::play(0, cue);

    auto start = std::chrono::high_resolution_clock::now();
    float t = 0;
    while (t < 10) {        
        // auto elapsed = std::chrono::high_resolution_clock::now() - start;
        // t = (float)(std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count() * 1000);
    }

    return 0;
}
