#include <Syntacts/Cue.hpp>
#include "Helpers.hpp"

namespace syntacts {

Cue::Cue() :
    m_generators(0)
{

}

Cue::Cue(std::shared_ptr<Oscillator> osc, float duration, float amplitude) :
    m_env(std::move(std::make_shared<Envelope>(duration, amplitude)))
{
    chain(std::move(osc));
}

Cue::Cue(std::shared_ptr<Oscillator> osc, std::shared_ptr<Envelope> env) :
    m_env(std::move(env))
{
    chain(std::move(osc));
}

Cue::Cue(std::shared_ptr<Oscillator> osc, std::shared_ptr<Oscillator> mod, std::shared_ptr<Envelope> env) :
    m_env(std::move(env))
{
    chain(std::move(osc));
    chain(std::move(mod));
}

Cue::~Cue() {

}

void Cue::setEnvelope(std::shared_ptr<Envelope> env) {
    m_env = env;
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
    sample *= m_env->nextSample();
    return sample;
}

int Cue::sampleCount() {
    return (int)(m_env->getDuration() * SAMPLE_RATE);
}

};