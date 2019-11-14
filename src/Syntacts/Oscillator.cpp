#include <Syntacts/Oscillator.hpp>
#include "Helpers.hpp"
#include <cmath>

namespace tact
{

Oscillator::Oscillator(float frequency) :
    Oscillator(create<Scalar>(frequency))
{

}

Oscillator::Oscillator(Ptr<Signal> frequency) :
    Signal(),
    m_frequency(frequency)
{

}

float Sine::sample(float t) const {
    return std::sin(TWO_PI * m_frequency->sample(t) * t);
}

float Square::sample(float t) const {
    return std::sin(TWO_PI * m_frequency->sample(t) * t) > 0 ? 1.0f : -1.0f;
}

float Saw::sample(float t) const {
    float f = m_frequency->sample(t);
    return -2.0f / PI * std::atan(std::cos(PI * f * t) / std::sin(PI * f * t));
}

float Triangle::sample(float t) const {
    return -2.0f / PI * std::asin(std::sin(2.0f * PI * m_frequency->sample(t) * t));
}

SineFM::SineFM(float frequency, float modulation, float index) :
    Oscillator(frequency),
    m_modulation(modulation),
    m_index(index) 
{

}

void SineFM::setModulation(float modulation) {
    m_modulation = modulation;
}

void SineFM::setIndex(float index) {
    m_index = index;
}

float SineFM::sample(float t) const {
    return std::cos(2.0f * PI * m_frequency->sample(t) * t + m_index * std::sin(2.0f * PI * m_modulation * t));
}

Chirp::Chirp(float frequency, float chirpyness) :
    Oscillator(frequency),
    m_chirpyness(chirpyness)
{

}

void Chirp::setChirpyness(float chirpyness) {
    m_chirpyness = chirpyness;
}

float Chirp::sample(float t) const {
    float phi = 2 * PI * (m_frequency->sample(t) + 0.5f * m_chirpyness * t) * t;
    return std::sin(phi);
}

PulseTrain::PulseTrain(float frequency, float dutyCycle) :
    Oscillator(frequency), 
    m_dutyCycle(clamp01(dutyCycle)),
    m_period(1.0f / frequency)
{

}

void PulseTrain::setDutyCycle(float dutyCycle) {
    m_dutyCycle = clamp01(dutyCycle);
}

float PulseTrain::sample(float t) const {
    float percentWithinPeriod = std::fmod(t, m_period) * m_frequency->sample(t);
    if (percentWithinPeriod < m_dutyCycle) 
        return 1;
    else 
        return -1;
}

} // namespace tact