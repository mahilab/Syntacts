#pragma once

namespace tact {

namespace Tween
{
    /// Prototype for a function which interpolate from a to b given t between 0 and 1
    struct Functor {
        virtual float operator()(float a, float b, float t) = 0;
    };

    /// Macro to quickly define a new tweening functor
    #define TWEEN(T) struct T : public Functor { \
                         virtual float operator()(float a, float b, float t) override; \
                     };

    /// Returns b instantly regardless of t.
    TWEEN(Instant);
    /// Returns a until t = 1.0, then returns b.
    TWEEN(Delayed);
    /// Transitions from a to b using linear interpolation
    TWEEN(Linear);
    /// Transitions from a to b using 3rd order Hermite interpolation.
    TWEEN(Smoothstep);
    /// Transitions from a to b using 5th order Hermite interpolation.
    TWEEN(Smootherstep);
    /// Transitions from a to b using 7th order Hermite interpolation.
    TWEEN(Smootheststep);

    /// Transitions from a to b using 2nd order polynomial interpolation.
    namespace Quadratic {
        TWEEN(In);
        TWEEN(Out);
        TWEEN(InOut);
    }

    /// Transitions from a to b using 3rd order polynomial interpolation.
    namespace Cubic {
        TWEEN(In);
        TWEEN(Out);
        TWEEN(InOut);
    }

    /// Transitions from a to b using 4th order polynomial interpolation.
    namespace Quartic {
        TWEEN(In);
        TWEEN(Out);
        TWEEN(InOut);
    }

    /// Transitions from a to b using 5th order polynomial interpolation.
    namespace Quintic {
        TWEEN(In);
        TWEEN(Out);
        TWEEN(InOut);
    }

    /// Transitions from a to b using sinusoidal interpolation.
    namespace Sinusoidal {
        TWEEN(In);
        TWEEN(Out);
        TWEEN(InOut);
    }

    /// Transitions from a to b using exponential interpolation.
    namespace Exponential {
        TWEEN(In);
        TWEEN(Out);
        TWEEN(InOut);
    }

    /// Transitions from a to b using circular interpolation.
    namespace Circular {
        TWEEN(In);
        TWEEN(Out);
        TWEEN(InOut);
    }

    /// Transitions from a to b with an elastic effect.
    namespace Elastic {
        TWEEN(In);
        TWEEN(Out);
        TWEEN(InOut);
    }

    /// Transitions from a to b with an overshooting effect
    namespace Back {
        TWEEN(In);
        TWEEN(Out);
        TWEEN(InOut);
    }

    /// Transitions from a to b with a bouncing effect.
    namespace Bounce {
        TWEEN(In);
        TWEEN(Out);
        TWEEN(InOut);
    }

} // namespace Tween
} // namespace tact

// #include "Detail/Tween.inl"
