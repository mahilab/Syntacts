#include <Tact/Signal.hpp>

namespace tact
{

///////////////////////////////////////////////////////////////////////////////

/// A Signal which repeats another Signal for a number of repetitions
class SYNTACTS_API Repeater
{
public:
    Repeater();
    Repeater(Signal signal, int repetitions, double delay = 0);
    double sample(double t) const;
    double length() const;

public:
    Signal signal;
    int repetitions;
    double delay;

private:
    TACT_SERIALIZE(TACT_MEMBER(signal), TACT_MEMBER(repetitions), TACT_MEMBER(delay));
};

///////////////////////////////////////////////////////////////////////////////

/// A Signal which stretches or compresses another Signal temporally by a factor
class SYNTACTS_API Stretcher
{
public:
    Stretcher();
    Stretcher(Signal signal, double factor);
    double sample(double t) const;
    double length() const;

public:
    Signal signal;
    double factor;

private:
    TACT_SERIALIZE(TACT_MEMBER(signal), TACT_MEMBER(factor));
};

///////////////////////////////////////////////////////////////////////////////

} // namespace tact