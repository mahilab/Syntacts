#pragma once

#include <Syntacts/Config.hpp>
#include <Syntacts/Generator.hpp>

namespace tact
{

//=============================================================================

/// Abstract base class for objects which produce a periodic, oscillating signal
class SYNTACTS_API Oscillator : public Generator {
public:

    /// Constructs an Oscillator with specified frequency
    Oscillator(float frequency = 1.0f);

    /// Sets the oscillator frequency in Hz
    void setFrequency(float frequency);

protected:

    float m_frequency;  ///< oscillator frequency

};

//=============================================================================

/// A sine wave Oscillator
class SYNTACTS_API SineWave : public Oscillator {
public:

    // Inherited Constructor
    using Oscillator::Oscillator;

    /// Implements sine wave oscillation
    virtual float sample(float t) override;

};

//=============================================================================

/// A square wave Oscillator
class SYNTACTS_API SquareWave : public Oscillator {
public:

    // Inherited Constructor
    using Oscillator::Oscillator;

    /// Implements square wave oscillation
    virtual float sample(float t) override;

};

//=============================================================================

/// A saw wave Oscillator
class SYNTACTS_API SawWave : public Oscillator {
public:

    // Inherited Constructor
    using Oscillator::Oscillator;

    /// Implements saw wave oscillation
    virtual float sample(float t) override;

};

//=============================================================================

/// A triangle wave Oscillator
class SYNTACTS_API TriWave : public Oscillator {
public:

    // Inherited Constructor
    using Oscillator::Oscillator;

    /// Implements saw wave oscillation
    virtual float sample(float t) override;

};

//=============================================================================

/// A frequency modulated sine wave Oscillator
class SYNTACTS_API SineWaveFM : public Oscillator {
public:

    /// Constructor
    SineWaveFM(float frequency, float modulation, float index = 2.0f);

    /// Sets modulation frequency
    void setModulation(float modulation);

    /// Sets modulation index
    void setIndex(float index);

    /// Implements FM sine wave oscillation
    virtual float sample(float t) override;

private:

    float m_modulation; ///< modulation frequency
    float m_index;      ///< modulation index

};

/// A linear chirp Oscillator
class SYNTACTS_API Chirp : public Oscillator {
public:

    /// Constructor
    Chirp(float frequency, float chirpyness);

    /// Sets the chirpyness (i.e. the chirp rate or (f1-f0)/T)
    void setChirpyness(float chirpyness);

    /// Implements chirp wave oscillation
    virtual float sample(float t) override;

private:

    float m_chirpyness; ///< the chirp rate or (f1-f0)/T

};

//=============================================================================

/// A square wave with adjustable frequency and duty cycle
class SYNTACTS_API PulseTrain : public Oscillator {
public:

    /// Constructor
    PulseTrain(float frequency, float dutyCycle = 0.5f);

    /// Sets the duty cycle between 0 and 1
    void setDutyCycle(float dutyCycle);

    /// Implements pulse train oscillation
    virtual float sample(float t) override;

private:

    float m_dutyCycle;
    float m_period;

};

} // namespace tact