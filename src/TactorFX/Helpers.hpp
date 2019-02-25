#pragma once

#include <cmath>

#define PI            3.14159265358979e+0f
#define TWO_PI        6.28318530717959e+0f
#define SAMPLE_LENGTH 2.26757369614512e-5f


namespace tfx
{

/// clampes value betweeen min and max
inline float clamp(float value, float min, float max) {
    return value <= min ? min : value >= max ? max : value;
}

/// Clampes value between 0 and 1
inline float clamp01(float value) {
    return value <= 0.0f ? 0.0f : value >= 1.0f ? 1.0f : value;
}
    
} // namespace tfx
