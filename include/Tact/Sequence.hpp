#pragma once

#include <Tact/Signal.hpp>
#include <vector>

namespace tact {

///////////////////////////////////////////////////////////////////////////////

/// A Signal formed by the sequencing of other Signals at various time points
class Sequence {
public:
    /// Default constructor
    Sequence();
    /// Pushes a Signal at the head position and then moves the head forward
    void push(Signal signal);
    /// Pushes another Sequence at the head position and then moves the head forward
    void push(Sequence sequence);
    /// Inserts a Signal at position t in this Sequence but does NOT move head
    void insert(Signal signal, double t);
    /// Inserts another Sequence at position t in this Sequence but does NOT move head
    void insert(Sequence sequence, double t);

    /// Samples and sums all overlapping signals in the sequence at time t
    double sample(double t) const;
    /// Returns the length of the Sequence
    double length() const;

    /// Moves the insertion head of this Sequence by the specified amount
    inline Sequence& operator<<(double rhs);
    /// Pushes a Signal into this Sequence at the current insertion head
    inline Sequence& operator<<(Signal rhs);
    /// Pushes another Sequence into this Sequence at the current insertion head
    inline Sequence& operator<<(Sequence rhs);

public:
    double head; ///< the current insertion head position/time
private:
    /// A Key in the Sequence
    struct Key { 
        double t; 
        Signal signal; 
        TACT_SERIALIZE(TACT_MEMBER(t), TACT_MEMBER(signal));
    };
    std::vector<Key> m_keys; ///< all keys
    double m_length;
private:
    TACT_SERIALIZE(TACT_MEMBER(head), TACT_MEMBER(m_keys), TACT_MEMBER(m_length));
};

///////////////////////////////////////////////////////////////////////////////

/// Creates a new Sequence from two Signals
inline Sequence operator<<(Signal lhs, Signal rhs);
/// Creates a new Sequence from a Signal and moves the head forward
inline Sequence operator<<(Signal lhs, double rhs);
/// Creates a new Sequence, moves the head forward, then inserts a Signal
inline Sequence operator<<(double lhs, Signal rhs);

///////////////////////////////////////////////////////////////////////////////

} // namespace tact

#include <Tact/Detail/Sequence.inl>