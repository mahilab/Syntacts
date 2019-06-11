#pragma once

#include <Syntacts/Config.hpp>
#include <Syntacts/Generator.hpp>

namespace tact
{

/// An object which gives a Cue a duration and/or shape
class SYNTACTS_API Envelope : public Generator {
public:

    /// Constructs an Eveloope with a specified duration
    Envelope(float duration = 1.0f, float amplitude = 1.0f);

    /// Sets the duration of an Envelope (default 1)
    void setDuration(float duration);

    /// Gets the duration of an Envelope
    float getDuration() const;

    /// Sets the normalized amplitude of the Envelope (default 1)
    void setAmplitude(float amplitude);

    /// Gets the normalized amlitude of the Envelope
    float getAmplitude() const;

protected:

    /// Returns true as long as current time less than duration
    bool playing() const;

    /// Default Envelope implementation (returns 1 for full duration)
    virtual float onSample(float t) override;

protected:

    float m_duration;  ///< duration of an Envelope in seconds
    float m_amplitude; ///< amplitude of the envelope normalized between 0 and 1

};


/// Linear Attack-Sustain-Release Envelope
class SYNTACTS_API ASR : public Envelope {
public:

    /// Constructs ASR Envelope with specified attack, sustain, and release times
    ASR(float attackTime, float sustainTime, float releaseTime, float amplitude = 1.0f);

protected:

    /// ASR implementation
    virtual float onSample(float t) override;
    
protected:

    float m_attackTime;
    float m_sustainTime;
    float m_releaseTime;

};

/// Linear Attack-Decay-Sustain-Release Envelope
class SYNTACTS_API ADSR : public ASR {
public:

    /// Constructs ASR Envelope with specified attack, sustain, and release times
    ADSR(float attackTime, float decayTime, float sustainTime, float releaseTime, float amplitude1 = 1.0f, float amplitude2 = 0.5f);

protected:

    /// ADSR implementation
    virtual float onSample(float t) override;

protected:

    float m_decayTime;
    float m_amplitude2;

};

    
} // namespace tact
