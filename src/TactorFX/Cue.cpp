#include <TactorFX/Cue.hpp>

namespace tfx {

Cue::Cue() :
    m_generators(0)
{

}

Cue::Cue(Ptr<Oscillator> osc, float duration) {
    chain(std::move(osc));
    chain(std::move(std::make_shared<Envelope>(duration)));
}

Cue::Cue(Ptr<Oscillator> osc, Ptr<Envelope> env) {
    chain(std::move(osc));
    chain(std::move(env));
}

Cue::Cue(Ptr<Oscillator> osc, Ptr<Oscillator> mod, Ptr<Envelope> env) {
    chain(std::move(osc));
    chain(std::move(mod));
    chain(std::move(env));
}

Cue::~Cue() {

}

void Cue::chain(Ptr<Generator> gen) {
    m_generators.push_back(std::move(gen));
}

float Cue::nextSample() {
    float sample = 1.0f;
    for (auto& g : m_generators)
        sample *= g->nextSample();
    return sample;
}

};