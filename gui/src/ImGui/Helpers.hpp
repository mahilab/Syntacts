#include <cmath>
#include <string>
#include <sstream>
#include <iomanip>
#include <ImGui/imgui.h>

namespace ImGui {

inline int OrderOfMagnitude(float value) {
    if (value == 0.0f)
        return 0;
    else
        return static_cast<int>(floor(log10(std::abs(value))));
}

inline int Precision(int order) {
    int prec;
    if (order >= 1)
        prec = 0;
    else if (order == 0)
        prec = 1;
    else
        prec = -order + 1;
    return prec;
}

inline std::string NumToStr(float number) {
    int order = OrderOfMagnitude(number);
    int prec = Precision(order);
    std::stringstream stream;
    stream << std::fixed << std::setprecision(prec) << number;
    return stream.str();
}

inline float Remap(float x, float x0, float x1, float y0, float y1) {
    return y0 + (x - x0) * (y1 - y0) / (x1 - x0);
}

inline float RoundUpToNearest(float value, float interval)
{
    if (interval == 0)
        return value;
    float remainder = fmod(abs(value), interval);
    if (remainder == 0)
        return value;
    if (value < 0)
        return -(abs(value) - remainder);
    else
        return value + interval - remainder;
}

inline float RoundDownToNearest(float value, float interval) {
    if (interval == 0)
        return value;
    float remainder = fmod(abs(value), interval);
    if (remainder == 0)
        return value;
    if (value < 0)
        return -(abs(value) - remainder);
    else
        return value - remainder;
}

inline float RoundToNearest(float value, float interval) {
    if (value >= 0) {
        float rem = fmod(value, interval);
        value = rem > interval * 0.5f ? value + interval - rem : value - rem;
    }
    else {
        value = -value;
        float rem = fmod(value, interval);
        value = rem > interval * 0.5f ? value + interval - rem : value - rem;
        value = -value;
    }
    return value;
}

inline bool Approx(float a, float b, float delta = FLT_EPSILON) {
    return std::abs(a - b) < delta;
}

inline double NiceNum(double x, bool round)
{
    int expv;  /* exponent of x */
    double f;  /* fractional part of x */
    double nf; /* nice, rounded fraction */
    expv = floor(log10(x));
    f = x / std::pow(10, expv); /* between 1 and 10 */
    if (round)
        if (f < 1.5)
            nf = 1;
        else if (f < 3)
            nf = 2;
        else if (f < 7)
            nf = 5;
        else
            nf = 10;
    else if (f <= 1)
        nf = 1;
    else if (f <= 2)
        nf = 2;
    else if (f <= 5)
        nf = 5;
    else
        nf = 10;
    return nf * std::pow(10., expv);
}

}