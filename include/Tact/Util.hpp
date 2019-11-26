#pragma once

#include <cmath>
#include <limits>

namespace tact {

///////////////////////////////////////////////////////////////////////////////

constexpr float PI      = 3.14159265358979f;
constexpr float HALF_PI = 0.5f * PI;
constexpr float TWO_PI  = 2.0f * PI;
constexpr float INV_PI  = 1.0f / PI;
constexpr float INF     = std::numeric_limits<float>::infinity();
constexpr float EPS     = std::numeric_limits<float>::epsilon();

///////////////////////////////////////////////////////////////////////////////

/// Clampes value between min and max
inline float clamp(float value, float min, float max) {
    return value <= min ? min : value >= max ? max : value;
}

/// Clampes value between 0 and 1
inline float clamp01(float value) {
    return value <= 0.0f ? 0.0f : value >= 1.0f ? 1.0f : value;
}

/// Remaps x from range [x0,x1] to range [y0,y1]
inline float remap(float x, float x0, float x1, float y0, float y1) {
    return y0 + (x - x0) * (y1 - y0) / (x1 - x0);
}

/// Linearly interpolates from a to b given interpolant t in range [0,1]
inline float lerp(float a, float b, float t) {
    return a + (b - a) * t;
}

/// Sleeps the calling thread for seconds (accurate within a few milliseconds)
void sleep(double seconds, double max = 60);

/// Starts a  stopwatch timer. Consecutive calls will restart the timer.
void tic();

/// Returns the time in seconds since tic() was called.
double toc();

} // namespace tact
