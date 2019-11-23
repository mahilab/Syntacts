#include <Syntacts/Signal.hpp>
#include <random>
#include <ctime>

namespace tact
{

Signal::Signal() : Signal(Scalar()) { }

float Signal::sample(float t) const {
    return m_ptr->sample(t); 
}

float Signal::length() const {
    return m_ptr->length();
}

int ISignal::s_count = 0;

ISignal::ISignal() :
    m_length(INF)
{
    s_count++;
}

ISignal::~ISignal() {
    s_count--;
}

int ISignal::count() {
    return s_count;
}

float ISignal::length() const {
    return m_length;
}

Scalar::Scalar(float _value) : value(_value) {}
float Scalar::sample(float t) const { return value; }

Ramp::Ramp(float _initial, float _rate) : initial(_initial), rate(_rate) {}
Ramp::Ramp(float _initial, float _final, float _span) : initial(_initial), rate((_final - _initial) / _span) {}
float Ramp::sample(float t) const { return initial + rate * t; }

Noise::Noise() {}

float Noise::sample(float t) const {
    float R1 = (float) rand() / (float) RAND_MAX;
    float R2 = (float) rand() / (float) RAND_MAX;
    float X = (float) sqrt( -2.0f * log( R1 )) * cos( 2.0f * PI * R2 );
    return X;
}

} // namespace tact