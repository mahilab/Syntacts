#include <Syntacts/Scalar.hpp>

namespace tact {

Scalar::Scalar(float value) : m_value(value) { }

float Scalar::sample(float t) {
    return m_value;
}

} // namespace tact