#pragma once

#include <Tact/Signal.hpp>

namespace tact {

///////////////////////////////////////////////////////////////////////////////

/// A Signal which is the result of operating on two other Signals
struct Operator {
    Operator() = default;
    Operator(Signal lhs, Signal rhs);
protected:
    Signal m_lhs, m_rhs;
private:
    TACT_SERIALIZE(TACT_MEMBER(m_lhs), TACT_MEMBER(m_rhs));
};

///////////////////////////////////////////////////////////////////////////////

/// A Signal which is the sum or difference of two other signals
struct Sum : public Operator {
    using Operator::Operator;
    float sample(float t) const;
    float length() const;
private:
    TACT_SERIALIZE(TACT_PARENT(Operator));
};

///////////////////////////////////////////////////////////////////////////////

/// A Signal which is the product of two other signals
struct Product : public Operator {
    using Operator::Operator;
    float sample(float t) const;
    float length() const;
private:
    TACT_SERIALIZE(TACT_PARENT(Operator));
};

///////////////////////////////////////////////////////////////////////////////

inline Signal operator+(Signal lhs, Signal rhs);
inline Signal operator+(float lhs, Signal rhs);
inline Signal operator+(Signal lhs, float rhs);
inline Signal& operator+=(Signal& lhs, float rhs);

inline Signal operator-(Signal lhs, Signal rhs);
inline Signal operator-(float lhs, Signal rhs);
inline Signal operator-(Signal lhs, float rhs);
inline Signal& operator-=(Signal& lhs, float rhs);
inline Signal operator-(Signal lhs);

inline Signal operator*(Signal lhs, Signal rhs);
inline Signal operator*(float lhs, Signal rhs);
inline Signal operator*(Signal lhs, float rhs);
inline Signal& operator*=(Signal& lhs, float rhs);

///////////////////////////////////////////////////////////////////////////////

} // namespace tact

#include <Tact/Detail/Operator.inl>
