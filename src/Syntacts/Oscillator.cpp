#include <Syntacts/Oscillator.hpp>
#include "Helpers.hpp"
#include <cmath>

namespace tact
{

Oscillator::Oscillator(float frequency) :
    Generator(),
    m_frequency(std::abs(frequency))
{

}

void Oscillator::setFrequency(float frequency) {
    m_frequency = std::abs(frequency);
}       

float SineWave::sample(float t) {
    return std::sin(TWO_PI * m_frequency * t);
}

float SquareWave::sample(float t) {
    return std::sin(TWO_PI * m_frequency * t) > 0 ? 1.0f : -1.0f;
}

float SawWave::sample(float t) {
    return -2.0f / PI * std::atan(std::cos(PI * m_frequency * t) / std::sin(PI * m_frequency * t));
}

float TriWave::sample(float t) {
    return -2.0f / PI * std::asin(std::sin(2.0f * PI * m_frequency * t));
}

SineWaveFM::SineWaveFM(float frequency, float modulation, float index) :
    Oscillator(frequency),
    m_modulation(modulation),
    m_index(index) 
{

}

void SineWaveFM::setModulation(float modulation) {
    m_modulation = modulation;
}

void SineWaveFM::setIndex(float index) {
    m_index = index;
}

float SineWaveFM::sample(float t) {
    return std::cos(2.0f * PI * m_frequency * t + m_index * std::sin(2.0f * PI * m_modulation * t));
}

Chirp::Chirp(float frequency, float chirpyness) :
    Oscillator(frequency),
    m_chirpyness(chirpyness)
{

}

void Chirp::setChirpyness(float chirpyness) {
    m_chirpyness = chirpyness;
}

float Chirp::sample(float t) {
    float phi = 2 * PI * ((m_frequency + 0.5f * m_chirpyness * t) * t);
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

float PulseTrain::sample(float t) {
    float percentWithinPeriod = std::fmod(t, m_period) * m_frequency;
    if (percentWithinPeriod < m_dutyCycle) 
        return 1;
    else 
        return -1;
}

} // namespace tact