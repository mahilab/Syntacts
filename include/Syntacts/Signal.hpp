#pragma once

#include <Syntacts/Config.hpp>
#include <Syntacts/Serialization.hpp>
#include <Syntacts/Memory.hpp>

namespace tact
{

///////////////////////////////////////////////////////////////////////////////

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

    /// Creates a deep copy of the Signal
    virtual Ptr<SignalBase> clone() const;

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

/// Signal with automatic clone implementation via CRTP
template <typename Derived>
class SignalCloneable : public SignalBase {
public:
    virtual Ptr<SignalBase> clone() const override {
        return create<Derived>(static_cast<Derived const &>(*this));
    }
};

///////////////////////////////////////////////////////////////////////////////

/// A Signal that emits a constant value over time
class SYNTACTS_API Scalar : public SignalCloneable<Scalar>
{
public:
    Scalar(float value = 1);

    /// Implements scalar
    virtual float sample(float t) const override;

public:
    float value; ///< the scalar value

private:
    SERIALIZE(PARENT(SignalBase), MEMBER(value));
};

///////////////////////////////////////////////////////////////////////////////

/// A Signal that increaes or decreases over time
class SYNTACTS_API Ramp : public SignalCloneable<Ramp>
{
public:
    Ramp(float initial = 1, float rate = 0);
    Ramp(float initial, float final, float span);

    virtual float sample(float t) const override;

public:
    float initial;
    float rate;

private:
    SERIALIZE(PARENT(SignalBase), MEMBER(initial), MEMBER(rate));
};

///////////////////////////////////////////////////////////////////////////////

} // namespace tact

