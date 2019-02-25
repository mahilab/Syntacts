#include <TactorFX/Cue.hpp>

namespace tfx {

Cue::Cue() :
    m_generators(0)
{

}

Cue::Cue(Ptr<Oscillator> osc, float duration) {
    addGenerator(std::move(osc));
    addGenerator(std::move(std::make_shared<Envelope>(duration)));
}

Cue::Cue(Ptr<Oscillator> osc, Ptr<Envelope> env) {
    addGenerator(std::move(osc));
    addGenerator(std::move(env));
}

Cue::Cue(Ptr<Oscillator> osc, Ptr<Oscillator> mod, Ptr<Envelope> env) {
    addGenerator(std::move(osc));
    addGenerator(std::move(mod));
    addGenerator(std::move(env));
}

Cue::~Cue() {

}

void Cue::addGenerator(Ptr<Generator> gen) {
    m_generators.push_back(std::move(gen));
}

float Cue::nextSample() {
    float sample = 1.0f;
    for (auto& g : m_generators)
        sample *= g->nextSample();
    return sample;
}

};