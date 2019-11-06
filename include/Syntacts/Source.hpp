#pragma once

#include <Syntacts/Config.hpp>
#include <Syntacts/Serialization.hpp>
#include <Syntacts/Memory.hpp>
#include <atomic>

namespace tact
{

//=============================================================================

/// An abstract class which generates time variant samples
class SYNTACTS_API Source
{
public:
    /// Default constructor
    Source() = default;

    /// Virtual destructor
    virtual ~Source() = default;

    /// Override to implement generator sampling behavior (required).
    virtual float sample(float t) const = 0;

private:
    // Serialization
    friend class cereal::access;
    template <class Archive>
    void serialize(Archive& archive) {}
};

//=============================================================================

class SYNTACTS_API Scalar : public Source
{
public:
    Scalar(float value = 1);

    /// Implements scalar
    virtual float sample(float t) const override;

private:
    std::atomic<float> m_value; ///< the scalar value

private:
    SERIALIZE(PARENT(Source), MEMBER(m_value));
};

//=============================================================================

class SYNTACTS_API Ramp : public Source
{
public:
    Ramp(float initial = 1, float rate = 0);
    Ramp(float initial, float final, float span);

    virtual float sample(float t) const override;

private:
    std::atomic<float> m_initial;
    std::atomic<float> m_rate;

private:
    SERIALIZE(PARENT(Source), MEMBER(m_initial), MEMBER(m_rate));
};

//=============================================================================

} // namespace tact

