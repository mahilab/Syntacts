#pragma once

#include <TactorFX/Config.hpp>
#include <TactorFX/Generator.hpp>

namespace tfx
{

/// An object which gives a Cue a duration and/or shape
class TFX_API Envelope : public Generator {
public:

    /// Constructs an Eveloope with a specified duration
    Envelope(float duration = 1.0f, float amplitude = 1.0f);

    /// Sets the duration of an Envelope (default 1)
    void setDuration(float duration);

    /// Sets the normalized amplitude of an Envelope (default 1)
    void setAmplitude(float amplitude);

protected:

    /// Returns true as long as current time less than duration
    bool playing() const;

    /// Default Envelope implementation (returns 1 for full duration)
    virtual float onSample(float t) override;

protected:

    float m_duration;  ///< duration of an Envelope in seconds
    float m_amplitude; ///< amplitude of the envelope normalized between 0 and 1

};


class TFX_API ASR : public Envelope {
public:

    /// Constructs ASR Envelope with specified attack and sustain times
    ASR(float attackTime, float sustainTime, float releaseTime, float amplitude = 1.0f);

protected:

    /// ASR implementatioin
    virtual float onSample(float t) override;
    
private:

    float m_attackTime, m_sustainTime, m_releaseTime;

};


    
} // namespace tfx
