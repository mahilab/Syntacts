#pragma once

#include <Syntacts/Signal.hpp>
#include <Syntacts/Tween.hpp>
#include <Syntacts/Oscillator.hpp>
#include <utility>
#include <map>

namespace tact
{

//=============================================================================

/// Abstract base class for object which gives a Cue a duration and/or shape
class SYNTACTS_API EnvelopeBase : public SignalBase {
public:

    /// Constructor
    EnvelopeBase(float duration = 1.0f);

    /// Implements a time bounded envelope with amplitude of 1
    virtual float sample(float t) const override;

    /// Gets the duration of an Envelope
    virtual float getDuration() const;

protected:
    std::atomic<float> m_duration;
private:
    SERIALIZE(PARENT(SignalBase), MEMBER(m_duration));
};

//=============================================================================

/// A basic envelope, providing a duratin and constant amplitude
class SYNTACTS_API Envelope : public EnvelopeBase {
public:
    /// Constructs an Evelope with a specified duration and amplitude
    Envelope(float duration = 1.0f, float amplitude = 1.0f);
    /// Implements a time bounded envelope with specified amplitude
    virtual float sample(float t) const override;
private:
    std::atomic<float> m_amplitude;
private:
    SERIALIZE(PARENT(EnvelopeBase));
};

//=============================================================================

/// Envelope with time sequenced amplitudes and tweening functions
class SYNTACTS_API KeyedEnvelope : public EnvelopeBase {
public:

    typedef Ptr<Tween::Function> TweenFunc;

    /// Constucts Envelope with initial amplitude
    KeyedEnvelope(float amplitude0 = 0.0f);

    /// Adds a new amplitude at time t seconds. Uses tween to interpolate from previous amplitude.
    void addKey(float t, float amplitude, TweenFunc tween = create<Tween::Linear>());

    /// Gets the duration of all keys
    float getDuration() const override;

    virtual float sample(float t) const override;

protected:

    std::map<float, std::pair<float, TweenFunc>> m_keys;

private:
    SERIALIZE(PARENT(EnvelopeBase), MEMBER(m_keys));    
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
class SYNTACTS_API OscillatingEnvelope : public EnvelopeBase {
public:
    /// Constructs an Envelope with a specified duration, positive oscillator type and frequency
    OscillatingEnvelope(float duration = 1.0f , float amplitude = 1.0f, Ptr<OscillatorBase> osc = create<Sine>());
    virtual float sample(float t) const override;
protected:
    Ptr<OscillatorBase> m_osc;
    std::atomic<float> m_amplitude;
private:
    SERIALIZE(PARENT(EnvelopeBase), MEMBER(m_osc), MEMBER(m_amplitude));
};

} // namespace tact
