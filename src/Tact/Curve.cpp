#include <Tact/Curve.hpp>
#include <Tact/Util.hpp>

namespace tact
{

Curve::Curve() : Curve(Curves::Linear()) {}

double Curve::operator()(double t) const
{
    return m_ptr->operator()(t);
}

double Curve::operator()(double a, double b, double t) const
{
    return lerp(a, b, m_ptr->operator()(t));
}

const char* Curve::name() const  {
    return m_ptr->name();
}

namespace Curves
{
double Instant::operator()(double t) const
{
    return 1;
}

double Delayed::operator()(double t) const
{
    if (t < 1)
        return 0;
    return 1;
}

double Linear::operator()(double t) const
{
    return t;
}

double Smoothstep::operator()(double t) const
{
    t = t * t * (3.0f - 2.0f * t);
    return t;
}

double Smootherstep::operator()(double t) const
{
    t = t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
    return t;
}

double Smootheststep::operator()(double t) const
{
    t = t * t * t * t * (t * (t * (t * -20.0f + 70.0f) - 84.0f) + 35.0f);
    return t;
}

///////////////////////////////////////////////////////////////////////////////=
// POLYNOMIAL TWEENS
///////////////////////////////////////////////////////////////////////////////=

namespace Quadratic
{

double In::operator()(double t) const
{
    t = t * t;
    return t;
}

double Out::operator()(double t) const
{
    t = t * (2.0f - t);
    return t;
}

double InOut::operator()(double t) const
{
    t *= 2.0f;
    if (t < 1.0f)
        t = 0.5f * t * t;
    else
    {
        t -= 1.0f;
        t = -0.5f * (t * (t - 2.0f) - 1.0f);
    }
    return t;
}

}; // namespace Quadratic

namespace Cubic
{

double In::operator()(double t) const
{
    t = t * t * t;
    return t;
}

double Out::operator()(double t) const
{
    t -= 1.0f;
    t = (t * t * t + 1.0f);
    return t;
}

double InOut::operator()(double t) const
{
    t *= 2.0f;
    if (t < 1.0f)
        t = 0.5f * t * t * t;
    else
    {
        t -= 2.0f;
        t = 0.5f * (t * t * t + 2.0f);
    }
    return t;
}

}; // namespace Cubic

namespace Quartic
{

double In::operator()(double t) const
{
    t = t * t * t * t;
    return t;
}

double Out::operator()(double t) const
{
    t -= 1.0f;
    t = -(t * t * t * t - 1.0f);
    return t;
}

double InOut::operator()(double t) const
{
    t *= 2.0f;
    if (t < 1.0f)
        t = 0.5f * t * t * t * t;
    else
    {
        t -= 2.0f;
        t = -0.5f * (t * t * t * t - 2.0f);
    }
    return t;
}

}; // namespace Quartic

namespace Quintic
{

double In::operator()(double t) const
{
    t = t * t * t * t * t;
    return t;
}

double Out::operator()(double t) const
{
    t -= 1.0f;
    t = (t * t * t * t * t + 1.0f);
    return t;
}

double InOut::operator()(double t) const
{
    t *= 2.0f;
    if (t < 1.0f)
        t = 0.5f * t * t * t * t * t;
    else
    {
        t -= 2.0f;
        t = 0.5f * (t * t * t * t * t + 2.0f);
    }
    return t;
}

}; // namespace Quintic

///////////////////////////////////////////////////////////////////////////////=
// MATHEMATICAL TWEENS
///////////////////////////////////////////////////////////////////////////////=

namespace Sinusoidal
{

double In::operator()(double t) const
{
    t = 1.0f - std::cos(t * HALF_PI);
    return t;
}

double Out::operator()(double t) const
{
    t = std::sin(t * HALF_PI);
    return t;
}

double InOut::operator()(double t) const
{
    t = -0.5f * (std::cos(PI * t) - 1.0f);
    return t;
}

}; // namespace Sinusoidal

namespace Exponential
{

double In::operator()(double t) const
{
    t = std::pow(2.0f, 10.0f * (t - 1.0f));
    return t;
}

double Out::operator()(double t) const
{
    t = (-std::pow(2.0f, -10.0f * t) + 1.0f);
    return t;
}

double InOut::operator()(double t) const
{
    t *= 2.0f;
    if (t < 1.0f)
        t = 0.5f * std::pow(2.0f, 10.0f * (t - 1.0f));
    else
    {
        t -= 1.0f;
        t = 0.5f * (-std::pow(2.0f, -10.0f * t) + 2.0f);
    }
    return t;
}

}; // namespace Exponential

namespace Circular
{

double In::operator()(double t) const
{
    t = (1.0f - std::sqrt(1.0f - t * t));
    return t;
}

double Out::operator()(double t) const
{
    t -= 1.0f;
    t = std::sqrt(1.0f - t * t);
    return t;
}

double InOut::operator()(double t) const
{
    t *= 2.0f;
    if (t < 1.0f)
        t = -0.5f * (std::sqrt(1.0f - t * t) - 1.0f);
    else
    {
        t -= 2.0f;
        t = 0.5f * (std::sqrt(1.0f - t * t) + 1.0f);
    }
    return t;
}

}; // namespace Circular

///////////////////////////////////////////////////////////////////////////////=
// EFFECT TWEENS
///////////////////////////////////////////////////////////////////////////////=

namespace Elastic
{

double In::operator()(double t) const
{
    if (t == 0 || t == 1)
        return t;
    t -= 1.0f;
    t = (-std::pow(2.0f, 10.0f * t) * std::sin((t - 0.1f) * (2.0f * PI) * 2.5f));
    return t;
}

double Out::operator()(double t) const
{
    if (t == 0 || t == 1)
        return t;
    return (std::pow(2.0f, -10.0f * t) * std::sin((t - 0.1f) * (2.0f * PI) * 2.5f) + 1.0f);
}

double InOut::operator()(double t) const
{
    t *= 2.0f;
    if (t < 1.0f)
    {
        t -= 1.0f;
        t = (-0.5f * std::pow(2.0f, 10.0f * t) * std::sin((t - 0.1f) * (2.0f * PI) * 2.5f));
    }
    else
    {
        t -= 1.0f;
        t = (std::pow(2.0f, -10.0f * t) * std::sin((t - 0.1f) * (2.0f * PI) * 2.5f) * 0.5f + 1.0f);
    }
    return t;
}

}; // namespace Elastic

namespace Back
{

constexpr double s = 1.70158f;
constexpr double s2 = 2.5949095f;

double In::operator()(double t) const
{
    t = (t * t * ((s + 1.0f) * t - s));
    return t;
}

double Out::operator()(double t) const
{
    t -= 1.0f;
    t = (t * t * ((s + 1.0f) * t + s) + 1.0f);
    return t;
}

double InOut::operator()(double t) const
{
    t *= 2.0f;
    if (t < 1.0f)
    {
        t = (0.5f * (t * t * ((s2 + 1.0f) * t - s2)));
    }
    else
    {
        t -= 2.0f;
        t = (0.5f * (t * t * ((s2 + 1.0f) * t + s2) + 2.0f));
    }
    return t;
}

}; // namespace Back

namespace Bounce
{

namespace detail
{
double Out(double t)
{
    if (t < (1.0f / 2.75f))
    {
        return 7.5625f * t * t;
    }
    else if (t < (2.0f / 2.75f))
    {
        t -= (1.5f / 2.75f);
        return 7.5625f * t * t + 0.75f;
    }
    else if (t < (2.5f / 2.75f))
    {
        t -= (2.25f / 2.75f);
        return 7.5625f * t * t + 0.9375f;
    }
    else
    {
        t -= (2.625f / 2.75f);
        return 7.5625f * t * t + 0.984375f;
    }
}

double In(double t)
{
    return 1.0f - Out(1.0f - t);
}

double InOut(double t)
{
    if (t < 0.5f)
        return In(t * 2.0f) * 0.5f;
    return Out(t * 2.0f - 1.0f) * 0.5f + 0.5f;
}
} // namespace detail

double In::operator()(double t) const
{
    return detail::In(t);
}

double Out::operator()(double t) const
{
    return detail::Out(t);
}

double InOut::operator()(double t) const
{
    return detail::InOut(t);
}

}; // namespace Bounce
} // namespace Curves

} // namespace tact
