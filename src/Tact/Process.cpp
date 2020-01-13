#include <Tact/Process.hpp>

namespace tact
{
    
Repeater::Repeater() :
    repetitions(1),
    delay(0)
{ }    

Repeater::Repeater(Signal _signal, int _repetitions, double _delay) : signal(std::move(_signal)),
                                                                      repetitions(_repetitions),
                                                                      delay(_delay)
{
}

double Repeater::sample(double t) const
{
    double sigLen = signal.length();
    double intLen = sigLen + delay;
    double maxLen = sigLen * repetitions + delay * (repetitions - 1);
    if (t <= maxLen)
    {
        double s = std::fmod(t, intLen);
        if (s <= sigLen)
        {
            return signal.sample(s);
        }
        return 0;
    }
    return 0;
}

double Repeater::length() const
{
    return signal.length() * repetitions + delay * (repetitions - 1);
}

Stretcher::Stretcher() : factor(1) { }

Stretcher::Stretcher(Signal _signal, double _factor) : signal(_signal),
                                                       factor(_factor)
{
}

double Stretcher::sample(double t) const
{
    return signal.sample(t / factor);
}

double Stretcher::length() const
{
    return signal.length() * factor;
}

} // namespace tact
