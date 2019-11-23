#include <Syntacts/Signal.hpp>
#include <random>
#include <ctime>

namespace tact
{

Signal::Signal() : Signal(Scalar()) {}

float Signal::sample(float t) const
{
    return m_ptr->sample(t);
}

float Signal::length() const
{
    return m_ptr->length();
}

Scalar::Scalar(float _value) : value(_value) {}
float Scalar::sample(float t) const
{
    return value;
}
float Scalar::length() const
{
    return INF;
}

Ramp::Ramp(float _initial, float _rate) : initial(_initial), rate(_rate) {}
Ramp::Ramp(float _initial, float _final, float _span) : initial(_initial), rate((_final - _initial) / _span) {}
float Ramp::sample(float t) const { return initial + rate * t; }
float Ramp::length() const { return INF; }

Noise::Noise() {}

float Noise::sample(float t) const
{
    float R1 = (float)rand() / (float)RAND_MAX;
    float R2 = (float)rand() / (float)RAND_MAX;
    float X = (float)sqrt(-2.0f * log(R1)) * cos(2.0f * PI * R2);
    return X;
}

float Noise::length() const
{
    return INF;
}

} // namespace tact