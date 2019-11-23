#include <Tact/Operator.hpp>
#include <algorithm>

namespace tact
{

Operator::Operator(Signal _lhs, Signal _rhs) :
    lhs(std::move(_lhs)), rhs(std::move(_rhs))
{ }

float Sum::sample(float t) const {
    return lhs.sample(t) + rhs.sample(t);
}

float Sum::length() const {
    return std::max(lhs.length(), rhs.length());
}

Sum operator+(const Signal& lhs, const Signal& rhs) {
    return Sum(lhs, rhs);
}

Sum operator+(float lhs, const Signal& rhs) {
    return Sum(Scalar(lhs), rhs);
}

Sum operator+(const Signal& lhs, float rhs) {
    return Sum(lhs, Scalar(rhs));
}


float Difference::sample(float t) const {
    return lhs.sample(t) - rhs.sample(t);
}

float Difference::length() const {
    return std::max(lhs.length(), rhs.length());
}

Difference operator-(const Signal& lhs, const Signal& rhs) {
    return Difference(lhs, rhs);
}

Difference operator-(float lhs, const Signal& rhs) {
    return Difference(Scalar(lhs), rhs);
}

Difference operator-(const Signal& lhs, float rhs) {
    return Difference(lhs, Scalar(rhs));
}


float Product::sample(float t) const {
    return lhs.sample(t) * rhs.sample(t);
}

float Product::length() const {
    return std::min(lhs.length(), rhs.length());
}

Product operator*(const Signal& lhs, const Signal& rhs) {
    return Product(lhs, rhs);
}

Product operator*(float lhs, const Signal& rhs) {
    return Product(Scalar(lhs), rhs);
}

Product operator*(const Signal& lhs, float rhs) {
    return Product(lhs, Scalar(rhs));
}


} // namespace tact
