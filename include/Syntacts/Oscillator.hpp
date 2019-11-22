#pragma once

#include <Syntacts/Config.hpp>
#include <Syntacts/Signal.hpp>

namespace tact
{

///////////////////////////////////////////////////////////////////////////////

/// Abstract base class for objects which produce a periodic, oscillating signal
class SYNTACTS_API IOscillator : public Cloneable<Interface<IOscillator>, ISignal>
{
public:
    /// Constructs an Oscillator with a scalar frequency
    IOscillator(float frequency = 1.0f);
    /// Constucts an Oscillator with a signal based frequency
    IOscillator(Ptr<ISignal> frequency);
public:
    Ptr<ISignal> frequency; ///< the Oscillator's frequency       
private:
    TACT_SERIALIZE(TACT_PARENT(ISignal), TACT_MEMBER(frequency));
};

///////////////////////////////////////////////////////////////////////////////

/// A sine wave Oscillator
class SYNTACTS_API Sine : public Cloneable<Sine, IOscillator>
{
public:
    /// Inherited Constructor
    using Cloneable::Cloneable;

    /// Implements sine wave oscillation
    virtual float sample(float t) const override;
private:
    TACT_SERIALIZE(TACT_PARENT(IOscillator))
};

///////////////////////////////////////////////////////////////////////////////

/// A square wave Oscillator
class SYNTACTS_API Square : public Cloneable<Square, IOscillator>
{
public:
    // Inherited Constructor
    using Cloneable::Cloneable;

    /// Implements square wave oscillation
    virtual float sample(float t) const override;
private:
    TACT_SERIALIZE(TACT_PARENT(IOscillator));
};

///////////////////////////////////////////////////////////////////////////////

/// A saw wave Oscillator
class SYNTACTS_API Saw : public Cloneable<Saw, IOscillator>
{
public:
    // Inherited Constructor
    using Cloneable::Cloneable;

    /// Implements saw wave oscillation
    virtual float sample(float t) const override;
private:
    TACT_SERIALIZE(TACT_PARENT(IOscillator));
};

///////////////////////////////////////////////////////////////////////////////

/// A triangle wave Oscillator
class SYNTACTS_API Triangle : public Cloneable<Triangle, IOscillator>
{
public:
    // Inherited Constructor
    using Cloneable::Cloneable;

    /// Implements saw wave oscillation
    virtual float sample(float t) const override;
private:
    TACT_SERIALIZE(TACT_PARENT(IOscillator));
};

///////////////////////////////////////////////////////////////////////////////

/// A frequency modulated sine wave Oscillator
class SYNTACTS_API SineFM : public Cloneable<SineFM, IOscillator>
{
public:
    /// Constructor
    SineFM(float frequency = 1, float modulation = 1, float index = 2);
    /// Implements FM sine wave oscillation
    virtual float sample(float t) const override;
public:
    float modulation; ///< modulation frequency
    float index;      ///< modulation index
private:
    TACT_SERIALIZE(TACT_PARENT(IOscillator), TACT_MEMBER(modulation), TACT_MEMBER(index))
};

/// A linear chirp Oscillator
class SYNTACTS_API Chirp : public Cloneable<Chirp, IOscillator>
{
public:
    /// Constructor
    Chirp(float frequency = 1, float rate = 0);
    /// Implements chirp wave oscillation
    virtual float sample(float t) const override;
public:
    float rate; ///< the chirp rate or (f1-f0)/T
private:
    TACT_SERIALIZE(TACT_PARENT(IOscillator), TACT_MEMBER(rate))
};

///////////////////////////////////////////////////////////////////////////////

/// A square wave with adjustable frequency and duty cycle
class SYNTACTS_API PulseTrain : public Cloneable<PulseTrain, IOscillator>
{
public:
    /// Constructor
    PulseTrain(float frequency = 1.0f, float dutyCycle = 0.5f);
    /// Implements pulse train oscillation
    virtual float sample(float t) const override;
public:
    float dutyCycle;
    float period;
private:
    TACT_SERIALIZE(TACT_PARENT(IOscillator), TACT_MEMBER(dutyCycle), TACT_MEMBER(period));
};

} // namespace tact