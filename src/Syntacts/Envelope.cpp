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
    Generator(),
    m_duration(std::abs(duration)),
    m_amplitude(clamp01(amplitude))
{

}

void BasicEnvelope::setDuration(float duration) {
    m_duration = std::abs(duration);
}

float BasicEnvelope::getDuration() const {
    return m_duration;
}

void BasicEnvelope::setAmplitude(float amplitude) {
    m_amplitude = clamp01(amplitude);
}

float BasicEnvelope::getAmplitude() const {
    return m_amplitude;
}

bool BasicEnvelope::playing() const {
    return getTime() < m_duration;
}

float BasicEnvelope::onSample(float t) {
    return playing() ? m_amplitude : 0.0f;
}

ASR::ASR(float attackTime, float sustainTime, float releaseTime, float amplitude, std::function<float(float, float, float)> tweenUp, std::function<float(float, float, float)> tweenDown) :
    BasicEnvelope(attackTime + sustainTime + releaseTime, amplitude),
    m_attackTime(attackTime), 
    m_sustainTime(sustainTime), 
    m_releaseTime(releaseTime),
    m_tweenUp(tweenUp),
    m_tweenDown(tweenDown)
{

}

float ASR::onSample(float t) {
    if (playing()) {
        if (t < m_attackTime) {
            float y = t / m_attackTime;
            return m_tweenUp(0.0f, m_amplitude, y);
        }
        else if (t > m_attackTime && t < (m_attackTime + m_sustainTime)) {
            return m_amplitude;
        }
        else if (t > (m_attackTime + m_sustainTime) && t < m_duration) {
            float y = (t - m_attackTime - m_sustainTime) / m_releaseTime;
            return m_tweenDown(m_amplitude, 0.0f, y);
        }
        else {
            return 0.0f;
        }
    }
    else 
        return 0.0f;        
}

ADSR::ADSR(float attackTime, float decayTime, float sustainTime, float releaseTime, float amplitude1, float amplitude2, std::function<float(float, float, float)> tweenUp, std::function<float(float, float, float)> tweenDown1, std::function<float(float, float, float)> tweenDown2) :
    ASR(attackTime, sustainTime, releaseTime, amplitude1, tweenUp, tweenDown1),
    m_decayTime(decayTime),
    m_amplitude2(amplitude2),
    m_tweenDown2(tweenDown2)
{
    setDuration(attackTime + decayTime + sustainTime + releaseTime);
}

float ADSR::onSample(float t) {
    if (playing()) {
        if (t < m_attackTime) {
            float y = t / m_attackTime;
            return m_tweenUp(0.0f, m_amplitude, y);
        }
        else if (t > m_attackTime && t < (m_attackTime + m_decayTime)) {
            float y = (t - m_attackTime) / m_decayTime;
            return m_tweenDown(m_amplitude, m_amplitude2, y);
        }
        else if (t > (m_attackTime + m_decayTime) && t < (m_attackTime + m_decayTime + m_sustainTime)) {
            return m_amplitude2;
        }
        else if (t > (m_attackTime + m_decayTime + m_sustainTime) && t < m_duration) {
            float y = (t - m_attackTime - m_decayTime - m_sustainTime) / m_releaseTime;
            return m_tweenDown2(m_amplitude2, 0.0f, y);
        }
        else
            return 0.0f;
    }
    else
        return 0.0f;
}

} // namespace tact