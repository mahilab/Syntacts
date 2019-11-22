#include <Syntacts/Oscillator.hpp>

namespace tact
{


IOscillator::IOscillator(float _frequency) :
    frequency(move(create<Scalar>(_frequency)))
{ }

IOscillator::IOscillator(Ptr<ISignal> _frequency) :
    frequency(std::move(_frequency))
{ }

float Sine::sample(float t) const {
    return std::sin(TWO_PI * frequency->sample(t) * t);
}

float Square::sample(float t) const {
    return std::sin(TWO_PI * frequency->sample(t) * t) > 0 ? 1.0f : -1.0f;
}

float Saw::sample(float t) const {
    // return -2.0f / PI * std::atan(std::cos(PI * frequency * t) / std::sin(PI * frequency * t));
    return (2.0f / PI) * (frequency->sample(t) * PI * std::fmod(t, 1.0f / frequency->sample(t)) - (PI / 2.0f));
    // float out = 0.0;
    // for (float n = 1.0; n < 10; n++)
    //     out += (sin(n * TWO_PI * frequency * t)) / n;
    // return out * (2.0f / PI);
}

float Triangle::sample(float t) const {
    return 2.0f / PI * std::asin(std::sin(2.0f * PI * frequency->sample(t) * t));
}

SineFM::SineFM(float _frequency, float _modulation, float _index) :
    Cloneable(_frequency),
    modulation(_modulation),
    index(_index) 
{

}

float SineFM::sample(float t) const {
    return std::sin(2.0f * PI * frequency->sample(t) * t + index * std::sin(2.0f * PI * modulation * t));
}

Chirp::Chirp(float _frequency, float _rate) :
    Cloneable(_frequency),
    rate(_rate)
{

}

float Chirp::sample(float t) const {
    float phi = 2 * PI * (frequency->sample(t) + 0.5f * rate * t) * t;
    return std::sin(phi);
}

PulseTrain::PulseTrain(float _frequency, float _dutyCycle) :
    Cloneable(_frequency), 
    dutyCycle(clamp01(_dutyCycle)),
    period(1.0f / _frequency)
{

}

float PulseTrain::sample(float t) const {
    float percentWithinPeriod = std::fmod(t, period) * frequency->sample(t);
    if (percentWithinPeriod < dutyCycle) 
        return 1;
    else 
        return -1;
}

} // namespace tact