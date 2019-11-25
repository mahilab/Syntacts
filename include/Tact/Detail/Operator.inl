namespace tact
{

inline Signal operator+(Signal lhs, Signal rhs)
{
    return Sum(lhs, rhs);
}

inline Signal operator+(float lhs, Signal rhs)
{
    rhs.offset += lhs;
    return rhs;
}

inline Signal operator+(Signal lhs, float rhs)
{
    lhs.offset += rhs;
    return lhs;
}

inline Signal &operator+=(Signal &lhs, float rhs)
{
    lhs.offset += rhs;
    return lhs;
}

inline Signal operator-(Signal lhs, Signal rhs)
{
    return Sum(lhs, -1 * rhs);
}

inline Signal operator-(float lhs, Signal rhs)
{
    return lhs + -1 * rhs;
}

inline Signal operator-(Signal lhs, float rhs)
{
    lhs.offset -= rhs;
    return lhs;
}

inline Signal &operator-=(Signal &lhs, float rhs)
{
    lhs.offset -= rhs;
    return lhs;
}

inline Signal operator-(Signal lhs)
{
    return -1 * lhs;
}

inline Signal operator*(Signal lhs, Signal rhs)
{
    return Product(lhs, rhs);
}

inline Signal operator*(float lhs, Signal rhs)
{
    rhs.scale *= lhs;
    rhs.offset *= lhs;
    return rhs;
}

inline Signal operator*(Signal lhs, float rhs)
{
    lhs.scale *= rhs;
    lhs.offset *= rhs;
    return lhs;
}

inline Signal &operator*=(Signal &lhs, float rhs)
{
    lhs.scale *= rhs;
    lhs.offset *= rhs;
    return lhs;
}

} // namespace tact