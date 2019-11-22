#pragma once

#include <Syntacts/Config.hpp>
#include <Syntacts/Oscillator.hpp>
#include <Syntacts/Envelope.hpp>
#include <Syntacts/Operator.hpp>
#include <vector>

namespace tact {

/// Encapsulates a cue to be played on a single channel
class SYNTACTS_API Cue : public Cloneable<Cue, ISignal> {
public:

    /// Default constructor
    Cue();

    /// Constructs a Cue with a Oscillator lasting a specified duration
    Cue(Ptr<IOscillator> osc, float duration, float amplitude = 1.0f);

    /// Constructs a Cue with a Oscillator and Envelope follower
    Cue(Ptr<IOscillator> osc, Ptr<IEnvelope> env);

    /// Constructs a Cue with a Oscillator, modulating Oscillator, and Envelope follower
    Cue(Ptr<IOscillator> osc, Ptr<IOscillator> mod, Ptr<IEnvelope> env);

    /// Virtual destructor
    virtual ~Cue();

    /// Sets the primary Envelope of a Cue
    void setEnvelope(Ptr<IEnvelope> env);

    /// Sets the primary Envelope of a Cue to duration seconds long
    void setEnvelope(float duration, float amplitude = 1.0f);

    /// Gets the primary Envelope of a Cue
    Ptr<IEnvelope> getEnvelope() const;

    /// Chains an existing custom Signal to be processed
    void chain(Ptr<ISignal> sig);

    /// Makes and then chains a custon Signal to be processed
    template <typename T, typename ...Args>
    void chain(Args ... args);

    /// Compounds all Signals to compute a sample at time t
    virtual float sample(float t) const override;

    /// Returns the number of samples this Cue generates for a particular sample rate
    int sampleCount(int sampleRate) const;

private:
    std::vector<Ptr<ISignal>> m_sigs; ///< array of Signals
    Ptr<IEnvelope> m_env;             ///< the Cue's primary envelope
private:
    TACT_SERIALIZE(TACT_PARENT(ISignal), TACT_MEMBER(m_sigs), TACT_MEMBER(m_env))
};

template <typename T, typename ...Args>
void Cue::chain(Args... args) {
    auto g = create<T>(std::forward<Args>(args)...);
    chain(move(g));
}

} // namespace tact