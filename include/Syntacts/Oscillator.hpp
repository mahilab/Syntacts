#pragma once

#include <Syntacts/Config.hpp>
#include <Syntacts/Signal.hpp>

namespace tact
{

///////////////////////////////////////////////////////////////////////////////

/// Abstract base class for objects which produce a periodic, oscillating signal
class SYNTACTS_API IOscillator : public ISignal
{
public:
    /// Constructs an Oscillator with a scalar frequency
    IOscillator(float frequency = 1.0f);

    /// Constructs an Oscillator with a variable frequency
    IOscillator(Ptr<ISignal> frequency);

protected:
    Ptr<ISignal> m_frequency;    

private:
    SERIALIZE(PARENT(ISignal), MEMBER(m_frequency));
};

///////////////////////////////////////////////////////////////////////////////

/// A sine wave Oscillator
class SYNTACTS_API Sine : public IOscillator
{
public:
    /// Inherited Constructor
    using IOscillator::IOscillator;

    /// Implements sine wave oscillation
    virtual float sample(float t) const override;
private:
    SERIALIZE(PARENT(IOscillator))
};

///////////////////////////////////////////////////////////////////////////////

/// A square wave Oscillator
class SYNTACTS_API Square : public IOscillator
{
public:
    // Inherited Constructor
    using IOscillator::IOscillator;

    /// Implements square wave oscillation
    virtual float sample(float t) const override;
private:
    SERIALIZE(PARENT(IOscillator));
};

///////////////////////////////////////////////////////////////////////////////

/// A saw wave Oscillator
class SYNTACTS_API Saw : public IOscillator
{
public:
    // Inherited Constructor
    using IOscillator::IOscillator;

    /// Implements saw wave oscillation
    virtual float sample(float t) const override;
private:
    SERIALIZE(PARENT(IOscillator));
};

///////////////////////////////////////////////////////////////////////////////

/// A triangle wave Oscillator
class SYNTACTS_API Triangle : public IOscillator
{
public:
    // Inherited Constructor
    using IOscillator::IOscillator;

    /// Implements saw wave oscillation
    virtual float sample(float t) const override;
private:
    SERIALIZE(PARENT(IOscillator));
};

///////////////////////////////////////////////////////////////////////////////

/// A frequency modulated sine wave Oscillator
class SYNTACTS_API SineFM : public IOscillator
{
public:
    /// Constructor
    SineFM(float frequency = 1, float modulation = 1, float index = 2);

    /// Implements FM sine wave oscillation
    virtual float sample(float t) const override;

private:
    float m_modulation; ///< modulation frequency
    float m_index;      ///< modulation index
private:
    SERIALIZE(PARENT(IOscillator), MEMBER(m_modulation), MEMBER(m_index))
};

/// A linear chirp Oscillator
class SYNTACTS_API Chirp : public IOscillator
{
public:
    /// Constructor
    Chirp(float frequency = 1, float chirpyness = 0);

    /// Implements chirp wave oscillation
    virtual float sample(float t) const override;

private:
    float m_chirpyness; ///< the chirp rate or (f1-f0)/T
private:
    SERIALIZE(PARENT(IOscillator), MEMBER(m_chirpyness))
};

///////////////////////////////////////////////////////////////////////////////

/// A square wave with adjustable frequency and duty cycle
class SYNTACTS_API PulseTrain : public IOscillator
{
public:
    /// Constructor
    PulseTrain(float frequency = 1, float dutyCycle = 0.5f);

    /// Implements pulse train oscillation
    virtual float sample(float t) const override;

private:
    float m_dutyCycle;
    float m_period;
private:
    SERIALIZE(PARENT(IOscillator), MEMBER(m_dutyCycle), MEMBER(m_period));
};

} // namespace tact