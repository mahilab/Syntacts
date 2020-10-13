#include <Tact/Process.hpp>

namespace tact
{

Repeater::Repeater() : repetitions(1),
                       delay(0)
{
}

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

Stretcher::Stretcher() : factor(1) {}

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

Reverser::Reverser()
{
}

Reverser::Reverser(Signal _signal) : signal(std::move(_signal))
{
}

double Reverser::sample(double t) const
{
    double l = signal.length();
    l = l == INF ? 1000000000 : l;
    t = clamp(l - t, 0, 1000000000);
    return signal.sample(t);
}

double Reverser::length() const
{
    return signal.length();
}


Filter::Filter() :
    Filter(Mode::LowPass,Signal(),Scalar(0.5),Scalar(0))
{ }


Filter::Filter(Mode mode, Signal input, double cutoff, double resonance) :
    Filter(mode, input, Scalar(cutoff), Scalar(resonance))
{ }

Filter::Filter(Mode _mode, Signal _input, Signal _cutoff, Signal _resonance) :
    mode(_mode),
    input(std::move(_input)),
    cutoff(std::move(_cutoff)),
    resonance(std::move(_resonance)),
    m_buff({})
{ }

double Filter::sample(double t) const {
    double in = input.sample(t);
    double cut = clamp(cutoff.sample(t), 0.000001, 0.999999);
    double res = resonance.sample(t);
    double feedback = res + res / (1.0 - cut);
    m_buff[0] += cut * (in - m_buff[0] + feedback * (m_buff[0] - m_buff[1]));
    m_buff[1] += cut * (m_buff[0] - m_buff[1]);
    m_buff[2] += cut * (m_buff[1] - m_buff[2]);
    m_buff[3] += cut * (m_buff[2] - m_buff[3]);
    switch (mode) {
        case Mode::LowPass:
            return m_buff[3];
        case Mode::HighPass:
            return in - m_buff[3];
        case Mode::BandPass:
            return m_buff[0] - m_buff[3];
        default:
            return 0.0;
    }
}

double Filter::length() const {
    return input.length();
}

void Filter::reset() {
    m_buff = {0,0,0,0};
}

} // namespace tact
