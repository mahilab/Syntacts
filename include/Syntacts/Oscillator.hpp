#pragma once

#include <Syntacts/Signal.hpp>

namespace tact
{

///////////////////////////////////////////////////////////////////////////////

/// Base class for objects which produce a periodic, oscillating signal
class SYNTACTS_API Oscillator 
{
public:
    /// Constructs an Oscillator with a scalar frequency
    Oscillator(float frequency = 1.0f);
    /// Constucts an Oscillator with a signal based frequency
    Oscillator(Signal frequency);
    /// Returns infinity
    float length() const;
public:
    Signal frequency; ///< the Oscillator's frequency       
private:
    TACT_SERIALIZE(TACT_MEMBER(frequency));
};

///////////////////////////////////////////////////////////////////////////////

/// A sine wave Oscillator
class SYNTACTS_API Sine : public Oscillator
{
public:
    /// Inherited Constructor
    using Oscillator::Oscillator;
    /// Implements sine wave oscillation
    float sample(float t) const;
private:
    TACT_SERIALIZE(TACT_PARENT(Oscillator));
};

///////////////////////////////////////////////////////////////////////////////

/// A square wave Oscillator
class SYNTACTS_API Square : public Oscillator
{
public:
    // Inherited Constructor
    using Oscillator::Oscillator;
    /// Implements square wave oscillation
    float sample(float t) const;
private:
    TACT_SERIALIZE(TACT_PARENT(Oscillator));
};

///////////////////////////////////////////////////////////////////////////////

/// A saw wave Oscillator
class SYNTACTS_API Saw : public Oscillator
{
public:
    // Inherited Constructor
    using Oscillator::Oscillator;
    /// Implements saw wave oscillation
    float sample(float t) const;
private:
    TACT_SERIALIZE(TACT_PARENT(Oscillator));
};

///////////////////////////////////////////////////////////////////////////////

/// A triangle wave Oscillator
class SYNTACTS_API Triangle : public Oscillator
{
public:
    // Inherited Constructor
    using Oscillator::Oscillator;
    /// Implements saw wave oscillation
    float sample(float t) const;
private:
    TACT_SERIALIZE(TACT_PARENT(Oscillator));
};

///////////////////////////////////////////////////////////////////////////////

/// A frequency modulated sine wave Oscillator
class SYNTACTS_API SineFM : public Oscillator
{
public:
    /// Constructor
    SineFM(float frequency = 1, float modulation = 1, float index = 2);
    /// Implements FM sine wave oscillation
    float sample(float t) const ;
public:
    float modulation; ///< modulation frequency
    float index;      ///< modulation index
private:
    TACT_SERIALIZE(TACT_PARENT(Oscillator), TACT_MEMBER(modulation), TACT_MEMBER(index));
};

/// A linear chirp Oscillator
class SYNTACTS_API Chirp : public Oscillator
{
public:
    /// Constructor
    Chirp(float frequency = 1, float rate = 0);
    /// Implements chirp wave oscillation
    float sample(float t) const;
public:
    float rate; ///< the chirp rate or (f1-f0)/T
private:
    TACT_SERIALIZE(TACT_PARENT(Oscillator), TACT_MEMBER(rate));
};

///////////////////////////////////////////////////////////////////////////////

/// A square wave with adjustable frequency and duty cycle
class SYNTACTS_API PulseTrain : public Oscillator
{
public:
    /// Constructor
    PulseTrain(float frequency = 1.0f, float dutyCycle = 0.5f);
    /// Implements pulse train oscillation
    float sample(float t) const;
public:
    float dutyCycle;
    float period;
private:
    TACT_SERIALIZE(TACT_PARENT(Oscillator), TACT_MEMBER(dutyCycle), TACT_MEMBER(period));
};

} // namespace tact