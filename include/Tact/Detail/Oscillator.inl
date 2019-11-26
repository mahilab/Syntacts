namespace tact {

float Oscillator::length() const {
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
    float phi = TWO_PI * (x.sample(t) + 0.5f * rate * t) * t;
    return std::sin(phi);
}

inline float PulseTrain::sample(float t) const {
    float percentWithinPeriod = std::fmod(t, period) * x.sample(t);
    if (percentWithinPeriod < dutyCycle) 
        return 1;
    else 
        return -1;
}

} // namespace tact