#include <Tact/Envelope.hpp>
#include <Tact/Oscillator.hpp>
#include <functional>

namespace tact {

Envelope::Envelope(double _duration, double _amplitude) :
    duration(_duration),
    amplitude(_amplitude)
{ }

double Envelope::sample(double t) const {
    return t > duration ? 0.0f : amplitude;
}

double Envelope::length() const {
    return duration;
}

KeyedEnvelope::KeyedEnvelope(double amplitude0)
{
   addKey(0.0f, amplitude0, Curves::Instant());
}

void KeyedEnvelope::addKey(double t, double amplitude, Curve curve) {
    keys[t] = std::make_pair(amplitude, curve);
}

double KeyedEnvelope::sample(double t) const {
    if (t > length())
        return 0.0f;
    auto b = keys.lower_bound(t);
    if (b->first == t)
        return b->second.first;
    auto a = std::prev(b);
    t = (t - a->first) / (b->first - a->first);
    double sample = b->second.second(a->second.first, b->second.first, t);    
    return sample;
}

double KeyedEnvelope::length() const {
    return keys.rbegin()->first;
}

ASR::ASR(double attackTime, double sustainTime, double releaseTime, double attackAmplitude, Curve attackCurve, Curve releaseCurve)
{
    addKey(attackTime, attackAmplitude, attackCurve);
    addKey(attackTime + sustainTime, attackAmplitude, Curves::Instant());
    addKey(attackTime + sustainTime + releaseTime, 0.0f, releaseCurve);
}


ADSR::ADSR(double attackTime, double decayTime, double sustainTime, double releaseTime, double attackAmplitude, double decayAmplitude, Curve attackCurve, Curve decayCurve, Curve releaseCurve)
{
    addKey(attackTime, attackAmplitude, attackCurve);
    addKey(attackTime + decayTime, decayAmplitude, decayCurve);
    addKey(attackTime + decayTime + sustainTime, decayAmplitude, Curves::Instant());
    addKey(attackTime + decayTime + sustainTime + releaseTime, 0.0f, releaseCurve);
}

ExponentialDecay::ExponentialDecay(double _amplitude, double _decay) : amplitude(_amplitude), decay(_decay) { }

double ExponentialDecay::sample(double t) const {
    return amplitude * std::exp(-decay * t);
}

double ExponentialDecay::length() const {
    return - std::log(0.001 /amplitude) / decay;
}

SignalEnvelope::SignalEnvelope(Signal _signal, double _duration , double _amplitude) :
    signal(_signal), duration(_duration), amplitude(_amplitude)
{ }

double SignalEnvelope::sample(double t) const {
    if (t > length())
        return 0.0f;
    double value = signal.sample(t);  
    value = remap(value, -1, 1 , 0 , amplitude);
    return value;
}

double SignalEnvelope::length() const {
    return duration;
}

} // namespace tact