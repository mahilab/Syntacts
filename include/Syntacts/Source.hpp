#pragma once

#include <Syntacts/Config.hpp>
#include <memory>

namespace tact
{

/// An abstract class which generates time variant samples
class SYNTACTS_API Source {
public:

    /// Default constructor
    Source() = default;

    /// Virtual destructor
    virtual ~Source() = default;

    /// Override to implement generator sampling behavior (required).
    virtual float sample(float t) = 0;
    
};
    
} // namespace tact
