// MIT License
//
// Copyright (c) 2020 Mechatronics and Haptic Interfaces Lab
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// Author(s): Evan Pezent (epezent@rice.edu)

#pragma once

#include <Tact/Session.hpp>

namespace tact
{

/// Syntacts Spatializer interface.
class SYNTACTS_API Spatializer {
public:

    // 2D Spatializer point
    struct Point {
        double x,y;
    };

    /// Constructor.
    Spatializer(Session* session = nullptr);
    /// Destructor.
    ~Spatializer();

    /// Binds this Spatializer to a Session.
    void bind(Session* session);
    /// Unbinds the Spatializer from its current Session.
    void unbind();

    /// Set the position of a channel. The channel will be added if it's not already in the Spatializer.
    void setPosition(int channel, double x, double y = 0);
    /// Set the position of a channel. The channel will be added if it's not already in the Spatializer.
    void setPosition(int channel, const Point& p);
    /// Gets the position of a channel if it is in the Spatializer.
    Point getPosition(int channel) const;

    /// Set the Spatialier target position.
    void setTarget(double x, double y = 0);
    /// Set the Spatialier target position.
    void setTarget(const Point& p);
    /// Get the Spatializer target position.
    const Point& getTarget() const;
    /// Set the Spatializer target radius.
    void setRadius(double r);
    /// Get the Spatializer target radius.
    double getRadius() const;
    /// Set the Spatializer target roll-off method.
    void setRollOff(Curve rollOff);
    /// Get the Spatializer target roll-off method.
    Curve getRollOff() const;
    /// Enable Spatializer wrapping for one or both axes. A value of zero disables wrapping.
    void setWrap(double xInterval, double yInterval);
    /// Enable Spatializer wrapping for one or both axes. A value of zero disables wrapping.
    void setWrap(const Point& wrapInterval);
    /// Get Spatializer wrapping intervals. 0s indicate wrapping is disabled.
    const Point& getWrap() const;

    /// Quickly create a grid of channels.
    bool createGrid(int rows, int cols);
    /// Remove all channels from the Spatializer.
    void clear();
    /// Remove a channel from the Spatializer.
    void remove(int channel);
    /// Get the number of channels in the Spatializer.
    int getChannelCount() const;
    /// Returns true if a channel is in the Spatializer.
    bool hasChannel(int channel) const;
    /// Get all the channels in the Spatializer.
    std::vector<int> getChannels() const;

    /// Play a Signal on the Spatializer.
    void play(Signal signal);   
    /// Stop all Signals playing in the Spatializer.
    void stop();

    /// Set the global volume of the Spatializer.
    void setVolume(double volume);
    /// Get the global volume of the Spatializer.
    double getVolume() const;

    /// Set the global pitch of the Spatializer.
    void setPitch(double pitch);
    /// Get the global pitch of the Spatializer.
    double getPitch() const;

    /// Enable/disable automatic updating of pitch/volume of all channels when Spatializer target or channels change (enabled by default).
    void autoUpdate(bool enable);
    /// Explicitly update the pitch/volume of all channels in the Spatializer.
    void update();
    
private:
    Session* m_session;
    Point m_target;
    double m_radius;
    double m_volume;
    double m_pitch;
    Curve m_rollOff;
    bool m_autoUpdate;
    Point m_wrapInterval;
    std::map<int,Point> m_positions;
};
    
} // namespace tact
