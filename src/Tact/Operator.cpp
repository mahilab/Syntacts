#include <Tact/Operator.hpp>
#include <algorithm>

namespace tact
{

IOperator::IOperator(Signal _lhs, Signal _rhs) :
    lhs(std::move(_lhs)), rhs(std::move(_rhs))
{ }

float Sum::sample(float t) const {
    return lhs.sample(t) + rhs.sample(t);
}

float Sum::length() const {
    return std::max(lhs.length(), rhs.length());
}

float Product::sample(float t) const {
    return lhs.sample(t) * rhs.sample(t);
}

float Product::length() const {
    return std::min(lhs.length(), rhs.length());
}

} // namespace tact
