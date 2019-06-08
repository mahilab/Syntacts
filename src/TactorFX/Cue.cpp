#include <TactorFX/Cue.hpp>

namespace tfx {

Cue::Cue() :
    m_generators(0)
{

}

Cue::Cue(std::shared_ptr<Oscillator> osc, float duration, float amplitude) {
    chain(std::move(osc));
    chain(std::move(std::make_shared<Envelope>(duration, amplitude)));
}

Cue::Cue(std::shared_ptr<Oscillator> osc, std::shared_ptr<Envelope> env) {
    chain(std::move(osc));
    chain(std::move(env));
}

Cue::Cue(std::shared_ptr<Oscillator> osc, std::shared_ptr<Oscillator> mod, std::shared_ptr<Envelope> env) {
    chain(std::move(osc));
    chain(std::move(mod));
    chain(std::move(env));
}

Cue::~Cue() {

}

void Cue::chain(std::shared_ptr<Generator> gen) {
    m_generators.push_back(std::move(gen));
}

float Cue::nextSample() {
    if (m_generators.empty())
        return 0.0f;
    float sample = 1.0f;
    for (auto& g : m_generators)
        sample *= g->nextSample();
    return sample;
}

};