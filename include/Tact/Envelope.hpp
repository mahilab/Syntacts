// MIT License
//
// Copyright (c) 2020 Mechatronics and Haptic Interfaces Lab
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// Author(s): Evan Pezent (epezent@rice.edu)

#pragma once

#include <Tact/Curve.hpp>
#include <Tact/Oscillator.hpp>
#include <Tact/Signal.hpp>
#include <map>
#include <utility>

namespace tact
{

///////////////////////////////////////////////////////////////////////////////

/// A basic envelope, providing a duration and constant amplitude.
class SYNTACTS_API Envelope
{
public:
    Envelope(double duration = 0.1, double amplitude = 1.0);
    double sample(double t) const;
    double length() const;

public:
    double duration;
    double amplitude;

private:
    TACT_SERIALIZE(TACT_MEMBER(duration), TACT_MEMBER(amplitude));
};

///////////////////////////////////////////////////////////////////////////////

/// Envelope with time sequenced amplitudes and curves.
class SYNTACTS_API KeyedEnvelope
{
public:
    /// Constucts Envelope with initial amplitude
    KeyedEnvelope(double amplitude0 = 0.0);
    /// Adds a new amplitude at time t seconds. Uses curve to interpolate from previous amplitude.
    void addKey(double t, double amplitude, Curve curve = Curves::Linear());
    double sample(double t) const;
    double length() const;
public:
    std::map<double, std::pair<double, Curve>> keys; ///< keys
private:
    TACT_SERIALIZE(TACT_MEMBER(keys));
};

///////////////////////////////////////////////////////////////////////////////

/// Attack-Sustain-Release Envelope.
class SYNTACTS_API ASR : public KeyedEnvelope
{
public:
    /// Constructs ASR Envelope with specified attack, sustain, and release times
    ASR(double attackTime = 0.025, double sustainTime = 0.05,
        double releaseTime = 0.025, double attackAmplitude = 1.0,
        Curve attackCurve = Curves::Linear(),
        Curve releaseCurve = Curves::Linear());
private:
    TACT_SERIALIZE(TACT_PARENT(KeyedEnvelope));
};

///////////////////////////////////////////////////////////////////////////////

/// Attack-Decay-Sustain-Release Envelope.
class SYNTACTS_API ADSR : public KeyedEnvelope
{
public:
    /// Constructs ASR Envelope with specified attack, sustain, and release times
    ADSR(double attackTime = 0.025, double decayTime = 0.025,
         double sustainTime = 0.025, double releaseTime = 0.025,
         double attackAmplitude = 1.0, double decayAmplitude = 0.5,
         Curve attackCurve = Curves::Linear(),
         Curve decayCurve = Curves::Linear(),
         Curve releaseCurve = Curves::Linear());

private:
    TACT_SERIALIZE(TACT_PARENT(KeyedEnvelope));
};

///////////////////////////////////////////////////////////////////////////////

/// Exponential decay according to the law y = A*e^(-Bt).
class SYNTACTS_API ExponentialDecay {
public:
    /// Default constructor
    ExponentialDecay(double amplitude = 1, double decay = 6.907755);
    double sample(double t) const;
    double length() const;
public:
    double amplitude;
    double decay;
private:
    TACT_SERIALIZE(TACT_MEMBER(amplitude), TACT_MEMBER(decay));
};

///////////////////////////////////////////////////////////////////////////////

/// Constructs an Envelope from a Signal.
class SYNTACTS_API SignalEnvelope
{
public:
    /// Constructs an Envelope with a specified length, positive oscillator type and frequency
    SignalEnvelope(Signal signal = Sine(), double duration = 1.0,
                   double amplitude = 1.0);
    double sample(double t) const;
    double length() const;

public:
    Signal signal;
    double duration;
    double amplitude;

private:
    TACT_SERIALIZE(TACT_MEMBER(signal), TACT_MEMBER(duration),
                   TACT_MEMBER(amplitude));
};

} // namespace tact
