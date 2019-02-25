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

AttackSustain::AttackSustain(float attackTime, float sustainTime) :
    Envelope(attackTime + sustainTime),
    m_attackTime(attackTime)
{

}

float AttackSustain::onSample(float t) {
    return playing() ? (1.0f - std::exp(-3.0f * t / m_attackTime)) : 0.0f;
}

} // namespae tfx