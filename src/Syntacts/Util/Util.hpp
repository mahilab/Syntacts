#pragma once

#include <cmath>

constexpr float PI      = 3.14159265358979f;
constexpr float HALF_PI = 0.5f * PI;
constexpr float TWO_PI  = 2.0f * PI;

namespace tact {
namespace {

/// clampes value betweeen min and max
inline float clamp(float value, float min, float max) {
    return value <= min ? min : value >= max ? max : value;
}

/// Clampes value between 0 and 1
inline float clamp01(float value) {
    return value <= 0.0f ? 0.0f : value >= 1.0f ? 1.0f : value;
}

inline float remap(float x, float x0, float x1, float y0, float y1) {
    return y0 + (x - x0) * (y1 - y0) / (x1 - x0);
}

} // namespace anonymous   
} // namespace tact
