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
    IOscillator(float hertz);
    /// Constructs an Oscillator with frequency modulation (FM)
    IOscillator(float hertz, Signal modulation, float index = 2.0f);
    /// Returns infinity
    inline float length() const;
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
    inline float sample(float t) const;
private:
    TACT_SERIALIZE(TACT_PARENT(IOscillator));
};

///////////////////////////////////////////////////////////////////////////////

/// A square wave Oscillator
class SYNTACTS_API Square : public IOscillator
{
public:
    using IOscillator::IOscillator;
    inline float sample(float t) const;
private:
    TACT_SERIALIZE(TACT_PARENT(IOscillator));
};

///////////////////////////////////////////////////////////////////////////////

/// A saw wave Oscillator
class SYNTACTS_API Saw : public IOscillator
{
public:
    using IOscillator::IOscillator;
    inline float sample(float t) const;
private:
    TACT_SERIALIZE(TACT_PARENT(IOscillator));
};

///////////////////////////////////////////////////////////////////////////////

/// A triangle wave Oscillator
class SYNTACTS_API Triangle : public IOscillator
{
public:
    using IOscillator::IOscillator;
    inline float sample(float t) const;
private:
    TACT_SERIALIZE(TACT_PARENT(IOscillator));
};

///////////////////////////////////////////////////////////////////////////////

/// A linear chirp Oscillator
class SYNTACTS_API Chirp : public IOscillator
{
public:
    /// Constructor
    Chirp();
    Chirp(float frequency, float rate);
    inline float sample(float t) const;
public:
    float rate; ///< the chirp rate or (f1-f0)/T
private:
    TACT_SERIALIZE(TACT_PARENT(IOscillator), TACT_MEMBER(rate));
};

///////////////////////////////////////////////////////////////////////////////

/// A PWM square wave with adjustable frequency and duty cycle
class SYNTACTS_API Pwm
{
public:
    /// Constructor
    Pwm(float frequency = 1.0f, float dutyCycle = 0.5f);
    inline float sample(float t) const;
    inline float length() const;
public:
    float frequency;
    float dutyCycle;
private:
    TACT_SERIALIZE(TACT_MEMBER(frequency), TACT_MEMBER(dutyCycle));
};

///////////////////////////////////////////////////////////////////////////////



} // namespace tact

#include <Tact/Detail/Oscillator.inl>