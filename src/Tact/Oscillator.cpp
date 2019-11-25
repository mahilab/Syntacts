#include <Tact/Oscillator.hpp>
#include <Tact/Operator.hpp>
#include <iostream>

namespace tact
{

Oscillator::Oscillator() {

}

Oscillator::Oscillator(float hertz) :
    x(TWO_PI * hertz * Time())
{ 
}

Oscillator::Oscillator(Signal _x) :
    x(std::move(_x))
{ 
}

float Oscillator::length() const {
    return INF;
}

float Sine::sample(float t) const {
    return std::sin(x.sample(t));
}

float Square::sample(float t) const {
    return std::sin(x.sample(t)) > 0 ? 1.0f : -1.0f;
}

Saw::Saw(float hertz) : Oscillator(Scalar(hertz)) { }


float Saw::sample(float t) const {
    return 2 * INV_PI * (x.sample(t) * PI * std::fmod(t, 1.0f / x.sample(t)) - HALF_PI);
}

float Triangle::sample(float t) const {
    return 2 * INV_PI * std::asin(std::sin(x.sample(t)));
}

SineFM::SineFM(float _frequency, float _modulation, float _index) :
    Oscillator(_frequency),
    modulation(_modulation),
    index(_index) 
{

}

float SineFM::sample(float t) const {
    return std::sin(2.0f * PI * x.sample(t) * t + index * std::sin(2.0f * PI * modulation * t));
}

Chirp::Chirp(float _frequency, float _rate) :
    Oscillator(_frequency),
    rate(_rate)
{

}

float Chirp::sample(float t) const {
    float phi = 2 * PI * (x.sample(t) + 0.5f * rate * t) * t;
    return std::sin(phi);
}

PulseTrain::PulseTrain(float _frequency, float _dutyCycle) :
    Oscillator(_frequency), 
    dutyCycle(clamp01(_dutyCycle)),
    period(1.0f / _frequency)
{

}

float PulseTrain::sample(float t) const {
    float percentWithinPeriod = std::fmod(t, period) * x.sample(t);
    if (percentWithinPeriod < dutyCycle) 
        return 1;
    else 
        return -1;
}

} // namespace tact