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
#include <vector>

namespace tact {

///////////////////////////////////////////////////////////////////////////////

/// A Signal formed by the sequencing of other Signals at various time points.
class Sequence {
public:

    /// A Key in the Sequence.
    struct Key { 
        double t; 
        Signal signal; 
        TACT_SERIALIZE(TACT_MEMBER(t), TACT_MEMBER(signal));
    };

    /// Default constructor.
    Sequence();
    /// Moves the insertion head forward/backward by t.
    Sequence& push(double t);
    /// Pushes a Signal at the head position and then moves the head forward.
    Sequence& push(Signal signal);
    /// Pushes another Sequence at the head position and then moves the head forward.
    Sequence& push(Sequence sequence);
    /// Inserts a Signal at position t in this Sequence but does NOT move head.
    Sequence& insert(Signal signal, double t);
    /// Inserts another Sequence at position t in this Sequence but does NOT move head.
    Sequence& insert(Sequence sequence, double t);
    /// Clears the Sequence
    void clear();

    /// Samples and sums all overlapping signals in the sequence at time t.
    double sample(double t) const;
    /// Returns the length of the Sequence.
    double length() const;

    /// Returns the number of keys in the sequence.
    int keyCount() const;
    /// Returns a key in the sequence.
    const Key& getKey(int idx) const;

    /// Moves the insertion head of this Sequence by the specified amount.
    inline Sequence& operator<<(double rhs);
    /// Pushes a Signal into this Sequence at the current insertion head.
    inline Sequence& operator<<(Signal rhs);
    /// Pushes another Sequence into this Sequence at the current insertion head.
    inline Sequence& operator<<(Sequence rhs);

public:
    double head; ///< the current insertion head position/time.
private:
    std::vector<Key> m_keys; ///< all keys
    double m_length;         ///< accumulated length
private:
    TACT_SERIALIZE(TACT_MEMBER(head), TACT_MEMBER(m_keys), TACT_MEMBER(m_length));
};

///////////////////////////////////////////////////////////////////////////////

/// Creates a new Sequence from two Signals.
inline Sequence operator<<(Signal lhs, Signal rhs);
/// Creates a new Sequence from a Signal and moves the head forward.
inline Sequence operator<<(Signal lhs, double rhs);
/// Creates a new Sequence, moves the head forward, then inserts a Signal.
inline Sequence operator<<(double lhs, Signal rhs);

///////////////////////////////////////////////////////////////////////////////

} // namespace tact

#include <Tact/Detail/Sequence.inl>