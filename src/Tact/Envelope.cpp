#include <Tact/Envelope.hpp>
#include <Tact/Oscillator.hpp>
#include <functional>

namespace tact {

Envelope::Envelope(float _duration, float _amplitude) :
    duration(_duration),
    amplitude(_amplitude)
{ }

float Envelope::sample(float t) const {
    return t > duration ? 0.0f : amplitude;
}

float Envelope::length() const {
    return duration;
}

KeyedEnvelope::KeyedEnvelope(float amplitude0)
{
   addKey(0.0f, amplitude0, Curves::Linear());
}

void KeyedEnvelope::addKey(float t, float amplitude, Curve curve) {
    keys[t] = std::make_pair(amplitude, curve);
}

float KeyedEnvelope::sample(float t) const {
    if (t > length())
        return 0.0f;
    auto b = keys.lower_bound(t);
    if (b->first == t)
        return b->second.first;
    auto a = std::prev(b);
    t = (t - a->first) / (b->first - a->first);
    float sample = b->second.second(a->second.first, b->second.first, t);    
    return sample;
}

float KeyedEnvelope::length() const {
    return keys.rbegin()->first;
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

SignalEnvelope::SignalEnvelope(Signal _signal, float _duration , float _amplitude) :
    signal(_signal), duration(_duration), amplitude(_amplitude)
{ }

float SignalEnvelope::sample(float t) const {
    if (t > length())
        return 0.0f;
    float value = signal.sample(t);  
    value = remap(value, -1, 1 , 0 , amplitude);
    return value;
}

float SignalEnvelope::length() const {
    return duration;
}

} // namespace tact