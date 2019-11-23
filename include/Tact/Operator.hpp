#pragma once

#include <Tact/Signal.hpp>

namespace tact {

struct Operator {
    Operator() = default;
    Operator(Signal lhs, Signal rhs);
    Signal lhs, rhs;
    TACT_SERIALIZE(TACT_MEMBER(lhs), TACT_MEMBER(rhs));
};

///////////////////////////////////////////////////////////////////////////////

struct Sum : public Operator {
    using Operator::Operator;
    float sample(float t) const;
    float length() const;
    TACT_SERIALIZE(TACT_PARENT(Operator));
};

Sum operator+(const Signal& lhs, const Signal& rhs);
Sum operator+(float lhs, const Signal& rhs);
Sum operator+(const Signal& lhs, float rhs);

///////////////////////////////////////////////////////////////////////////////

struct Difference : public Operator {
    using Operator::Operator;
    float sample(float t) const;
    float length() const;
    TACT_SERIALIZE(TACT_PARENT(Operator));
};

Difference operator-(const Signal& lhs, const Signal& rhs);
Difference operator-(float lhs, const Signal& rhs);
Difference operator-(const Signal& lhs, float rhs);

///////////////////////////////////////////////////////////////////////////////

struct Product : public Operator {
    using Operator::Operator;
    float sample(float t) const;
    float length() const;
    TACT_SERIALIZE(TACT_PARENT(Operator));
};

Product operator*(const Signal& lhs, const Signal& rhs);
Product operator*(float lhs, const Signal& rhs);
Product operator*(const Signal& lhs, float rhs);

///////////////////////////////////////////////////////////////////////////////

} // namespace tact