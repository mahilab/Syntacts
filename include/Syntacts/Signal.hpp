#pragma once

#include <Syntacts/Config.hpp>
#include <Syntacts/Serialization.hpp>
#include <Syntacts/Memory.hpp>
#include <atomic>

namespace tact
{

//=============================================================================

/// An abstract base class which generates time variant samples
class SYNTACTS_API SignalBase
{
public:
    /// Default constructor
    SignalBase();

    /// Virtual destructor
    virtual ~SignalBase();

    /// Override to implement generator sampling behavior (required).
    virtual float sample(float t) const = 0;

public:
    /// Returns the number of Signals across the entire process
    static int count();

private:
    static int s_count;

private:
    // Serialization
    friend class cereal::access;
    template <class Archive>
    void serialize(Archive& archive) {}

};

//=============================================================================

/// A Signal that emits a constant value over time
class SYNTACTS_API Scalar : public SignalBase
{
public:
    Scalar(float value = 1);

    /// Implements scalar
    virtual float sample(float t) const override;

private:
    std::atomic<float> m_value; ///< the scalar value

private:
    SERIALIZE(PARENT(SignalBase), MEMBER(m_value));
};

//=============================================================================

/// A Signal that increaes or decreases over time
class SYNTACTS_API Ramp : public SignalBase
{
public:
    Ramp(float initial = 1, float rate = 0);
    Ramp(float initial, float final, float span);

    virtual float sample(float t) const override;

private:
    std::atomic<float> m_initial;
    std::atomic<float> m_rate;

private:
    SERIALIZE(PARENT(SignalBase), MEMBER(m_initial), MEMBER(m_rate));
};

//=============================================================================

} // namespace tact

