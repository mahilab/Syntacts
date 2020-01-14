#pragma once

#include <Tact/Signal.hpp>

namespace tact
{

///////////////////////////////////////////////////////////////////////////////

/// Interface class for objects which produce a periodic, oscillating signal
class SYNTACTS_API IOscillator 
{
public:
    /// Default constructor
    IOscillator();
    /// Constucts an Oscillator with a signal based input
    IOscillator(Signal x);
    /// Constructs an Oscillator with a scalar frequency in hertz
    IOscillator(double hertz);
    /// Constructs a "chirp" Oscillator with an initial frequency and ramp rate in hertx
    IOscillator(double initial, double rate);
    /// Constructs frequency modulated (FM) Oscillator 
    IOscillator(double hertz, Signal modulation, double index = 2.0);
    /// Returns infinity
    inline double length() const;
public:
    Signal x; ///< the Oscillator's input       
private:
    TACT_SERIALIZE(TACT_MEMBER(x));
};

///////////////////////////////////////////////////////////////////////////////

/// A sine wave Oscillator
class SYNTACTS_API Sine : public IOscillator
{
public:
    using IOscillator::IOscillator;
    inline double sample(double t) const;
private:
    TACT_SERIALIZE(TACT_PARENT(IOscillator));
};

///////////////////////////////////////////////////////////////////////////////

/// A square wave Oscillator
class SYNTACTS_API Square : public IOscillator
{
public:
    using IOscillator::IOscillator;
    inline double sample(double t) const;
private:
    TACT_SERIALIZE(TACT_PARENT(IOscillator));
};

///////////////////////////////////////////////////////////////////////////////

/// A saw wave Oscillator
class SYNTACTS_API Saw : public IOscillator
{
public:
    using IOscillator::IOscillator;
    inline double sample(double t) const;
private:
    TACT_SERIALIZE(TACT_PARENT(IOscillator));
};

///////////////////////////////////////////////////////////////////////////////

/// A triangle wave Oscillator
class SYNTACTS_API Triangle : public IOscillator
{
public:
    using IOscillator::IOscillator;
    inline double sample(double t) const;
private:
    TACT_SERIALIZE(TACT_PARENT(IOscillator));
};

///////////////////////////////////////////////////////////////////////////////

/// A PWM square wave with adjustable frequency and duty cycle
class SYNTACTS_API Pwm
{
public:
    /// Constructor
    Pwm(double frequency = 1.0, double dutyCycle = 0.5);
    inline double sample(double t) const;
    inline double length() const;
public:
    double frequency;
    double dutyCycle;
private:
    TACT_SERIALIZE(TACT_MEMBER(frequency), TACT_MEMBER(dutyCycle));
};

///////////////////////////////////////////////////////////////////////////////



} // namespace tact

#include <Tact/Detail/Oscillator.inl>