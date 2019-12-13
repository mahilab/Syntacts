#pragma once

#include <Tact/Signal.hpp>

namespace tact {

///////////////////////////////////////////////////////////////////////////////

/// A signal which is the result of operating on two other Signals
struct IOperator {
    IOperator() = default;
    IOperator(Signal lhs, Signal rhs);
public:
    Signal lhs, rhs;
private:
    TACT_SERIALIZE(TACT_MEMBER(lhs), TACT_MEMBER(rhs));
};

///////////////////////////////////////////////////////////////////////////////

/// A Signal which is the sum or difference of two other signals
struct Sum : public IOperator {
    using IOperator::IOperator;
    float sample(float t) const;
    float length() const;
private:
    TACT_SERIALIZE(TACT_PARENT(IOperator));
};

///////////////////////////////////////////////////////////////////////////////

/// A Signal which is the product of two other signals
struct Product : public IOperator {
    using IOperator::IOperator;
    float sample(float t) const;
    float length() const;
private:
    TACT_SERIALIZE(TACT_PARENT(IOperator));
};

///////////////////////////////////////////////////////////////////////////////

inline Signal operator*(Signal lhs, Signal rhs);
inline Signal operator*(float lhs, Signal rhs);
inline Signal operator*(Signal lhs, float rhs);
inline Signal& operator*=(Signal& lhs, float rhs);

inline Signal operator+(Signal lhs, Signal rhs);
inline Signal operator+(float lhs, Signal rhs);
inline Signal operator+(Signal lhs, float rhs);
inline Signal& operator+=(Signal& lhs, float rhs);

inline Signal operator-(Signal lhs, Signal rhs);
inline Signal operator-(float lhs, Signal rhs);
inline Signal operator-(Signal lhs, float rhs);
inline Signal& operator-=(Signal& lhs, float rhs);

inline Signal operator-(Signal lhs);

///////////////////////////////////////////////////////////////////////////////

} // namespace tact

#include <Tact/Detail/Operator.inl>
