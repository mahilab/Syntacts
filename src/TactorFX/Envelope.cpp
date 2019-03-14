#include <TactorFX/Envelope.hpp>

namespace tfx {

namespace {
inline float interp(float x, float x0, float x1, float y0, float y1) {
    return y0 + (x - x0) * (y1 - y0) / (x1 - x0);
}
}

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
    if (playing()) {
        if (t < m_attackTime)
            return interp(t,0,m_attackTime,0.0f,1.0f);
        else if (t > (m_attackTime + m_sustainTime))
            return interp(t, (m_attackTime + m_sustainTime), (m_attackTime + m_sustainTime + m_releaseTime), 1.0f, 0.0f);
        else if (t > m_duration)
            return 0.0f;
    }
    else 
        return 0.0f;      
    // return playing() ? 1.0f / (1.0f + std::exp(10.0f/m_attackTime * (t-m_attackTime/2)) * 1.0f/(1.0f+std::exp(10.0f / (m_duration - (m_attackTime + m_sustainTime))) * (t - ((m_duration - (m_attackTime + m_sustainTime)) / 2.0f)-(m_attackTime+m_sustainTime)))) : 0.0f;
}
// (1.0f - std::exp(-3.0f * t / m_attackTime))
} // namespae tfx