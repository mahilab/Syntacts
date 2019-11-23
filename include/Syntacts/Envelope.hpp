#pragma once

#include <Syntacts/Signal.hpp>
#include <Syntacts/Curve.hpp>
#include <Syntacts/Oscillator.hpp>
#include <utility>
#include <map>

namespace tact
{

///////////////////////////////////////////////////////////////////////////////

/// Abstract base class for object which gives a Cue a duration and/or shape
class SYNTACTS_API IEnvelope : public Cloneable<Interface<IEnvelope>, ISignal> {
private:
    TACT_SERIALIZE(TACT_PARENT(ISignal));
};

///////////////////////////////////////////////////////////////////////////////

/// A basic envelope, providing a durations and constant amplitude
class SYNTACTS_API Envelope : public Cloneable<Envelope, IEnvelope> {
public:
    /// Constructs an Evelope with a specified duration and amplitude
    Envelope(float duration = 1.0f, float amplitude = 1.0f);
    /// Implements a time bounded envelope with specified amplitude
    virtual float sample(float t) const override;
public:
    float amplitude;
private:
    TACT_SERIALIZE(TACT_PARENT(IEnvelope), TACT_MEMBER(amplitude));
};

///////////////////////////////////////////////////////////////////////////////

/// Envelope with time sequenced amplitudes and curves
class SYNTACTS_API KeyedEnvelope : public Cloneable<KeyedEnvelope, IEnvelope> {
public:
    /// Constucts Envelope with initial amplitude
    KeyedEnvelope(float amplitude0 = 0.0f);
    /// Adds a new amplitude at time t seconds. Uses curve to interpolate from previous amplitude.
    void addKey(float t, float amplitude, Curve curve = Curves::Linear());
    virtual float sample(float t) const override;
private:
    std::map<float, std::pair<float, Curve>> m_keys;
private:
    TACT_SERIALIZE(TACT_PARENT(IEnvelope), TACT_MEMBER(m_keys));    
};

///////////////////////////////////////////////////////////////////////////////

/// Attack-Sustain-Release Envelope
class SYNTACTS_API ASR : public Cloneable<ASR, KeyedEnvelope> {
public:
    /// Constructs ASR Envelope with specified attack, sustain, and release times
    ASR(float attackTime = 1.0f, float sustainTime = 1.0f, float releaseTime = 1.0f, float attackAmlitude = 1.0f, 
        Curve attackCurve = Curves::Linear(), Curve releaseCurve = Curves::Linear());
private:
    TACT_SERIALIZE(TACT_PARENT(KeyedEnvelope));
};

///////////////////////////////////////////////////////////////////////////////

/// Attack-Decay-Sustain-Release Envelope
class SYNTACTS_API ADSR : public Cloneable<ADSR, KeyedEnvelope> {
public:
    /// Constructs ASR Envelope with specified attack, sustain, and release times
    ADSR(float attackTime = 1.0f, float decayTime = 1.0f, float sustainTime = 1.0f, float releaseTime = 1.0f, float attackAmplitude = 1.0f, float decayAplitude = 0.5f, 
         Curve attackCurve = Curves::Linear(), Curve decayCurve = Curves::Linear(), Curve releaseCurve = Curves::Linear());
private:
    TACT_SERIALIZE(TACT_PARENT(KeyedEnvelope));
};

///////////////////////////////////////////////////////////////////////////////

/// An object which gives a Cue a duration and/or shape
class SYNTACTS_API OscillatingEnvelope : public Cloneable<OscillatingEnvelope, Envelope> {
public:
    /// Constructs an Envelope with a specified duration, positive oscillator type and frequency
    OscillatingEnvelope(float duration = 1.0f , float amplitude = 1.0f, Ptr<IOscillator> osc = create<Sine>());
    virtual float sample(float t) const override;
protected:
    Ptr<IOscillator> m_osc;
private:
    TACT_SERIALIZE(TACT_PARENT(Envelope), TACT_MEMBER(m_osc));
};

} // namespace tact
