#include <Syntacts/Envelope.hpp>
#include <Syntacts/Tween.hpp>
#include <Syntacts/Oscillator.hpp>
#include "Helpers.hpp"
#include <functional>

namespace tact {

Envelope::Envelope(float duration) :
    Generator(), m_duration(duration)
{
    
}

float Envelope::getDuration() const {
    return m_duration;
}

KeyedEnvelope::KeyedEnvelope(float amplitude0) :
    Envelope()
{
   addKey(0.0f, amplitude0);
}

void KeyedEnvelope::addKey(float t, float amplitude, TweenFunc tween) {
    m_keys[t] = std::make_pair(amplitude, tween);
}

float KeyedEnvelope::getDuration() const {
    return m_keys.rbegin()->first;
}

float KeyedEnvelope::sample(float t) {
    if (t > getDuration())
        return 0.0f;
    auto b = m_keys.lower_bound(t);
    if (b->first == t)
        return b->second.first;
    auto a = std::prev(b);
    t = (t - a->first) / (b->first - a->first);
    return b->second.second(a->second.first, b->second.first, t);    
}


BasicEnvelope::BasicEnvelope(float duration, float amplitude) :
    KeyedEnvelope(amplitude)
{
    addKey(duration, amplitude, Tween::Instant);
}

float BasicEnvelope::getAmplitude() const {
    return m_keys.begin()->second.first;
}


ASR::ASR(float attackTime, float sustainTime, float releaseTime, float attackAmplitude, std::function<float(float, float, float)> attackTween, std::function<float(float, float, float)> releaseTween) :
    KeyedEnvelope(0.0f)
{
    addKey(attackTime, attackAmplitude, attackTween);
    addKey(attackTime + sustainTime, attackAmplitude, Tween::Instant);
    addKey(attackTime + sustainTime + releaseTime, 0.0f, releaseTween);
}


ADSR::ADSR(float attackTime, float decayTime, float sustainTime, float releaseTime, float attackAmplitude, float decayAmplitude, std::function<float(float, float, float)> attackTween, std::function<float(float, float, float)> decayTween, std::function<float(float, float, float)> releaseTween) :
    KeyedEnvelope(0.0f)
{
    addKey(attackTime, attackAmplitude, attackTween);
    addKey(attackTime + decayTime, decayAmplitude, decayTween);
    addKey(attackTime + decayTime + sustainTime, decayAmplitude, Tween::Instant);
    addKey(attackTime + decayTime + sustainTime + releaseTime, 0.0f, releaseTween);
}

OscillatingEnvelope::OscillatingEnvelope(float duration , float amplitude , std::shared_ptr<Oscillator> osc) :
    Envelope(duration), m_amplitude(amplitude), m_osc(std::move(osc))
{
    
}

float OscillatingEnvelope::sample(float t) {
    if (t > getDuration())
        return 0.0f;
    float value = m_osc->sample(t);  
    value = interp(value, -1, 1 , 0 , m_amplitude);
    return value;
}

} // namespace tact