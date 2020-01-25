#pragma once

#include <cmath>
#include <limits>
#include <string>

namespace tact {

///////////////////////////////////////////////////////////////////////////////

constexpr double PI      = 3.14159265358979323846;
constexpr double HALF_PI = 0.5 * PI;
constexpr double TWO_PI  = 2.0 * PI;
constexpr double INV_PI  = 1.0 / PI;
constexpr double INF     = std::numeric_limits<double>::infinity();
constexpr double EPS     = std::numeric_limits<double>::epsilon();

///////////////////////////////////////////////////////////////////////////////

/// Clampes value between min and max
inline double clamp(double value, double min, double max) {
    return value <= min ? min : value >= max ? max : value;
}

/// Clampes value between 0 and 1
inline double clamp01(double value) {
    return value <= 0.0 ? 0.0 : value >= 1.0 ? 1.0 : value;
}

/// Remaps x from range [x0,x1] to range [y0,y1]
inline double remap(double x, double x0, double x1, double y0, double y1) {
    return y0 + (x - x0) * (y1 - y0) / (x1 - x0);
}

/// Linearly interpolates from a to b given interpolant t in range [0,1]
inline double lerp(double a, double b, double t) {
    return a + (b - a) * t;
}

///////////////////////////////////////////////////////////////////////////////

/// Sleeps the calling thread for seconds (accurate within a few milliseconds)
void sleep(double seconds, double max = 60);

/// Starts a  stopwatch timer. Consecutive calls will restart the timer.
void tic();

/// Returns the time in seconds since tic() was called.
double toc();

///////////////////////////////////////////////////////////////////////////////

/// Returns the Syntacts version number (e.g. "1.0.0")
const std::string& syntactsVersion();

} // namespace tact
