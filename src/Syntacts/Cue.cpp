#include <Syntacts/Syntacts.hpp>
#include "Util/Util.hpp"

namespace tact {

Cue::Cue() :
    m_sigs(0),
    m_env(move(create<Envelope>()))
{

}

Cue::Cue(Ptr<OscillatorBase> osc, float duration, float amplitude) :
    m_env(move(create<Envelope>(duration, amplitude)))
{
    chain(move(osc));
}

Cue::Cue(Ptr<OscillatorBase> osc, Ptr<EnvelopeBase> env) :
    m_env(move(env))
{
    chain(move(osc));
}

Cue::Cue(Ptr<OscillatorBase> osc, Ptr<OscillatorBase> mod, Ptr<EnvelopeBase> env) :
    m_env(move(env))
{
    chain(move(osc));
    chain(move(mod));
}

Cue::~Cue() {

}

void Cue::setEnvelope(Ptr<EnvelopeBase> env) {
    m_env = env;
}

Ptr<EnvelopeBase> Cue::getEnvelope() const {
    return m_env;
}

void Cue::chain(Ptr<SignalBase> gen) {
    m_sigs.push_back(move(gen));
}

float Cue::sample(float t) const {
    if (m_sigs.empty())
        return 0.0f;
    float compoundSample = 1.0f;
    for (auto& g : m_sigs)
        compoundSample *= g->sample(t);
    compoundSample *= m_env->sample(t);
    return compoundSample;
}

int Cue::sampleCount(int sampleRate) const {
    return (int)(m_env->getDuration() * sampleRate);
}

};