#include <Syntacts/Syntacts.hpp>
#include <chrono>

using namespace tact;

int main(int argc, char const *argv[])
{
    auto rmp = create<Ramp>(175.0f, 250.0f, 3.0f);
    auto tri = create<SineWave>(rmp);
    auto sqr  = create<SineWave>(25.0f);
    auto env = create<ADSR>(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.5f, create<Tween::Smootheststep>(), create<Tween::Exponential::Out>(), create<Tween::Sinusoidal::In>());
    auto cue  = create<Cue>(tri, sqr, env);
    save(cue, "myCue.json", SerialFormat::JSON);
    save(cue, "myCue.tact", SerialFormat::Binary);

    Ptr<Cue> cueLoaded;
    load(cueLoaded, "myCue.tact", SerialFormat::Binary);

    int iterations = 10'000'000;
    float sum = 0;
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        float t = (float)i/(float)iterations;
        sum += cue->sample(t) - cueLoaded->sample(t);
    }
    auto end = std::chrono::high_resolution_clock::now();

    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;

    std::cout << sum << std::endl;

    return 0;
}
