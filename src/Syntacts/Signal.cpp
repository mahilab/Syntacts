#include <Syntacts/Signal.hpp>

namespace tact
{

int SignalBase::s_count = 0;

SignalBase::SignalBase() {
    s_count++;
}

SignalBase::~SignalBase() {
    s_count--;
}

int SignalBase::count() {
    return s_count;
}

Ptr<SignalBase> SignalBase::clone() const {
    return create<Scalar>(1);
}

Scalar::Scalar(float _value) : value(_value) {}
float Scalar::sample(float t) const { return value; }

Ramp::Ramp(float _initial, float _rate) : initial(_initial), rate(_rate) {}
Ramp::Ramp(float _initial, float _final, float _span) : initial(_initial), rate((_final - _initial) / _span) {}
float Ramp::sample(float t) const { return initial + rate * t; }

} // namespace tact