#include <Tact/Oscillator.hpp>

namespace tact {

double IOscillator::length() const {
    return INF;
}

inline double Sine::sample(double t) const {
    return std::sin(x.sample(t));
}

inline double Square::sample(double t) const {
    return std::sin(x.sample(t)) > 0 ? 1.0 : -1.0;
}

inline double Saw::sample(double t) const {
    return -2 * INV_PI * std::atan(std::cos(0.5 * x.sample(t)) / std::sin(0.5 * x.sample(t)));
}

inline double Triangle::sample(double t) const {
    return 2 * INV_PI * std::asin(std::sin(x.sample(t)));
}

inline double Chirp::sample(double t) const {
    double phi = x.sample(t) + PI * rate * t * t;
    return std::sin(phi);
}

inline double Pwm::sample(double t) const {
    return std::fmod(t, 1.0 / frequency) * frequency < dutyCycle ? 1.0 : -1.0;
}

inline double Pwm::length() const {
    return INF;
}

} // namespace tact