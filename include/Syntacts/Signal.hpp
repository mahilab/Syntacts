#pragma once

#include <Syntacts/Macros.hpp>
#include <Syntacts/Util.hpp>
#include <memory>

namespace tact
{

///////////////////////////////////////////////////////////////////////////////

/// Generates time variant samples 
class SYNTACTS_API Signal {
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

/// A Signal that emits a constant value over time
class SYNTACTS_API Scalar
{
public:
    Scalar(float value = 1);
    float sample(float t) const;
    float length() const;
public:
    float value;
private:
    TACT_SERIALIZE(TACT_PARENT(ISignal), TACT_MEMBER(value));
};

///////////////////////////////////////////////////////////////////////////////

/// A Signal that increases or decreases over time
class SYNTACTS_API Ramp 
{
public:
    Ramp(float initial = 1, float rate = 0);
    Ramp(float initial, float final, float span);
    float sample(float t) const;
    float length() const;
public:
    float initial;
    float rate;
private:
    TACT_SERIALIZE(TACT_PARENT(ISignal), TACT_MEMBER(initial), TACT_MEMBER(rate));
};

///////////////////////////////////////////////////////////////////////////////

/// A signal that generates white noise
class SYNTACTS_API Noise
{
public:
    Noise();
    float sample(float t) const;
    float length() const;
private:
    friend class cereal::access;
    template <class Archive>
    void serialize(Archive& archive) {}
};

///////////////////////////////////////////////////////////////////////////////


} // namespace tact

