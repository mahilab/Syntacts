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
    Filter(Mode::LowPass, Signal(), Scalar(1000))
{ }


Filter::Filter(Mode mode, Signal input, double cutoff, double fs,  double q, double gain) :
    Filter(mode, input, Scalar(cutoff), fs, q, gain)
{ }

Filter::Filter(Mode _mode, Signal _input, Signal _cutoff, double _fs, double _q, double _gain) :
    mode(_mode),
    input(std::move(_input)),
    cutoff(std::move(_cutoff)),
    fs(_fs),
    q(_q),
    gain(_gain)
{ 
    reset();
}

double Filter::sample(double t) const {
    updateCoefficients(cutoff.sample(t));
    const double in0 = input.sample(t);
    const double yn = b0a0 * in0 + b1a0 * in1 + b2a0 * in2 - a1a0 * ou1 - a2a0 * ou2;
    in2 = in1;
    in1 = in0;
    ou2 = ou1;
    ou1 = yn;
    return yn;
}

double Filter::length() const {
    return input.length();
}

void Filter::updateCoefficients(double fc) const {
    double A,alpha,sq,a0, a1, a2, b0, b1, b2;
    const double w0 = TWO_PI * fc / fs;
    const double sinw0 = std::sin(w0);
    const double cosw0 = std::cos(w0);
    switch (mode)
    {
    case LowPass:
    {
        alpha = sinw0 / (2.0 * q);
        b0 = (1.0 - cosw0) / 2.0;
        b1 = 1.0 - cosw0;
        b2 = (1.0 - cosw0) / 2.0;
        a0 = 1.0 + alpha;
        a1 = -2.0 * cosw0;
        a2 = 1.0 - alpha;
        break;
    }
    case HighPass:
    {
        alpha = sinw0 / (2.0 * q);
        b0 = (1.0 + cosw0) / 2.0;
        b1 = -(1.0 + cosw0);
        b2 = (1.0 + cosw0) / 2.0;
        a0 = 1.0 + alpha;
        a1 = -2.0 * cosw0;
        a2 = 1.0 - alpha;
        break;
    }
    case BandPass1:
    {
        // alpha = sinw0 * std::sinh(std::log(2.0) / 2.0 * q * w0 / sinw0);
        alpha = sinw0 / (2.0 * q);
        b0 = sinw0 / 2.0;
        b1 = 0.0;
        b2 = -sinw0 / 2;
        a0 = 1.0 + alpha;
        a1 = -2.0 * cosw0;
        a2 = 1.0 - alpha;
        break;
    }
    case BandPass2:
    {
        // alpha = sinw0 * std::sinh(std::log(2.0) / 2.0 * q * w0 / sinw0);
        alpha = sinw0 / (2.0 * q);
        b0 = alpha;
        b1 = 0.0;
        b2 = -alpha;
        a0 = 1.0 + alpha;
        a1 = -2.0 * cosw0;
        a2 = 1.0 - alpha;
        break;
    }
    case Notch:
    {
        // alpha = sinw0 * std::sinh(std::log(2.0) / 2.0 * q * w0 / sinw0);
        alpha = sinw0 / (2.0 * q);
        b0 = 1.0;
        b1 = -2.0 * cosw0;
        b2 = 1.0;
        a0 = 1.0 + alpha;
        a1 = -2.0 * cosw0;
        a2 = 1.0 - alpha;
        break;
    }
    case AllPass:
    {
        alpha = sinw0 / (2.0 * q);
        b0 = 1.0 - alpha;
        b1 = -2.0 * cosw0;
        b2 = 1.0 + alpha;
        a0 = 1.0 + alpha;
        a1 = -2.0 * cosw0;
        a2 = 1.0 - alpha;
        break;
    }
    case Peaking:
    {
        A = std::pow(10.0, (gain / 40.0));
        alpha = sinw0 * std::sinh(std::log(2.0) / 2.0 * q * w0 / sinw0);
        b0 = (1.0 + alpha * A);
        b1 = (-2.0 * cosw0);
        b2 = (1.0 - alpha * A);
        a0 = (1.0 + alpha / A);
        a1 = (-2.0 * cosw0);
        a2 = (1.0 - alpha / A);
        break;
    }
    case LowShelf:
    {
        A = std::pow(10.0, (gain / 40.0));
        alpha = sinw0 / 2 * std::sqrt((A+1.0/A)*(1.0/q-1) + 2);
        sq  = 2*std::sqrt(A)*alpha;
        b0 = A * ((A + 1.0) - (A - 1.0) * cosw0 + sq);
        b1 = 2.0 * A * ((A - 1.0) - (A + 1.0) * cosw0);
        b2 = A * ((A + 1.0) - (A - 1.0) * cosw0 - sq);
        a0 = (A + 1.0) + (A - 1.0) * cosw0 + sq;
        a1 = -2.0 * ((A - 1.0) + (A + 1.0) * cosw0);
        a2 = (A + 1.0) + (A - 1.0) * cosw0 - sq;
        break;
    }
    case HighShelf:
    {
        A = std::pow(10.0, (gain / 40.0));
        alpha = sinw0 / 2 * std::sqrt((A+1.0/A)*(1.0/q-1) + 2);
        sq  = 2*std::sqrt(A)*alpha;
        b0 = A * ((A + 1.0) + (A - 1.0) * cosw0 + sq);
        b1 = -2.0 * A * ((A - 1.0) + (A + 1.0) * cosw0);
        b2 = A * ((A + 1.0) + (A - 1.0) * cosw0 - sq);
        a0 = (A + 1.0) - (A - 1.0) * cosw0 + sq;
        a1 = 2.0 * ((A - 1.0) - (A + 1.0) * cosw0);
        a2 = (A + 1.0) - (A - 1.0) * cosw0 - sq;
        break;
    }
    default:
    {
        break;
    }
    }
    // set filter coefficients
    b0a0 = b0 / a0;
    b1a0 = b1 / a0;
    b2a0 = b2 / a0;
    a1a0 = a1 / a0;
    a2a0 = a2 / a0;
}

void Filter::reset() {
    // reset filter coefficients
    b0a0=b1a0=b2a0=a1a0=a2a0=0.0;  
    // reset in/out history
    ou1=ou2=in1=in2=0.0f; 
}

} // namespace tact
