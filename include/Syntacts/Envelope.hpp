#pragma once

#include <Syntacts/Config.hpp>
#include <Syntacts/Source.hpp>
#include <Syntacts/Tween.hpp>
#include <Syntacts/Oscillator.hpp>
#include <functional>
#include <map>
#include <utility>

namespace tact
{

//=============================================================================

/// Abstract Base Envelope 
class SYNTACTS_API Envelope : public Source {
public:

    /// Constructor
    Envelope(float duration = 1.0f);

    /// Implements 
    virtual float sample(float t) override;

    /// Gets the duration of an Envelope
    virtual float getDuration() const;

protected:
    float m_duration;
private:
    SERIALIZE(PARENT(Source), MEMBER(m_duration))
};

//=============================================================================

/// Envelope with time sequenced amplitudes and tweening functions
class SYNTACTS_API KeyedEnvelope : public Envelope {
public:

    /// Constucts Envelope with initial amplitude
    KeyedEnvelope(float amplitude0 = 0.0f);

    /// Adds a new amplitude at time t seconds. Uses tween to interpolate from previous amplitude.
    void addKey(float t, float amplitude, TweenFunc tween = Tween::Linear);

    float getDuration() const override;

    virtual float sample(float t) override;

protected:

    std::map<float, std::pair<float, TweenFunc>> m_keys;
    
};

//=============================================================================

/// An object which gives a Cue a duration and/or shape
class SYNTACTS_API AmplitudeEnvelope : public KeyedEnvelope {
public:

    /// Constructs an Eveloope with a specified duration
    AmplitudeEnvelope(float duration = 1.0f, float amplitude = 1.0f);

    /// Gets the normalized amlitude of the Envelope
    float getAmplitude() const;

};

//=============================================================================

/// Attack-Sustain-Release Envelope
class SYNTACTS_API ASR : public KeyedEnvelope {
public:

    /// Constructs ASR Envelope with specified attack, sustain, and release times
    ASR(float attackTime, float sustainTime, float releaseTime, float attackAmlitude = 1.0f, 
        TweenFunc attackTween = Tween::Linear, TweenFunc releaseTween = Tween::Linear);

};

//=============================================================================

/// Attack-Decay-Sustain-Release Envelope
class SYNTACTS_API ADSR : public KeyedEnvelope {
public:

    /// Constructs ASR Envelope with specified attack, sustain, and release times
    ADSR(float attackTime, float decayTime, float sustainTime, float releaseTime, float attackAmplitude = 1.0f, float decayAplitude = 0.5f, 
         TweenFunc attackTween = Tween::Linear, TweenFunc decayTween = Tween::Linear, TweenFunc releaseTween = Tween::Linear);

};

//=============================================================================

/// An object which gives a Cue a duration and/or shape
class SYNTACTS_API OscillatingEnvelope : public Envelope {
public:

    /// Constructs an Envelope with a specified duration, positive oscillator type and frequency
    OscillatingEnvelope(float duration , float amplitude, std::shared_ptr<Oscillator> osc);

    virtual float sample(float t) override;

protected:

    std::shared_ptr<Oscillator> m_osc;
    float m_amplitude;
};

} // namespace tact
