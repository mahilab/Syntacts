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
    virtual float sample(float t) const override;

    /// Gets the duration of an Envelope
    virtual float getDuration() const;

protected:
    float m_duration;
private:
    SERIALIZE(PARENT(Source), MEMBER(m_duration));
};

//=============================================================================

/// Envelope with time sequenced amplitudes and tweening functions
class SYNTACTS_API KeyedEnvelope : public Envelope {
public:

    typedef Ptr<Tween::Function> TweenFunc;

    /// Constucts Envelope with initial amplitude
    KeyedEnvelope(float amplitude0 = 0.0f);

    /// Adds a new amplitude at time t seconds. Uses tween to interpolate from previous amplitude.
    void addKey(float t, float amplitude, TweenFunc tween = create<Tween::Linear>());

    float getDuration() const override;

    virtual float sample(float t) const override;

protected:

    std::map<float, std::pair<float, TweenFunc>> m_keys;

private:
    SERIALIZE(PARENT(Envelope), MEMBER(m_keys));    
};

//=============================================================================

/// An object which gives a Cue a duration and/or shape
class SYNTACTS_API AmplitudeEnvelope : public KeyedEnvelope {
public:
    /// Constructs an Eveloope with a specified duration
    AmplitudeEnvelope(float duration = 1.0f, float amplitude = 1.0f);
    /// Gets the normalized amlitude of the Envelope
    float getAmplitude() const;
private:
    SERIALIZE(PARENT(KeyedEnvelope));
};

//=============================================================================

/// Attack-Sustain-Release Envelope
class SYNTACTS_API ASR : public KeyedEnvelope {
public:
    /// Constructs ASR Envelope with specified attack, sustain, and release times
    ASR(float attackTime = 1.0f, float sustainTime = 1.0f, float releaseTime = 1.0f, float attackAmlitude = 1.0f, 
        TweenFunc attackTween = create<Tween::Linear>(), TweenFunc releaseTween = create<Tween::Linear>());
private:
    SERIALIZE(PARENT(KeyedEnvelope));
};

//=============================================================================

/// Attack-Decay-Sustain-Release Envelope
class SYNTACTS_API ADSR : public KeyedEnvelope {
public:
    /// Constructs ASR Envelope with specified attack, sustain, and release times
    ADSR(float attackTime = 1.0f, float decayTime = 1.0f, float sustainTime = 1.0f, float releaseTime = 1.0f, float attackAmplitude = 1.0f, float decayAplitude = 0.5f, 
         TweenFunc attackTween = create<Tween::Linear>(), TweenFunc decayTween = create<Tween::Linear>(), TweenFunc releaseTween = create<Tween::Linear>());
private:
    SERIALIZE(PARENT(KeyedEnvelope));
};

//=============================================================================

/// An object which gives a Cue a duration and/or shape
class SYNTACTS_API OscillatingEnvelope : public Envelope {
public:
    /// Constructs an Envelope with a specified duration, positive oscillator type and frequency
    OscillatingEnvelope(float duration = 1.0f , float amplitude = 1.0f, Ptr<Oscillator> osc = create<SineWave>());
    virtual float sample(float t) const override;
protected:
    Ptr<Oscillator> m_osc;
    std::atomic<float> m_amplitude;
private:
    SERIALIZE(PARENT(Envelope), MEMBER(m_osc), MEMBER(m_amplitude));
};

} // namespace tact
