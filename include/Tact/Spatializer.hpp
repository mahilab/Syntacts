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

class SYNTACTS_API Spatializer {
public:
    struct Point {
        double x,y;
    };
    Spatializer(Session* session = nullptr);
    ~Spatializer();

    void bind(Session* session);
    void unbind();

    void setPosition(int channel, double x, double y = 0);
    void setPosition(int channel, Point p);
    Point getPosition(int channel) const;

    void setTarget(double x, double y = 0);
    void setTarget(Point p);
    Point getTarget() const;
    void setRadius(double r);
    double getRadius() const;
    void setRollOff(Curve rollOff);
    Curve getRollOff() const;
    
    bool createGrid(int rows, int cols);

    void clear();
    void remove(int channel);
    int getChannelCount() const;
    bool hasChannel(int channel) const;
    std::vector<int> getChannels() const;

    void play(Signal signal);   
    void stop();

    void setVolume(double volume);
    double getVolume() const;

    void setPitch(double pitch);
    double getPitch() const;

    void autoUpdate(bool enable);
    void update();
private:
    Session* m_session;
    Point m_target;
    double m_radius;
    double m_volume;
    double m_pitch;
    Curve m_rollOff;
    bool m_autoUpdate;
    std::map<int,Point> m_positions;
};
    
} // namespace tact
