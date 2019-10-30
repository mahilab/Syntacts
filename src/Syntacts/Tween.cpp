#include <Syntacts/Tween.hpp>
#include <cmath>

namespace tact {
namespace Tween {

namespace {

constexpr float PI     = 3.14159265358979f;
constexpr float HALFPI = PI * 0.5f;

inline float lerp(float a, float b, float t) {
    return a + (b - a) * t;
}

} // private namespace

float Instant::operator()(float a, float b, float t) {
    return b;
}

float Delayed::operator()(float a, float b, float t) {
    if (t == 1.0f)
        return b;
}


float Linear::operator()(float a, float b, float t) {
    return lerp(a,b,t);
}


float Smoothstep::operator()(float a, float b, float t) {
    t = t * t * (3.0f - 2.0f * t);
    return lerp(a, b, t);
}


float Smootherstep::operator()(float a, float b, float t) {
    t = t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
    return lerp(a, b, t);
}


float Smootheststep::operator()(float a, float b, float t) {
    t = t * t * t * t * (t * (t * (t * -20.0f + 70.0f) - 84.0f) + 35.0f);
    return lerp(a, b, t);
}

//==============================================================================
// POLYNOMIAL TWEENS
//==============================================================================

namespace Quadratic {

float In::operator()(float a, float b, float t) {
    t = t * t;
    return lerp(a, b, t);
}


float Out::operator()(float a, float b, float t) {
    t = t * (2.0f - t);
    return lerp(a, b, t);
}


float InOut::operator()(float a, float b, float t) {
    t *= 2.0f;
    if (t < 1.0f)
        t = 0.5f * t * t;
    else {
        t -= 1.0f;
        t = -0.5f * (t * (t - 2.0f) - 1.0f);
    }
    return lerp(a, b, t);
}

};  // namespace Quadratic

namespace Cubic {

float In::operator()(float a, float b, float t) {
    t = t * t * t;
    return lerp(a, b, t);
}


float Out::operator()(float a, float b, float t) {
    t -= 1.0f;
    t = (t * t * t + 1.0f);
    return lerp(a, b, t);
}


float InOut::operator()(float a, float b, float t) {
    t *= 2.0f;
    if (t < 1.0f)
        t = 0.5f * t * t * t;
    else {
        t -= 2.0f;
        t = 0.5f * (t * t * t + 2.0f);
    }
    return lerp(a, b, t);
}

};  // namespace Cubic

namespace Quartic {

float In::operator()(float a, float b, float t) {
    t = t * t * t * t;
    return lerp(a, b, t);
}


float Out::operator()(float a, float b, float t) {
    t -= 1.0f;
    t = -(t * t * t * t - 1.0f);
    return lerp(a, b, t);
}


float InOut::operator()(float a, float b, float t) {
    t *= 2.0f;
    if (t < 1.0f)
        t = 0.5f * t * t * t * t;
    else {
        t -= 2.0f;
        t = -0.5f * (t * t * t * t - 2.0f);
    }
    return lerp(a, b, t);
}

};  // namespace Quartic

namespace Quintic {

float In::operator()(float a, float b, float t) {
    t = t * t * t * t * t;
    return lerp(a, b, t);
}


float Out::operator()(float a, float b, float t) {
    t -= 1.0f;
    t = (t * t * t * t * t + 1.0f);
    return lerp(a, b, t);
}


float InOut::operator()(float a, float b, float t) {
    t *= 2.0f;
    if (t < 1.0f)
        t = 0.5f * t * t * t * t * t;
    else {
        t -= 2.0f;
        t = 0.5f * (t * t * t * t * t + 2.0f);
    }
    return lerp(a, b, t);
}

};  // namespace Quintic

//==============================================================================
// MATHEMATICAL TWEENS
//==============================================================================

namespace Sinusoidal {

float In::operator()(float a, float b, float t) {
    t = 1.0f - std::cos(t * HALFPI);
    return lerp(a, b, t);
}


float Out::operator()(float a, float b, float t) {
    t = std::sin(t * HALFPI);
    return lerp(a, b, t);
}


float InOut::operator()(float a, float b, float t) {
    t = -0.5f * (std::cos(PI * t) - 1.0f);
    return lerp(a, b, t);
}

};  // namespace Sinusoidal

namespace Exponential {

float In::operator()(float a, float b, float t) {
    t = std::pow(2.0f, 10.0f * (t - 1.0f));
    return lerp(a, b, t);
}


float Out::operator()(float a, float b, float t) {
    t = (-std::pow(2.0f, -10.0f * t) + 1.0f);
    return lerp(a, b, t);
}


float InOut::operator()(float a, float b, float t) {
    t *= 2.0f;
    if (t < 1.0f)
        t = 0.5f * std::pow(2.0f, 10.0f * (t - 1.0f));
    else {
        t -= 1.0f;
        t = 0.5f * (-std::pow(2.0f, -10.0f * t) + 2.0f);
    }
    return lerp(a, b, t);
}

};  // namespace Exponential

namespace Circular {

float In::operator()(float a, float b, float t) {
    t = (1.0f - std::sqrt(1.0f - t * t));
    return lerp(a, b, t);
}


float Out::operator()(float a, float b, float t) {
    t -= 1.0f;
    t = std::sqrt(1.0f - t * t);
    return lerp(a, b, t);
}


float InOut::operator()(float a, float b, float t) {
    t *= 2.0f;
    if (t < 1.0f)
        t = -0.5f * (std::sqrt(1.0f - t * t) - 1.0f);
    else {
        t -= 2.0f;
        t = 0.5f * (std::sqrt(1.0f - t * t) + 1.0f);
    }
    return lerp(a, b, t);
}

};  // namespace Circular

//==============================================================================
// EFFECT TWEENS
//==============================================================================

namespace Elastic {

float In::operator()(float a, float b, float t) {
    if (t == 0.0f) {
        return a;
    }
    if (t == 1.0f) {
        return b;
    }
    t -= 1.0f;
    t = (-std::pow(2.0f, 10.0f * t) * std::sin((t - 0.1f) * (2.0f * PI) * 2.5f));
    return lerp(a, b, t);
}


float Out::operator()(float a, float b, float t) {
    if (t == 0.0f) {
        return a;
    }
    if (t == 1.0f) {
        return b;
    }
    t =  (std::pow(2.0f, -10.0f * t) * std::sin((t - 0.1f) * (2.0f * PI) * 2.5f) + 1.0f);
    return lerp(a, b, t);
}


float InOut::operator()(float a, float b, float t) {
    t *= 2.0f;
    if (t < 1.0f) {
        t -= 1.0f;
        t = (-0.5f * std::pow(2.0f, 10.0f * t) * std::sin((t - 0.1f) * (2.0f * PI) * 2.5f));
    }
    else {
        t -= 1.0f;
        t = (std::pow(2.0f, -10.0f * t) * std::sin((t - 0.1f) * (2.0f * PI) * 2.5f) * 0.5f + 1.0f);
    }
    return lerp(a, b, t);
}

};  // namespace Elastic

namespace Back {
static float s  = 1.70158f;
static float s2 = 2.5949095f;


float In::operator()(float a, float b, float t) {
    t = (t * t * ((s + 1.0f) * t - s));
    return lerp(a, b, t);
}


float Out::operator()(float a, float b, float t) {
    t -= 1.0f;
    t = (t * t * ((s + 1.0f) * t + s) + 1.0f);
    return lerp(a, b, t);
}


float InOut::operator()(float a, float b, float t) {
    t *= 2.0f;
    if (t < 1.0f) {
        t = (0.5f * (t * t * ((s2 + 1.0f) * t - s2)));
    }
    else {
        t -= 2.0f;
        t = (0.5f * (t * t * ((s2 + 1.0f) * t + s2) + 2.0f));
    }
    return lerp(a, b, t);
}

};  // namespace Back

namespace Bounce {

namespace detail {
float Out(float t) {
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

float In(float t) {
    return 1.0f - Out(1.0f - t);
}

float InOut(float t) {
    if (t < 0.5f)
        return In(t * 2.0f) * 0.5f;
    return Out(t * 2.0f - 1.0f) * 0.5f + 0.5f;
}
}  // namespace detail


float In::operator()(float a, float b, float t) {
    t = detail::In(t);
    return lerp(a, b, t);
}


float Out::operator()(float a, float b, float t) {
    t = detail::Out(t);
    return lerp(a, b, t);
}


float InOut::operator()(float a, float b, float t) {
    t = detail::InOut(t);
    return lerp(a, b, t);
}

};  // namespace Bounce

} // namespace Tween
} // namespace Tact