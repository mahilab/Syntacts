#pragma once

#include <Syntacts/Config.hpp>
#include <Syntacts/Signal.hpp>

namespace tact
{

///////////////////////////////////////////////////////////////////////////////

/// Abstract base class for objects which produce a periodic, oscillating signal
class SYNTACTS_API OscillatorBase : public SignalBase
{
public:
    /// Constructs an Oscillator with a scalar frequency
    OscillatorBase(float frequency = 1.0f);

    /// Constructs an Oscillator with a variable frequency
    OscillatorBase(Ptr<SignalBase> frequency);

protected:
    Ptr<SignalBase> m_frequency;    

private:
    SERIALIZE(PARENT(SignalBase), MEMBER(m_frequency));
};

///////////////////////////////////////////////////////////////////////////////

/// A sine wave Oscillator
class SYNTACTS_API Sine : public OscillatorBase
{
public:
    /// Inherited Constructor
    using OscillatorBase::OscillatorBase;

    /// Implements sine wave oscillation
    virtual float sample(float t) const override;
private:
    SERIALIZE(PARENT(OscillatorBase))
};

///////////////////////////////////////////////////////////////////////////////

/// A square wave Oscillator
class SYNTACTS_API Square : public OscillatorBase
{
public:
    // Inherited Constructor
    using OscillatorBase::OscillatorBase;

    /// Implements square wave oscillation
    virtual float sample(float t) const override;
private:
    SERIALIZE(PARENT(OscillatorBase));
};

///////////////////////////////////////////////////////////////////////////////

/// A saw wave Oscillator
class SYNTACTS_API Saw : public OscillatorBase
{
public:
    // Inherited Constructor
    using OscillatorBase::OscillatorBase;

    /// Implements saw wave oscillation
    virtual float sample(float t) const override;
private:
    SERIALIZE(PARENT(OscillatorBase));
};

///////////////////////////////////////////////////////////////////////////////

/// A triangle wave Oscillator
class SYNTACTS_API Triangle : public OscillatorBase
{
public:
    // Inherited Constructor
    using OscillatorBase::OscillatorBase;

    /// Implements saw wave oscillation
    virtual float sample(float t) const override;
private:
    SERIALIZE(PARENT(OscillatorBase));
};

///////////////////////////////////////////////////////////////////////////////

/// A frequency modulated sine wave Oscillator
class SYNTACTS_API SineFM : public OscillatorBase
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
    SERIALIZE(PARENT(OscillatorBase), MEMBER(m_modulation), MEMBER(m_index))
};

/// A linear chirp Oscillator
class SYNTACTS_API Chirp : public OscillatorBase
{
public:
    /// Constructor
    Chirp(float frequency = 1, float chirpyness = 0);

    /// Implements chirp wave oscillation
    virtual float sample(float t) const override;

private:
    float m_chirpyness; ///< the chirp rate or (f1-f0)/T
private:
    SERIALIZE(PARENT(OscillatorBase), MEMBER(m_chirpyness))
};

///////////////////////////////////////////////////////////////////////////////

/// A square wave with adjustable frequency and duty cycle
class SYNTACTS_API PulseTrain : public OscillatorBase
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
    SERIALIZE(PARENT(OscillatorBase), MEMBER(m_dutyCycle), MEMBER(m_period));
};

} // namespace tact