#pragma once

#include <Syntacts/Config.hpp>
#include <Syntacts/Macros.hpp>
#include <Syntacts/Memory.hpp>
#include <Syntacts/Util.hpp>

namespace tact
{

///////////////////////////////////////////////////////////////////////////////

/// Generates time variant samples 
class Signal {
public:
    /// Default constructor
    Signal();
    /// Constructor
    template<typename T>
    Signal(T signal) : m_ptr(std::make_shared<Model<T>>(std::move(signal))) { }
    /// Samples the Signal at time t in seconds
    float sample(float t) const;
    /// Returns the length of the Signal in seconds
    float length() const;
public:
    /// Type erasure concept that Signal implementations must adhere to
    struct Concept {
        Concept() = default;
        virtual ~Concept() = default;
        virtual float sample(float t) const = 0;
        virtual float length() const = 0;
        template <class Archive>
        void serialize(Archive& archive) {}
    };
    /// Model
    template <typename T>
    struct Model final : Concept {
        Model() = default;
        Model(T model) : m_model(std::move(model)) { }
        float sample(float t) const override 
        { return m_model.sample(t); }
        float length() const override
        { return m_model.length(); }
        T m_model;
        TACT_SERIALIZE(TACT_PARENT(Concept), TACT_MEMBER(m_model));
    };
private:
    std::shared_ptr<const Concept> m_ptr;
    TACT_SERIALIZE(TACT_MEMBER(m_ptr));
};

///////////////////////////////////////////////////////////////////////////////

/// An abstract base class which generates time variant samples
class SYNTACTS_API ISignal : public Cloneable<Interface<ISignal>>
{
public:
    /// Default constructor
    ISignal();
    /// Virtual destructor
    virtual ~ISignal();
    /// Override to implement generator sampling behavior (required).
    virtual float sample(float t) const = 0;
    /// Returns the length of the Signal in seconds
    float length() const;
public:
    /// Returns the number of Signals across the entire process
    static int count();
protected:
    float m_length; 
private:
    static int s_count;
private:
    // Serialization
    TACT_SERIALIZE(TACT_MEMBER(m_length));
};

///////////////////////////////////////////////////////////////////////////////

/// A Signal that emits a constant value over time
class SYNTACTS_API Scalar : public Cloneable<Scalar, ISignal>
{
public:
    Scalar(float value = 1);
    virtual float sample(float t) const override;
public:
    float value;
private:
    TACT_SERIALIZE(TACT_PARENT(ISignal), TACT_MEMBER(value));
};

///////////////////////////////////////////////////////////////////////////////

/// A Signal that increases or decreases over time
class SYNTACTS_API Ramp : public Cloneable<Ramp, ISignal>
{
public:
    Ramp(float initial = 1, float rate = 0);
    Ramp(float initial, float final, float span);
    virtual float sample(float t) const override;
public:
    float initial;
    float rate;
private:
    TACT_SERIALIZE(TACT_PARENT(ISignal), TACT_MEMBER(initial), TACT_MEMBER(rate));
};

///////////////////////////////////////////////////////////////////////////////

/// A signal that generates white noise
class SYNTACTS_API Noise : public Cloneable<Noise, ISignal>
{
public:
    Noise();
    virtual float sample(float t) const override;
private:
    TACT_SERIALIZE(TACT_PARENT(ISignal));
};

///////////////////////////////////////////////////////////////////////////////


} // namespace tact

