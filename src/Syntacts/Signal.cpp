#include <Syntacts/Signal.hpp>

namespace tact
{

int ISignal::s_count = 0;

ISignal::ISignal() {
    s_count++;
}

ISignal::~ISignal() {
    s_count--;
}

int ISignal::count() {
    return s_count;
}

Scalar::Scalar(float _value) : value(_value) {}
float Scalar::sample(float t) const { return value; }

Ramp::Ramp(float _initial, float _rate) : initial(_initial), rate(_rate) {}
Ramp::Ramp(float _initial, float _final, float _span) : initial(_initial), rate((_final - _initial) / _span) {}
float Ramp::sample(float t) const { return initial + rate * t; }

} // namespace tact