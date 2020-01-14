#include <Tact/Oscillator.hpp>
#include <Tact/Operator.hpp>

namespace tact
{

IOscillator::IOscillator() :
    IOscillator(100)
{ }

IOscillator::IOscillator(double hertz) :
    x(std::move(TWO_PI * hertz * Time()))
{ }

IOscillator::IOscillator(double initial, double rate) :
    x(std::move( ( TWO_PI * initial + PI * rate * Time() ) * Time() ))
{ }

IOscillator::IOscillator(Signal _x) :
    x(std::move(_x))
{ }

IOscillator::IOscillator(double hertz, Signal modulation, double index) :
    x(std::move(TWO_PI * hertz * Time() + index * modulation))
{ }

Pwm::Pwm(double _frequency, double _dutyCycle) :
    frequency(_frequency), 
    dutyCycle(clamp01(_dutyCycle))
{ }

} // namespace tact