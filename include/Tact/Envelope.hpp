#pragma once

#include <Tact/Curve.hpp>
#include <Tact/Oscillator.hpp>
#include <Tact/Signal.hpp>
#include <map>
#include <utility>

namespace tact
{

///////////////////////////////////////////////////////////////////////////////

/// A basic envelope, providing a duration and constant amplitude
class SYNTACTS_API Envelope
{
public:
    Envelope(float duration = 0.1f, float amplitude = 1.0f);
    float sample(float t) const;
    float length() const;

public:
    float duration;
    float amplitude;

private:
    TACT_SERIALIZE(TACT_MEMBER(duration), TACT_MEMBER(amplitude));
};

///////////////////////////////////////////////////////////////////////////////

/// Envelope with time sequenced amplitudes and curves
class SYNTACTS_API KeyedEnvelope
{
public:
    /// Constucts Envelope with initial amplitude
    KeyedEnvelope(float amplitude0 = 0.0f);
    /// Adds a new amplitude at time t seconds. Uses curve to interpolate from previous amplitude.
    void addKey(float t, float amplitude, Curve curve = Curves::Linear());
    float sample(float t) const;
    float length() const;
public:
    std::map<float, std::pair<float, Curve>> keys; ///< keys
private:
    TACT_SERIALIZE(TACT_MEMBER(keys));
};

///////////////////////////////////////////////////////////////////////////////

/// Attack-Sustain-Release Envelope
class SYNTACTS_API ASR : public KeyedEnvelope
{
public:
    /// Constructs ASR Envelope with specified attack, sustain, and release times
    ASR(float attackTime = 0.025f, float sustainTime = 0.05f,
        float releaseTime = 0.025f, float attackAmplitude = 1.0f,
        Curve attackCurve = Curves::Linear(),
        Curve releaseCurve = Curves::Linear());
private:
    TACT_SERIALIZE(TACT_PARENT(KeyedEnvelope));
};

///////////////////////////////////////////////////////////////////////////////

/// Attack-Decay-Sustain-Release Envelope
class SYNTACTS_API ADSR : public KeyedEnvelope
{
public:
    /// Constructs ASR Envelope with specified attack, sustain, and release times
    ADSR(float attackTime = 0.025f, float decayTime = 0.025f,
         float sustainTime = 0.025f, float releaseTime = 0.025f,
         float attackAmplitude = 1.0f, float decayAmplitude = 0.5f,
         Curve attackCurve = Curves::Linear(),
         Curve decayCurve = Curves::Linear(),
         Curve releaseCurve = Curves::Linear());

private:
    TACT_SERIALIZE(TACT_PARENT(KeyedEnvelope));
};

///////////////////////////////////////////////////////////////////////////////

/// An object which gives a Cue a length and/or shape
class SYNTACTS_API SignalEnvelope
{
public:
    /// Constructs an Envelope with a specified length, positive oscillator type and frequency
    SignalEnvelope(Signal signal = Sine(), float duration = 1.0f,
                   float amplitude = 1.0f);
    float sample(float t) const;
    float length() const;

public:
    Signal signal;
    float duration;
    float amplitude;

private:
    TACT_SERIALIZE(TACT_MEMBER(signal), TACT_MEMBER(duration),
                   TACT_MEMBER(amplitude));
};

} // namespace tact
