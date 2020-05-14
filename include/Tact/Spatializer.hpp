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
