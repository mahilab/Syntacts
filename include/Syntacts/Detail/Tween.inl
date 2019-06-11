
#include <vector>
#include <algorithm>
// #include <../src/Syntacts/Helpers.hpp>

#define PI            3.14159265358979e+0f
#define HALFPI        1.57079632679e+0f

namespace Tween {

//==============================================================================
// BASIC TWEENS
//==============================================================================


inline float Instant(float a, float b, float t) {
    return b;
}


inline float Delayed(float a, float b, float t) {
    if (t == 1.0f)
        return b;
}


inline float Linear(float a, float b, float t) {
    return a + (b - a) * t;
}


inline std::vector<float> Linear(const std::vector<float>& a, const std::vector<float>& b, float t) {
    std::size_t n = std::min(a.size(), b.size());
    std::vector<float> out(n);
    for (std::size_t i = 0; i < n; ++i) {
        out[i] = Linear(a[i], b[i], t);
    }
    return out;
}


inline float Smoothstep(float a, float b, float t) {
    t = t * t * (3.0f - 2.0f * t);
    return Linear(a, b, t);
}


inline float Smootherstep(float a, float b, float t) {
    t = t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
    return Linear(a, b, t);
}


inline float Smootheststep(float a, float b, float t) {
    t = t * t * t * t * (t * (t * (t * -20.0f + 70.0f) - 84.0f) + 35.0f);
    return Linear(a, b, t);
}

//==============================================================================
// POLYNOMIAL TWEENS
//==============================================================================

namespace Quadratic {

inline float In(float a, float b, float t) {
    t = t * t;
    return Linear(a, b, t);
}


inline float Out(float a, float b, float t) {
    t = t * (2.0f - t);
    return Linear(a, b, t);
}


inline float InOut(float a, float b, float t) {
    t *= 2.0f;
    if (t < 1.0f)
        t = 0.5f * t * t;
    else {
        t -= 1.0f;
        t = -0.5f * (t * (t - 2.0f) - 1.0f);
    }
    return Linear(a, b, t);
}

};  // namespace Quadratic

namespace Cubic {

inline float In(float a, float b, float t) {
    t = t * t * t;
    return Linear(a, b, t);
}


inline float Out(float a, float b, float t) {
    t -= 1.0f;
    t = (t * t * t + 1.0f);
    return Linear(a, b, t);
}


inline float InOut(float a, float b, float t) {
    t *= 2.0f;
    if (t < 1.0f)
        t = 0.5f * t * t * t;
    else {
        t -= 2.0f;
        t = 0.5f * (t * t * t + 2.0f);
    }
    return Linear(a, b, t);
}

};  // namespace Cubic

namespace Quartic {

inline float In(float a, float b, float t) {
    t = t * t * t * t;
    return Linear(a, b, t);
}


inline float Out(float a, float b, float t) {
    t -= 1.0f;
    t = -(t * t * t * t - 1.0f);
    return Linear(a, b, t);
}


inline float InOut(float a, float b, float t) {
    t *= 2.0f;
    if (t < 1.0f)
        t = 0.5f * t * t * t * t;
    else {
        t -= 2.0f;
        t = -0.5f * (t * t * t * t - 2.0f);
    }
    return Linear(a, b, t);
}

};  // namespace Quartic

namespace Quintic {

inline float In(float a, float b, float t) {
    t = t * t * t * t * t;
    return Linear(a, b, t);
}


inline float Out(float a, float b, float t) {
    t -= 1.0f;
    t = (t * t * t * t * t + 1.0f);
    return Linear(a, b, t);
}


inline float InOut(float a, float b, float t) {
    t *= 2.0f;
    if (t < 1.0f)
        t = 0.5f * t * t * t * t * t;
    else {
        t -= 2.0f;
        t = 0.5f * (t * t * t * t * t + 2.0f);
    }
    return Linear(a, b, t);
}

};  // namespace Quintic

//==============================================================================
// MATHEMATICAL TWEENS
//==============================================================================

namespace Sinusoidal {

inline float In(float a, float b, float t) {
    t = 1.0f - std::cos(t * HALFPI);
    return Linear(a, b, t);
}


inline float Out(float a, float b, float t) {
    t = std::sin(t * HALFPI);
    return Linear(a, b, t);
}


inline float InOut(float a, float b, float t) {
    t = -0.5f * (std::cos(PI * t) - 1.0f);
    return Linear(a, b, t);
}

};  // namespace Sinusoidal

namespace Exponential {

inline float In(float a, float b, float t) {
    t = std::pow(2.0f, 10.0f * (t - 1.0f));
    return Linear(a, b, t);
}


inline float Out(float a, float b, float t) {
    t = (-std::pow(2.0f, -10.0f * t) + 1.0f);
    return Linear(a, b, t);
}


inline float InOut(float a, float b, float t) {
    t *= 2.0f;
    if (t < 1.0f)
        t = 0.5f * std::pow(2.0f, 10.0f * (t - 1.0f));
    else {
        t -= 1.0f;
        t = 0.5f * (-std::pow(2.0f, -10.0f * t) + 2.0f);
    }
    return Linear(a, b, t);
}

};  // namespace Exponential

namespace Circular {

inline float In(float a, float b, float t) {
    t = (1.0f - std::sqrt(1.0f - t * t));
    return Linear(a, b, t);
}


inline float Out(float a, float b, float t) {
    t -= 1.0f;
    t = std::sqrt(1.0f - t * t);
    return Linear(a, b, t);
}


inline float InOut(float a, float b, float t) {
    t *= 2.0f;
    if (t < 1.0f)
        t = -0.5f * (std::sqrt(1.0f - t * t) - 1.0f);
    else {
        t -= 2.0f;
        t = 0.5f * (std::sqrt(1.0f - t * t) + 1.0f);
    }
    return Linear(a, b, t);
}

};  // namespace Circular

//==============================================================================
// EFFECT TWEENS
//==============================================================================

namespace Elastic {

inline float In(float a, float b, float t) {
    if (t == 0.0f) {
        return a;
    }
    if (t == 1.0f) {
        return b;
    }
    t -= 1.0f;
    t = (-std::pow(2.0f, 10.0f * t) * std::sin((t - 0.1f) * (2.0f * PI) * 2.5f));
    return Linear(a, b, t);
}


inline float Out(float a, float b, float t) {
    if (t == 0.0f) {
        return a;
    }
    if (t == 1.0f) {
        return b;
    }
    t =  (std::pow(2.0f, -10.0f * t) * std::sin((t - 0.1f) * (2.0f * PI) * 2.5f) + 1.0f);
    return Linear(a, b, t);
}


inline float InOut(float a, float b, float t) {
    t *= 2.0f;
    if (t < 1.0f) {
        t -= 1.0f;
        t = (-0.5f * std::pow(2.0f, 10.0f * t) * std::sin((t - 0.1f) * (2.0f * PI) * 2.5f));
    }
    else {
        t -= 1.0f;
        t = (std::pow(2.0f, -10.0f * t) * std::sin((t - 0.1f) * (2.0f * PI) * 2.5f) * 0.5f + 1.0f);
    }
    return Linear(a, b, t);
}

};  // namespace Elastic

namespace Back {
static float s  = 1.70158f;
static float s2 = 2.5949095f;


inline float In(float a, float b, float t) {
    t = (t * t * ((s + 1.0f) * t - s));
    return Linear(a, b, t);
}


inline float Out(float a, float b, float t) {
    t -= 1.0f;
    t = (t * t * ((s + 1.0f) * t + s) + 1.0f);
    return Linear(a, b, t);
}


inline float InOut(float a, float b, float t) {
    t *= 2.0f;
    if (t < 1.0f) {
        t = (0.5f * (t * t * ((s2 + 1.0f) * t - s2)));
    }
    else {
        t -= 2.0f;
        t = (0.5f * (t * t * ((s2 + 1.0f) * t + s2) + 2.0f));
    }
    return Linear(a, b, t);
}

};  // namespace Back

namespace Bounce {

namespace detail {
inline float Out(float t) {
    if (t < (1.0f / 2.75f)) {
        return 7.5625f * t * t;
    } else if (t < (2.0f / 2.75f)) {
        t -= (1.5f / 2.75f);
        return 7.5625f * t * t + 0.75f;
    } else if (t < (2.5f / 2.75f)) {
        t -= (2.25f / 2.75f);
        return 7.5625f * t * t + 0.9375f;
    } else {
        t -= (2.625f / 2.75f);
        return 7.5625f * t * t + 0.984375f;
    }
}

inline float In(float t) {
    return 1.0f - Out(1.0f - t);
}

inline float InOut(float t) {
    if (t < 0.5f)
        return In(t * 2.0f) * 0.5f;
    return Out(t * 2.0f - 1.0f) * 0.5f + 0.5f;
}
}  // namespace detail


inline float In(float a, float b, float t) {
    t = detail::In(t);
    return Linear(a, b, t);
}


inline float Out(float a, float b, float t) {
    t = detail::Out(t);
    return Linear(a, b, t);
}


inline float InOut(float a, float b, float t) {
    t = detail::InOut(t);
    return Linear(a, b, t);
}

};  // namespace Bounce

}  // namespace Tween


