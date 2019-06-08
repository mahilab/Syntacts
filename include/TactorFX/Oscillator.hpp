#pragma once

#include <TactorFX/Config.hpp>
#include <TactorFX/Generator.hpp>

namespace tfx
{

/// Abstract base class for object which produces a periodic, oscillating signal
class TFX_API Oscillator : public Generator {
public:

    /// Constructs an Oscillator with specified frequency
    Oscillator(float frequency = 1.0f);

    /// Sets the oscillator frequency in Hz
    void setFrequency(float frequency);

protected:

    float m_frequency;  ///< oscillator frequency

};


/// A sine wave Oscillator
class TFX_API SineWave : public Oscillator {
public:

    // Inherited Constructor
    using Oscillator::Oscillator;

protected:

    /// Implements sine wave oscillation
    virtual float onSample(float t) override;

};


/// A square wave Oscillator
class TFX_API SquareWave : public Oscillator {
public:

    // Inherited Constructor
    using Oscillator::Oscillator;

protected:

    /// Implements square wave oscillation
    virtual float onSample(float t) override;

};


/// A saw wave Oscillator
class TFX_API SawWave : public Oscillator {
public:

    // Inherited Constructor
    using Oscillator::Oscillator;

protected:

    /// Implements saw wave oscillation
    virtual float onSample(float t) override;

};

/// A triangle wave Oscillator
class TFX_API TriWave : public Oscillator {
public:

    // Inherited Constructor
    using Oscillator::Oscillator;

protected:

    /// Implements saw wave oscillation
    virtual float onSample(float t) override;

};

} // namespace tfx