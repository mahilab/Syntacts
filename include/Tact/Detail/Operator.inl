#include <Tact/Operator.hpp>

namespace tact
{

inline Signal operator+(Signal lhs, Signal rhs)
{
    return Sum(std::move(lhs), std::move(rhs));
}

inline Signal operator+(double lhs, Signal rhs)
{
    rhs.bias += lhs;
    return rhs;
}

inline Signal operator+(Signal lhs, double rhs)
{
    lhs.bias += rhs;
    return lhs;
}

inline Signal &operator+=(Signal &lhs, double rhs)
{
    lhs.bias += rhs;
    return lhs;
}

inline Signal operator-(Signal lhs, Signal rhs)
{
    rhs *= -1;
    return Sum(std::move(lhs), std::move(rhs));
}

inline Signal operator-(double lhs, Signal rhs)
{
    return lhs + -1 * rhs;
}

inline Signal operator-(Signal lhs, double rhs)
{
    lhs.bias -= rhs;
    return lhs;
}

inline Signal &operator-=(Signal &lhs, double rhs)
{
    lhs.bias -= rhs;
    return lhs;
}

inline Signal operator-(Signal lhs)
{
    return -1 * lhs;
}

inline Signal operator*(Signal lhs, Signal rhs)
{
    return Product(std::move(lhs), std::move(rhs));
}

inline Signal operator*(double lhs, Signal rhs)
{
    rhs.gain  *= lhs;
    rhs.bias *= lhs;
    return rhs;
}

inline Signal operator*(Signal lhs, double rhs)
{
    lhs.gain  *= rhs;
    lhs.bias *= rhs;
    return lhs;
}

inline Signal &operator*=(Signal &lhs, double rhs)
{
    lhs.gain  *= rhs;
    lhs.bias *= rhs;
    return lhs;
}

} // namespace tact