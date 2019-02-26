#include <TactorFX/Envelope.hpp>

namespace tfx {

Envelope::Envelope(float duration) :
    Generator(),
    m_duration(duration)
{

}

void Envelope::setDuration(float duration) {
    m_duration = duration;
}

bool Envelope::playing() const {
    return getTime() < m_duration;
}

float Envelope::onSample(float t) {
    return playing() ? 1.0f : 0.0f;
}

ASR::ASR(float attackTime, float sustainTime, float releaseTime) :
    Envelope(attackTime + sustainTime + releaseTime),
    m_attackTime(attackTime), m_sustainTime(sustainTime), m_releaseTime(releaseTime)
{

}

float ASR::onSample(float t) {
    return playing() ? (1.0f - std::exp(-3.0f * t / m_attackTime)) * 1/(1+std::exp(10 / (m_duration - (m_attackTime + m_sustainTime)) * (t - ((m_duration - (m_attackTime + m_sustainTime)) / 2)-(m_attackTime+m_sustainTime)))) : 0.0f;
}

} // namespae tfx