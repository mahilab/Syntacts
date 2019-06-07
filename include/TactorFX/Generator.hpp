#pragma once

#include <TactorFX/Config.hpp>
#include <memory>

namespace tfx
{

// Forward declarations
class Cue;

/// An abstract class which generates time variant samples
class TFX_API Generator {
public:

    /// Default constructor
    Generator();

    /// Virtual destructor
    virtual ~Generator();

    /// Returns the current Generator time
    float getTime() const;

protected:

    /// Override to implement generator behavior. Do not lock m_mutex here.
    virtual float onSample(float t) = 0;

private:

    // Friend class Cue so it can sample this
    friend class Cue;

    /// Returns the next sample of the Oscillator
    float nextSample();

private:

     float m_time;     ///< oscillator running time

};

    
} // namespace tfx
