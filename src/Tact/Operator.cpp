#include <Tact/Operator.hpp>
#include <algorithm>

namespace tact
{

IOperator::IOperator(Signal _lhs, Signal _rhs) :
    lhs(std::move(_lhs)), rhs(std::move(_rhs))
{ }

double Sum::sample(double t) const {
    return lhs.sample(t) + rhs.sample(t);
}

double Sum::length() const {
    return std::max(lhs.length(), rhs.length());
}

double Product::sample(double t) const {
    return lhs.sample(t) * rhs.sample(t);
}

double Product::length() const {
    return std::min(lhs.length(), rhs.length());
}

} // namespace tact
