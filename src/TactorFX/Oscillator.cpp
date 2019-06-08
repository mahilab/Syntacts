#include <TactorFX/Oscillator.hpp>
#include "Helpers.hpp"
#include <cmath>

namespace tfx
{

Oscillator::Oscillator(float frequency) :
    Generator(),
    m_frequency(frequency)
{

}

void Oscillator::setFrequency(float frequency) {
    m_frequency = frequency;
}       

float SineWave::onSample(float t) {
    return std::sin(TWO_PI * m_frequency * t);
}

float SquareWave::onSample(float t) {
    return std::sin(TWO_PI * m_frequency * t) > 0 ? 1.0f : -1.0f;
}

float SawWave::onSample(float t) {
    return -2.0f / PI * std::atan(std::cos(PI * m_frequency * t) / std::sin(PI * m_frequency * t));
}

float TriWave::onSample(float t) {
    return -2.0f / PI * std::asin(std::sin(2.0f * PI * m_frequency * t));
}

} // namespace tfx