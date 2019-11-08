#include <Syntacts/Source.hpp>

namespace tact
{

Scalar::Scalar(float value) : m_value(value) {}
float Scalar::sample(float t) const { return m_value; }
void Scalar::setValue(float value) { m_value = value; }

Ramp::Ramp(float initial, float rate) : m_initial(initial), m_rate(rate) {}
Ramp::Ramp(float initial, float final, float span) : m_initial(initial), m_rate((final - initial) / span) {}
float Ramp::sample(float t) const { return m_initial + m_rate * t; }

} // namespace tact