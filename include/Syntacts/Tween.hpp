#pragma once

#include <functional>

namespace tact {

typedef std::function<float(float,float,float)> TweenFunc;

namespace Tween
{
    /// Returns b instantly regardless of t.
    inline float Instant(float a, float b, float t);
    /// Returns a until t = 1.0, then returns b.
    inline float Delayed(float a, float b, float t);
    /// Transitions from a to b using linear interpolation
    inline float Linear(float a, float b, float t);
    /// Transitions from a to b using 3rd order Hermite interpolation.
    inline float Smoothstep(float a, float b, float t);
    /// Transitions from a to b using 5th order Hermite interpolation.
    inline float Smootherstep(float a, float b, float t);
    /// Transitions from a to b using 7th order Hermite interpolation.
    inline float Smootheststep(float a, float b, float t);

    /// Transitions from a to b using 2nd order polynomial interpolation.
    namespace Quadratic
    {
        inline float In(float a, float b, float t);
        inline float Out(float a, float b, float t);
        inline float InOut(float a, float b, float t);
    }

    /// Transitions from a to b using 3rd order polynomial interpolation.
    namespace Cubic
    {
        inline float In(float a, float b, float t);
        inline float Out(float a, float b, float t);
        inline float InOut(float a, float b, float t);
    }

    /// Transitions from a to b using 4th order polynomial interpolation.
    namespace Quartic
    {
        inline float In(float a, float b, float t);
        inline float Out(float a, float b, float t);
        inline float InOut(float a, float b, float t);
    }

    /// Transitions from a to b using 5th order polynomial interpolation.
    namespace Quintic
    {
        inline float In(float a, float b, float t);
        inline float Out(float a, float b, float t);
        inline float InOut(float a, float b, float t);
    }

    /// Transitions from a to b using sinusoidal interpolation.
    namespace Sinusoidal
    {
        inline float In(float a, float b, float t);
        inline float Out(float a, float b, float t);
        inline float InOut(float a, float b, float t);
    }

    /// Transitions from a to b using exponential interpolation.
    namespace Exponential
    {
        inline float In(float a, float b, float t);
        inline float Out(float a, float b, float t);
        inline float InOut(float a, float b, float t);
    }

    /// Transitions from a to b using circular interpolation.
    namespace Circular
    {
        inline float In(float a, float b, float t);
        inline float Out(float a, float b, float t);
        inline float InOut(float a, float b, float t);
    }

    /// Transitions from a to b with an elastic effect.
    namespace Elastic
    {
        inline float In(float a, float b, float t);
        inline float Out(float a, float b, float t);
        inline float InOut(float a, float b, float t);
    }

    /// Transitions from a to b with an overshooting effect
    namespace Back
    {
        inline float In(float a, float b, float t);
        inline float Out(float a, float b, float t);
        inline float InOut(float a, float b, float t);
    }

    /// Transitions from a to b with a bouncing effect.
    namespace Bounce
    {
        inline float In(float a, float b, float t);
        inline float Out(float a, float b, float t);
        inline float InOut(float a, float b, float t);
    }

} // namespace Tween
} // namespace tact

#include "Detail/Tween.inl"
