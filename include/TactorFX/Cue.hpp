#pragma once

#include <TactorFX/Config.hpp>
#include <TactorFX/Oscillator.hpp>
#include <TactorFX/Envelope.hpp>
#include <vector>
#include <utility>

namespace tfx {

/// Encapsulates a cue to be played on a single channel
class TFX_API Cue {
public:

    /// Default constructor
    Cue();

    /// Constructs a Cue with a Oscillator lasting a specified duration
    Cue(std::shared_ptr<Oscillator> osc, float duration);

    /// Constructs a Cue with a Oscillator and Envelope follower
    Cue(std::shared_ptr<Oscillator> osc, std::shared_ptr<Envelope> env);

    /// Constructs a Cue with a Oscillator, modulating Oscillator, and Envelope follower
    Cue(std::shared_ptr<Oscillator> osc, std::shared_ptr<Oscillator> mod, std::shared_ptr<Envelope> env);

    /// Virtual destructor
    virtual ~Cue();

    /// Chains an existing custom Generator to be processed
    void chain(std::shared_ptr<Generator> gen);

    /// Makes and then chains a custon Generator to be processed
    template <typename T, typename ...Args>
    void chain(Args ... args);

    /// Compounds all Generators to compute the next sample
    float nextSample();

private:

    std::vector<std::shared_ptr<Generator>> m_generators; ///< array of generators

};

template <typename T, typename ...Args>
void Cue::chain(Args... args) {
    auto g = std::make_shared<T>(std::forward<Args>(args)...);
    chain(std::move(g));
}

} // namespace tfx