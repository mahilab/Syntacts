#pragma once

#include <Syntacts/Config.hpp>
#include <Syntacts/Source.hpp>
#include <memory>

namespace tact
{

class SYNTACTS_API Scalar : public Source
{
public:
    /// Constructs a scalar value source
    Scalar(float value = 1.0f);

    /// Implements scalar
    virtual float sample(float t) override; 

private:
    float m_value; ///< the scalar value
};

} // namespace tact