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

#include <Tact/Signal.hpp>
#include <array>

namespace tact
{

///////////////////////////////////////////////////////////////////////////////

/// A Signal which repeats another Signal for a number of repetitions.
class SYNTACTS_API Repeater
{
public:
    Repeater();
    Repeater(Signal signal, int repetitions, double delay = 0);
    double sample(double t) const;
    double length() const;

public:
    Signal signal;
    int repetitions;
    double delay;

private:
    TACT_SERIALIZE(TACT_MEMBER(signal), TACT_MEMBER(repetitions), TACT_MEMBER(delay));
};

///////////////////////////////////////////////////////////////////////////////

/// A Signal which stretches or compresses another Signal temporally by a factor.
class SYNTACTS_API Stretcher
{
public:
    Stretcher();
    Stretcher(Signal signal, double factor);
    double sample(double t) const;
    double length() const;

public:
    Signal signal;
    double factor;

private:
    TACT_SERIALIZE(TACT_MEMBER(signal), TACT_MEMBER(factor));
};

///////////////////////////////////////////////////////////////////////////////

/// A Signal which reverses the input Signal.
class SYNTACTS_API Reverser {
public:
    Reverser();
    Reverser(Signal signal);
    double sample(double t) const;
    double length() const;
public:
    Signal signal;
private:
    TACT_SERIALIZE(TACT_MEMBER(signal));
};

///////////////////////////////////////////////////////////////////////////////

/// RBJ Biquad Filter
class SYNTACTS_API Filter {
public:

    enum Mode : int {
        LowPass   = 0, 
        HighPass  = 1,
        BandPass1 = 2,
        BandPass2 = 3,
        Notch     = 4,
        AllPass   = 5,
        Peaking   = 6,
        LowShelf  = 7,
        HighShelf = 8
    };

    Filter();
    Filter(Mode mode, Signal input, double cutoff, double fs = 44100, double q = 1, double gain = 0);
    Filter(Mode mode, Signal input, Signal cutoff, double fs = 44100, double q = 1, double gain = 0);

    double sample(double t) const;
    double length() const;
    void reset();

    Mode   mode;
    Signal input;
    Signal cutoff;
    double fs;
    double q;
    double gain;

private:
    void updateCoefficients(double frequency) const;
    TACT_SERIALIZE(TACT_MEMBER(mode), TACT_MEMBER(input), TACT_MEMBER(cutoff), TACT_MEMBER(q));
private:
    mutable double b0a0,b1a0,b2a0,a1a0,a2a0;
    mutable double ou1,ou2,in1,in2;
};

} // namespace tact