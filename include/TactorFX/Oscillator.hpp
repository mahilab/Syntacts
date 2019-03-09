#pragma once

#include <TactorFX/Generator.hpp>

namespace tfx
{

/// Abstract base class for object which produces a periodic, oscillating signal
class Oscillator : public Generator {
public:

    /// Constructs an Oscillator with specified frequency and amplitude
    Oscillator(float frequency = 1.0f, float amplitude = 1.0f);

    /// Sets the oscillator frequency in Hz
    void setFrequency(float frequency);

    /// Sets the oscillator amplitude to the absolute value of amplitude
    void setAmplitude(float amplitude);

protected:

    float m_frequency;  ///< oscillator frequency
    float m_amplitude;  ///< oscillator amplitude

};


/// A sine wave Oscillator
class SineWave : public Oscillator {
public:

    // Inherited Constructor
    using Oscillator::Oscillator;

protected:

    /// Implements sine wave oscillation
    virtual float onSample(float t) override;

};


/// A square wave Oscillator
class SquareWave : public Oscillator {
public:

    // Inherited Constructor
    using Oscillator::Oscillator;

protected:

    /// Implements square wave oscillation
    virtual float onSample(float t) override;

};


/// A saw wave Oscillator
class SawWave : public Oscillator {
public:

    // Inherited Constructor
    using Oscillator::Oscillator;

protected:

    /// Implements saw wave oscillation
    virtual float onSample(float t) override;

};

/// A triangle wave Oscillator
class TriWave : public Oscillator {
public:

    // Inherited Constructor
    using Oscillator::Oscillator;

protected:

    /// Implements saw wave oscillation
    virtual float onSample(float t) override;

};

} // namespace tfx