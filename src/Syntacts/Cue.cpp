#include <Syntacts/Syntacts.hpp>
#include "Helpers.hpp"

namespace tact {

Cue::Cue() :
    m_generators(0)
{

}

Cue::Cue(std::shared_ptr<Oscillator> osc, float duration, float amplitude) :
    m_env(std::move(std::make_shared<BasicEnvelope>(duration, amplitude)))
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

std::shared_ptr<Envelope> Cue::getEnvelope() const {
    return m_env;
}

void Cue::chain(std::shared_ptr<Source> gen) {
    m_generators.push_back(std::move(gen));
}

float Cue::sample(float t) {
    if (m_generators.empty())
        return 0.0f;
    float compoundSample = 1.0f;
    for (auto& g : m_generators)
        compoundSample *= g->sample(t);
    compoundSample *= m_env->sample(t);
    return compoundSample;
}

int Cue::sampleCount(int sampleRate) {
    return (int)(m_env->getDuration() * sampleRate);
}

};