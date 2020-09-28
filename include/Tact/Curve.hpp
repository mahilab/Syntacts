// MIT License
//
// Copyright (c) 2020 Mechatronics and Haptic Interfaces Lab
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// Author(s): Evan Pezent (epezent@rice.edu)

#pragma once

#include <Tact/Serialization.hpp>
#include <memory>

#define TACT_CURVE(T) struct T { \
                          double operator()(double t) const; \
                          const char* name() const { return #T; } \
                          template <class Archive> void serialize(Archive& archive) {} \
                      };

#define TACT_CURVE_N(N,T) struct T { \
                              double operator()(double t) const; \
                              const char* name() const { return #N"::"#T; } \
                              template <class Archive> void serialize(Archive& archive) {} \
                          };

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
    double operator()(double t) const;
    /// Returns value in between a and b given interpolant t in range [0,1]
    double operator()(double a, double b, double t) const;
    /// Returns curve name
    const char* name() const;    
public:
    struct Concept {
        Concept() = default;
        virtual ~Concept() = default;
        virtual double operator()(double t) const = 0;
        virtual const char* name() const = 0;
        template <class Archive>
        void serialize(Archive& archive) {}
    };
    template <typename T>
    struct Model final : Concept {
        Model() = default;
        Model(T model) : m_model(std::move(model)) { }
        double operator()(double t) const override
        { return m_model(t); }
        const char* name() const override
        { return m_model.name(); }
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
        TACT_CURVE_N(Quadratic, In);
        TACT_CURVE_N(Quadratic, Out);
        TACT_CURVE_N(Quadratic, InOut);
    }

    /// Transitions from a to b using 3rd order polynomial interpolation.
    namespace Cubic {
        TACT_CURVE_N(Cubic, In);
        TACT_CURVE_N(Cubic, Out);
        TACT_CURVE_N(Cubic, InOut);
    }

    /// Transitions from a to b using 4th order polynomial interpolation.
    namespace Quartic {
        TACT_CURVE_N(Quartic, In);
        TACT_CURVE_N(Quartic, Out);
        TACT_CURVE_N(Quartic, InOut);
    }

    /// Transitions from a to b using 5th order polynomial interpolation.
    namespace Quintic {
        TACT_CURVE_N(Quintic, In);
        TACT_CURVE_N(Quintic, Out);
        TACT_CURVE_N(Quintic, InOut);
    }

    /// Transitions from a to b using sinusoidal interpolation.
    namespace Sinusoidal {
        TACT_CURVE_N(Sinusoidal, In);
        TACT_CURVE_N(Sinusoidal, Out);
        TACT_CURVE_N(Sinusoidal, InOut);
    }

    /// Transitions from a to b using exponential interpolation.
    namespace Exponential {
        TACT_CURVE_N(Exponential, In);
        TACT_CURVE_N(Exponential, Out);
        TACT_CURVE_N(Exponential, InOut);
    }

    /// Transitions from a to b using circular interpolation.
    namespace Circular {
        TACT_CURVE_N(Circular, In);
        TACT_CURVE_N(Circular, Out);
        TACT_CURVE_N(Circular, InOut);
    }

    /// Transitions from a to b with an elastic effect.
    namespace Elastic {
        TACT_CURVE_N(Elastic, In);
        TACT_CURVE_N(Elastic, Out);
        TACT_CURVE_N(Elastic, InOut);
    }

    /// Transitions from a to b with an overshooting effect
    namespace Back {
        TACT_CURVE_N(Back, In);
        TACT_CURVE_N(Back, Out);
        TACT_CURVE_N(Back, InOut);
    }

    /// Transitions from a to b with a bouncing effect.
    namespace Bounce {
        TACT_CURVE_N(Bounce, In);
        TACT_CURVE_N(Bounce, Out);
        TACT_CURVE_N(Bounce, InOut);
    }

} // namespace Curves

///////////////////////////////////////////////////////////////////////////////

} // namespace tact