#pragma once

#include <Syntacts/Config.hpp>
#include <Syntacts/Serialization.hpp>
#include <Syntacts/Memory.hpp>

namespace tact
{

///////////////////////////////////////////////////////////////////////////////

/// An abstract base class which generates time variant samples
class SYNTACTS_API ISignal
{
public:
    /// Default constructor
    ISignal();

    /// Virtual destructor
    virtual ~ISignal();

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

///////////////////////////////////////////////////////////////////////////////

/// A Signal that emits a constant value over time
class SYNTACTS_API Scalar : public ISignal
{
public:
    Scalar(float value = 1);

    /// Implements scalar
    virtual float sample(float t) const override;

public:
    float value; ///< the scalar value

private:
    SERIALIZE(PARENT(ISignal), MEMBER(value));
};

///////////////////////////////////////////////////////////////////////////////

/// A Signal that increases or decreases over time
class SYNTACTS_API Ramp : public ISignal
{
public:
    Ramp(float initial = 1, float rate = 0);
    Ramp(float initial, float final, float span);

    virtual float sample(float t) const override;

public:
    float initial;
    float rate;

private:
    SERIALIZE(PARENT(ISignal), MEMBER(initial), MEMBER(rate));
};

///////////////////////////////////////////////////////////////////////////////

} // namespace tact

