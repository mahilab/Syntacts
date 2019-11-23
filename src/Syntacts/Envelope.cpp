#include <Syntacts/Envelope.hpp>
#include <Syntacts/Oscillator.hpp>
#include <functional>

namespace tact {

Envelope::Envelope(float duration, float _amplitude) :
    amplitude(_amplitude)
{ 
    m_length = duration;
}

float Envelope::sample(float t) const {
    return t > m_length ? 0.0f : amplitude;
}

KeyedEnvelope::KeyedEnvelope(float amplitude0)
{
   addKey(0.0f, amplitude0, Curves::Linear());
}

void KeyedEnvelope::addKey(float t, float amplitude, Curve curve) {
    m_keys[t] = std::make_pair(amplitude, curve);
    m_length = m_keys.rbegin()->first;
}

float KeyedEnvelope::sample(float t) const {
    if (t > length())
        return 0.0f;
    auto b = m_keys.lower_bound(t);
    if (b->first == t)
        return b->second.first;
    auto a = std::prev(b);
    t = (t - a->first) / (b->first - a->first);
    return b->second.second(a->second.first, b->second.first, t);    
}

ASR::ASR(float attackTime, float sustainTime, float releaseTime, float attackAmplitude, Curve attackCurve, Curve releaseCurve)
{
    addKey(attackTime, attackAmplitude, attackCurve);
    addKey(attackTime + sustainTime, attackAmplitude, Curves::Instant());
    addKey(attackTime + sustainTime + releaseTime, 0.0f, releaseCurve);
}


ADSR::ADSR(float attackTime, float decayTime, float sustainTime, float releaseTime, float attackAmplitude, float decayAmplitude, Curve attackCurve, Curve decayCurve, Curve releaseCurve)
{
    addKey(attackTime, attackAmplitude, attackCurve);
    addKey(attackTime + decayTime, decayAmplitude, decayCurve);
    addKey(attackTime + decayTime + sustainTime, decayAmplitude, Curves::Instant());
    addKey(attackTime + decayTime + sustainTime + releaseTime, 0.0f, releaseCurve);
}

OscillatingEnvelope::OscillatingEnvelope(float duration , float amplitude , Ptr<IOscillator> osc) :
    Cloneable(duration, amplitude), m_osc(std::move(osc))
{
    
}

float OscillatingEnvelope::sample(float t) const {
    if (t > length())
        return 0.0f;
    float value = m_osc->sample(t);  
    value = remap(value, -1, 1 , 0 , amplitude);
    return value;
}

} // namespace tact