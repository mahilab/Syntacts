namespace tact {

float IOscillator::length() const {
    return INF;
}

inline float Sine::sample(float t) const {
    return std::sin(x.sample(t));
}

inline float Square::sample(float t) const {
    return std::sin(x.sample(t)) > 0 ? 1.0f : -1.0f;
}

inline float Saw::sample(float t) const {
    return 2 * INV_PI * (x.sample(t) * PI * std::fmod(t, 1.0f / x.sample(t)) - HALF_PI);
}

inline float Triangle::sample(float t) const {
    return 2 * INV_PI * std::asin(std::sin(x.sample(t)));
}

inline float SineFM::sample(float t) const {
    return std::sin(x.sample(t) + index * std::sin(TWO_PI * modulation * t));
}

inline float Chirp::sample(float t) const {
    float phi = x.sample(t) + PI * rate * t * t;
    return std::sin(phi);
}

inline float Pwm::sample(float t) const {
    return std::fmod(t, 1.0f / frequency) * frequency < dutyCycle ? 1.0f : -1.0f;
}

inline float Pwm::length() const {
    return INF;
}

inline float FM::sample(float t) const {
    return 0;
}

inline float FM::length() const {
    return INF;
}

} // namespace tact