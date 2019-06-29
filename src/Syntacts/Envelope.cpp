#include <Syntacts/Envelope.hpp>
#include <Syntacts/Tween.hpp>
#include "Helpers.hpp"
#include <functional>

namespace tact {

Envelope::Envelope(float amplitude0) :
    Generator()
{
   addKey(0.0f, amplitude0);
}

void Envelope::addKey(float t, float amplitude, TweenFunc tween) {
    m_keys[t] = std::make_pair(amplitude, tween);
}

float Envelope::getDuration() const {
    return m_keys.rbegin()->first;
}

float Envelope::onSample(float t) {
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
    Envelope(amplitude)
{
    addKey(duration, amplitude, Tween::Instant);
}

float BasicEnvelope::getAmplitude() const {
    return m_keys.begin()->second.first;
}


ASR::ASR(float attackTime, float sustainTime, float releaseTime, float attackAmplitude, std::function<float(float, float, float)> attackTween, std::function<float(float, float, float)> releaseTween) :
    Envelope(0.0f)
{
    addKey(attackTime, attackAmplitude, attackTween);
    addKey(attackTime + sustainTime, attackAmplitude, Tween::Instant);
    addKey(attackTime + sustainTime + releaseTime, 0.0f, releaseTween);
}


ADSR::ADSR(float attackTime, float decayTime, float sustainTime, float releaseTime, float attackAmplitude, float decayAmplitude, std::function<float(float, float, float)> attackTween, std::function<float(float, float, float)> decayTween, std::function<float(float, float, float)> releaseTween) :
    Envelope(0.0f)
{
    addKey(attackTime, attackAmplitude, attackTween);
    addKey(attackTime + decayTime, decayAmplitude, decayTween);
    addKey(attackTime + decayTime + sustainTime, decayAmplitude, Tween::Instant);
    addKey(attackTime + decayTime + sustainTime + releaseTime, 0.0f, releaseTween);
}



} // namespace tact