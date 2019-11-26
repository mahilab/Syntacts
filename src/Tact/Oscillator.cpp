#include <Tact/Oscillator.hpp>
#include <Tact/Operator.hpp>

namespace tact
{

IOscillator::IOscillator(float hertz) :
    x(TWO_PI * hertz * Time())
{ }

IOscillator::IOscillator(Signal _x) :
    x(std::move(_x))
{ }

Saw::Saw(float hertz) : 
    IOscillator(Scalar(hertz))
{ }

SineFM::SineFM(float _frequency, float _modulation, float _index) :
    IOscillator(_frequency),
    modulation(_modulation),
    index(_index) 
{ }

Chirp::Chirp(float _frequency, float _rate) :
    IOscillator(_frequency),
    rate(_rate)
{ }

Pwm::Pwm(float _frequency, float _dutyCycle) :
    frequency(_frequency), 
    dutyCycle(clamp01(_dutyCycle))
{ }

FM::FM(Signal _carrier, Signal _modulation) :
    carrier(std::move(_carrier)),
    modulation(std::move(_modulation))
{ }

} // namespace tact