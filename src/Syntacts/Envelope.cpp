#include <Syntacts/Envelope.hpp>
#include <Syntacts/Tween.hpp>
#include "Helpers.hpp"
#include <functional>

namespace tact {

Envelope::Envelope(float duration, float amplitude) :
    Generator(),
    m_duration(std::abs(duration)),
    m_amplitude(clamp01(amplitude))
{

}

void Envelope::setDuration(float duration) {
    m_duration = std::abs(duration);
}

float Envelope::getDuration() const {
    return m_duration;
}

void Envelope::setAmplitude(float amplitude) {
    m_amplitude = clamp01(amplitude);
}

float Envelope::getAmplitude() const {
    return m_amplitude;
}

bool Envelope::playing() const {
    return getTime() < m_duration;
}

float Envelope::onSample(float t) {
    return playing() ? m_amplitude : 0.0f;
}

ASR::ASR(float attackTime, float sustainTime, float releaseTime, float amplitude, std::function<float(float, float, float)> tweenUp, std::function<float(float, float, float)> tweenDown) :
    Envelope(attackTime + sustainTime + releaseTime, amplitude),
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