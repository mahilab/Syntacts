#pragma once

#include <Syntacts/Config.hpp>
#include <Syntacts/Oscillator.hpp>
#include <Syntacts/Envelope.hpp>
#include <vector>
#include <utility>

namespace tact {

/// Encapsulates a cue to be played on a single channel
class SYNTACTS_API Cue {
public:

    /// Default constructor
    Cue();

    /// Constructs a Cue with a Oscillator lasting a specified duration
    Cue(Ptr<Oscillator> osc, float duration, float amplitude = 1.0f);

    /// Constructs a Cue with a Oscillator and Envelope follower
    Cue(Ptr<Oscillator> osc, Ptr<Envelope> env);

    /// Constructs a Cue with a Oscillator, modulating Oscillator, and Envelope follower
    Cue(Ptr<Oscillator> osc, Ptr<Oscillator> mod, Ptr<Envelope> env);

    /// Virtual destructor
    virtual ~Cue();

    /// Sets the primary Envelope of a Cue
    void setEnvelope(Ptr<Envelope> env);

    /// Gets the primary Envelope of a Cue
    Ptr<Envelope> getEnvelope() const;

    /// Chains an existing custom Generator to be processed
    void chain(Ptr<Source> gen);

    /// Makes and then chains a custon Generator to be processed
    template <typename T, typename ...Args>
    void chain(Args ... args);

    /// Compounds all Generators to compute a sample at time t
    float sample(float t);

    /// Returns the number of samples this Cue generates for a particular sample rate
    int sampleCount(int sampleRate);

private:

    std::vector<Ptr<Source>> m_generators; ///< array of generators
    Ptr<Envelope> m_env;                      ///< the Cue's primary envelope

    SERIALIZE(MEMBER(m_generators), MEMBER(m_env))
};

template <typename T, typename ...Args>
void Cue::chain(Args... args) {
    auto g = create<T>(std::forward<Args>(args)...);
    chain(std::move(g));
}

} // namespace tact