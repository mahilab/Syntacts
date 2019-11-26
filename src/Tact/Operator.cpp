#include <Tact/Operator.hpp>
#include <algorithm>

namespace tact
{

IOperator::IOperator(Signal _lhs, Signal _rhs) :
    m_lhs(std::move(_lhs)), m_rhs(std::move(_rhs))
{ }

float Sum::sample(float t) const {
    return m_lhs.sample(t) + m_rhs.sample(t);
}

float Sum::length() const {
    return std::max(m_lhs.length(), m_rhs.length());
}

float Product::sample(float t) const {
    return m_lhs.sample(t) * m_rhs.sample(t);
}

float Product::length() const {
    return std::min(m_lhs.length(), m_rhs.length());
}

} // namespace tact
