#include <Tact/Oscillator.hpp>

namespace tact
{


Oscillator::Oscillator(float _frequency) :
    frequency(Scalar(_frequency))
{ }

Oscillator::Oscillator(Signal _frequency) :
    frequency(_frequency)
{ }

float Oscillator::length() const {
    return INF;
}

float Sine::sample(float t) const {
    return std::sin(TWO_PI * frequency.sample(t) * t);
}

float Square::sample(float t) const {
    return std::sin(TWO_PI * frequency.sample(t) * t) > 0 ? 1.0f : -1.0f;
}

float Saw::sample(float t) const {
    return 2 * INV_PI * (frequency.sample(t) * PI * std::fmod(t, 1.0f / frequency.sample(t)) - HALF_PI);
}

float Triangle::sample(float t) const {
    return 2 * INV_PI * std::asin(std::sin(TWO_PI * frequency.sample(t) * t));
}

SineFM::SineFM(float _frequency, float _modulation, float _index) :
    Oscillator(_frequency),
    modulation(_modulation),
    index(_index) 
{

}

float SineFM::sample(float t) const {
    return std::sin(2.0f * PI * frequency.sample(t) * t + index * std::sin(2.0f * PI * modulation * t));
}

Chirp::Chirp(float _frequency, float _rate) :
    Oscillator(_frequency),
    rate(_rate)
{

}

float Chirp::sample(float t) const {
    float phi = 2 * PI * (frequency.sample(t) + 0.5f * rate * t) * t;
    return std::sin(phi);
}

PulseTrain::PulseTrain(float _frequency, float _dutyCycle) :
    Oscillator(_frequency), 
    dutyCycle(clamp01(_dutyCycle)),
    period(1.0f / _frequency)
{

}

float PulseTrain::sample(float t) const {
    float percentWithinPeriod = std::fmod(t, period) * frequency.sample(t);
    if (percentWithinPeriod < dutyCycle) 
        return 1;
    else 
        return -1;
}

} // namespace tact