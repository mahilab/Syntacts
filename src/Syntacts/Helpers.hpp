#pragma once

#include <cmath>

#define PI            3.14159265358979e+0f
#define TWO_PI        6.28318530717959e+0f
#define HALFPI        1.57079632679e+0f
#define SAMPLE_LENGTH 2.26757369614512e-5f

namespace syntacts {
namespace {

/// clampes value betweeen min and max
inline float clamp(float value, float min, float max) {
    return value <= min ? min : value >= max ? max : value;
}

/// Clampes value between 0 and 1
inline float clamp01(float value) {
    return value <= 0.0f ? 0.0f : value >= 1.0f ? 1.0f : value;
}

inline float interp(float x, float x0, float x1, float y0, float y1) {
    return y0 + (x - x0) * (y1 - y0) / (x1 - x0);
}

} // namespace anonymous   
} // namespace syntacts
