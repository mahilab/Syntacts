#pragma once

#include <Tact/Macros.hpp>
#include <memory>

namespace tact {

///////////////////////////////////////////////////////////////////////////////

/// Curve Type Erasure
class Curve {
public:
    /// Default constructor
    Curve();    
    /// Constructor
    template <typename T>
    Curve(T curve) : m_ptr(std::make_shared<Model<T>>(std::move(curve))) { }
    /// Transforms interpolant t in range [0,1] 
    float operator()(float t) const;
    /// Returns value in between a and b given interpolant t in range [0,1]
    float operator()(float a, float b, float t) const;    
public:
    struct Concept {
        Concept() = default;
        virtual ~Concept() = default;
        virtual float operator()(float t) const = 0;
        template <class Archive>
        void serialize(Archive& archive) {}
    };
    template <typename T>
    struct Model final : Concept {
        Model() = default;
        Model(T model) : m_model(std::move(model)) { }
        float operator()(float t) const override
        { return m_model(t); }
        T m_model;
        TACT_SERIALIZE(TACT_PARENT(Concept), TACT_MEMBER(m_model));
    };
private:
    std::shared_ptr<const Concept> m_ptr;
private:
    TACT_SERIALIZE(TACT_MEMBER(m_ptr));
};

///////////////////////////////////////////////////////////////////////////////

/// Collection of Curves available in Syntacts
namespace Curves {

    /// Returns b instantly regardless of t.
    TACT_CURVE(Instant);
    /// Returns a until t = 1.0, then returns b.
    TACT_CURVE(Delayed);
    /// Transitions from a to b using linear interpolation
    TACT_CURVE(Linear);
    /// Transitions from a to b using 3rd order Hermite interpolation.
    TACT_CURVE(Smoothstep);
    /// Transitions from a to b using 5th order Hermite interpolation.
    TACT_CURVE(Smootherstep);
    /// Transitions from a to b using 7th order Hermite interpolation.
    TACT_CURVE(Smootheststep);

    /// Transitions from a to b using 2nd order polynomial interpolation.
    namespace Quadratic {
        TACT_CURVE(In);
        TACT_CURVE(Out);
        TACT_CURVE(InOut);
    }

    /// Transitions from a to b using 3rd order polynomial interpolation.
    namespace Cubic {
        TACT_CURVE(In);
        TACT_CURVE(Out);
        TACT_CURVE(InOut);
    }

    /// Transitions from a to b using 4th order polynomial interpolation.
    namespace Quartic {
        TACT_CURVE(In);
        TACT_CURVE(Out);
        TACT_CURVE(InOut);
    }

    /// Transitions from a to b using 5th order polynomial interpolation.
    namespace Quintic {
        TACT_CURVE(In);
        TACT_CURVE(Out);
        TACT_CURVE(InOut);
    }

    /// Transitions from a to b using sinusoidal interpolation.
    namespace Sinusoidal {
        TACT_CURVE(In);
        TACT_CURVE(Out);
        TACT_CURVE(InOut);
    }

    /// Transitions from a to b using exponential interpolation.
    namespace Exponential {
        TACT_CURVE(In);
        TACT_CURVE(Out);
        TACT_CURVE(InOut);
    }

    /// Transitions from a to b using circular interpolation.
    namespace Circular {
        TACT_CURVE(In);
        TACT_CURVE(Out);
        TACT_CURVE(InOut);
    }

    /// Transitions from a to b with an elastic effect.
    namespace Elastic {
        TACT_CURVE(In);
        TACT_CURVE(Out);
        TACT_CURVE(InOut);
    }

    /// Transitions from a to b with an overshooting effect
    namespace Back {
        TACT_CURVE(In);
        TACT_CURVE(Out);
        TACT_CURVE(InOut);
    }

    /// Transitions from a to b with a bouncing effect.
    namespace Bounce {
        TACT_CURVE(In);
        TACT_CURVE(Out);
        TACT_CURVE(InOut);
    }

} // namespace Curves

///////////////////////////////////////////////////////////////////////////////

} // namespace tact