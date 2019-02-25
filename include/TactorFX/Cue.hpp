#pragma once

#include <TactorFX/Oscillator.hpp>
#include <TactorFX/Envelope.hpp>
#include <vector>
#include <utility>

namespace tfx {

/// Encapsulates a cue to be played on a single channel
class Cue {
public:

    /// Default constructor
    Cue();

    /// Constructs a Cue with a Oscillator lasting a specified duration
    Cue(Ptr<Oscillator> osc, float duration);

    /// Constructs a Cue with a Oscillator and Envelope follower
    Cue(Ptr<Oscillator> osc, Ptr<Envelope> env);

    /// Constructs a Cue with a Oscillator, modulating Oscillator, and Envelope follower
    Cue(Ptr<Oscillator> osc, Ptr<Oscillator> mod, Ptr<Envelope> env);

    /// Virtual destructor
    virtual ~Cue();

    /// Adds an existing custom Generator to be processed
    void addGenerator(Ptr<Generator> gen);

    /// Makes and then adds a custon Generator to be processed
    template <typename T, typename ...Args>
    void addGenerator(Args ... args);

    /// Compounds all Generators to compute the next sample
    float nextSample();

private:

    std::vector<Ptr<Generator>> m_generators; ///< array of generators

};

} // namespace tfx