#include <Tact/Oscillator.hpp>
#include <Tact/Operator.hpp>

namespace tact
{

Oscillator::Oscillator() { }

Oscillator::Oscillator(float hertz) :
    x(TWO_PI * hertz * Time())
{ }

Oscillator::Oscillator(Signal _x) :
    x(std::move(_x))
{ }

Saw::Saw(float hertz) : 
    Oscillator(Scalar(hertz))
{ }

SineFM::SineFM(float _frequency, float _modulation, float _index) :
    Oscillator(_frequency),
    modulation(_modulation),
    index(_index) 
{ }

Chirp::Chirp(float _frequency, float _rate) :
    Oscillator(_frequency),
    rate(_rate)
{ }

PulseTrain::PulseTrain(float _frequency, float _dutyCycle) :
    Oscillator(_frequency), 
    dutyCycle(clamp01(_dutyCycle)),
    period(1.0f / _frequency)
{ }

} // namespace tact