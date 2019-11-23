#pragma once

#include <Syntacts/Signal.hpp>
#include <vector>

namespace tact {

///////////////////////////////////////////////////////////////////////////////

/// A Signal formed by the sequencing of other Signals at various time points
class Sequence {
public:
    /// Default constructor
    Sequence();
    /// Inserts a Signal at the head position and then moves the head forward
    void insert(Signal signal);
    /// Inserts a Signal at position t in the Sequence but does NOT move head
    void insert(float t, Signal signal);
    /// Samples and sums all overlapping signals in the sequence at time t
    float sample(float t) const;
    /// Returns the length of the Sequence
    float length() const;
public:
    float head; ///< the current insertion head position/time
private:
    /// A Key in the Sequence
    struct Key { 
        float t; 
        Signal signal; 
        TACT_SERIALIZE(TACT_MEMBER(t), TACT_MEMBER(signal));
    };
    std::vector<Key> m_keys; ///< all keys
    float m_length;
private:
    TACT_SERIALIZE(TACT_MEMBER(head), TACT_MEMBER(m_keys), TACT_MEMBER(m_length));
};

///////////////////////////////////////////////////////////////////////////////

/// Inserts a Signal into the Sequence at the current insertion head
Sequence operator<<(Sequence lhs, Signal rhs);
/// Moves the insertion head of the Sequence by the specified amount
Sequence operator<<(Sequence lhs, float rhs);
/// Creates a new Sequence from two Signals
Sequence operator<<(Signal lhs, Signal rhs);
/// Creates a new Sequence from a Signal and moves the head forward
Sequence operator<<(Signal lhs, float rhs);
/// Creates a new Sequence, moves the head forward, then inserts a Signal
Sequence operator<<(float lhs, Signal rhs);

///////////////////////////////////////////////////////////////////////////////

} // namespace tact