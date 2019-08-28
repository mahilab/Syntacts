#include <Syntacts/Oscillator.hpp>
#include "Helpers.hpp"
#include <cmath>

namespace tact
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

SineWaveFM::SineWaveFM(float frequency, float modulation, float index) :
    Oscillator(frequency),
    m_modulation(modulation),
    m_index(index) 
{

}

float SineWaveFM::onSample(float t) {
    return std::cos(2.0f * PI * m_frequency * t + m_index * std::sin(2.0f * PI * m_modulation * t));
}

PulseTrain::PulseTrain(float frequency, float dutyCycle) :
    Oscillator(frequency), 
    m_dutyCycle(dutyCycle),
    m_period(1.0f / frequency)
{

}

float PulseTrain::onSample(float t) {
    float percentWithinPeriod = std::fmod(t, m_period) / m_period;
    if (percentWithinPeriod < m_dutyCycle ) 
        return 1;
    else 
        return -1;
}

} // namespace tact