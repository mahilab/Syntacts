#include <TactorFX/Oscillator.hpp>
#include "Helpers.hpp"
#include <cmath>

namespace tfx
{

Oscillator::Oscillator(float frequency, float amplitude) :
    Generator(),
    m_frequency(frequency),
    m_amplitude(amplitude)
{

}

void Oscillator::setFrequency(float frequency) {
    m_frequency = frequency;
}       

void Oscillator::setAmplitude(float amplitude) {
    m_amplitude = clamp01(amplitude);
}

float SineWave::onSample(float t) {
    return m_amplitude * std::sin(TWO_PI * m_frequency * t);
}

float SquareWave::onSample(float t) {
    return std::sin(TWO_PI * m_frequency * t) > 0 ? m_amplitude : -m_amplitude;
}

float SawWave::onSample(float t) {
    return -2.0f / PI * m_amplitude * std::atan(std::cos(PI * m_frequency * t) / std::sin(PI * m_frequency * t));
}

} // namespace tfx