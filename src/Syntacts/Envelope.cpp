#include <Syntacts/Envelope.hpp>
#include "Helpers.hpp"

namespace syntacts {

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

ASR::ASR(float attackTime, float sustainTime, float releaseTime, float amplitude) :
    Envelope(attackTime + sustainTime + releaseTime, amplitude),
    m_attackTime(attackTime), 
    m_sustainTime(sustainTime), 
    m_releaseTime(releaseTime)
{

}

float ASR::onSample(float t) {
    if (playing()) {
        if (t < m_attackTime)
            return interp(t, 0.0f, m_attackTime, 0.0f, m_amplitude);
        else if (t > m_attackTime && t < (m_attackTime + m_sustainTime))
            return m_amplitude;
        else if (t > (m_attackTime + m_sustainTime))
            return interp(t, (m_attackTime + m_sustainTime), (m_attackTime + m_sustainTime + m_releaseTime), m_amplitude, 0.0f);
        else if (t > m_duration)
            return 0.0f;
    }
    else 
        return 0.0f;        
}

ADSR::ADSR(float attackTime, float decayTime, float sustainTime, float releaseTime, float amplitude1, float amplitude2) :
    ASR(attackTime, sustainTime, releaseTime, amplitude1),
    m_decayTime(decayTime),
    m_amplitude2(amplitude2)
{
    setDuration(attackTime + decayTime + sustainTime + releaseTime);
}

float ADSR::onSample(float t) {
    if (playing()) {
        if (t < m_attackTime)
            return interp(t, 0.0f, m_attackTime, 0.0f, m_amplitude);
        else if (t > m_attackTime && t < (m_attackTime + m_decayTime))
            return interp(t, m_attackTime, m_attackTime + m_decayTime, m_amplitude, m_amplitude2);
        else if (t > (m_attackTime + m_decayTime) && t < (m_attackTime + m_decayTime + m_sustainTime))
            return m_amplitude2;
        else if (t > (m_attackTime + m_decayTime + m_sustainTime))
            return interp(t, (m_attackTime + m_decayTime + m_sustainTime), (m_attackTime + m_decayTime + m_sustainTime + m_releaseTime), m_amplitude2, 0.0f);
        else
            return 0.0f;
    }
    else
        return 0.0f;
}

} // namespace syntacts