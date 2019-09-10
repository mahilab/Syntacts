#pragma once

#include <Syntacts/Config.hpp>
#include <memory>

namespace tact
{

/// An abstract class which generates time variant samples
class SYNTACTS_API Generator {
public:

    /// Default constructor
    Generator() = default;

    /// Virtual destructor
    virtual ~Generator() = default;

    /// Override to implement generator sampling behavior (required).
    virtual float sample(float t) = 0;

};

    
} // namespace tact
