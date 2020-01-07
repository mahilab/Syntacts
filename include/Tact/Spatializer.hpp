#pragma once

#include <Tact/Session.hpp>

namespace tact
{

class SYNTACTS_API Spatializer {
public:
    struct Point {
        float x,y;
    };
    Spatializer(Session* session = nullptr);
    ~Spatializer();

    void bind(Session* session);
    void unbind();

    void setPosition(int channel, float x, float y = 0);
    void setPosition(int channel, Point p);
    Point getPosition(int channel) const;

    void setTarget(float x, float y = 0);
    void setTarget(Point p);
    Point getTarget() const;
    void setRadius(float r);
    float getRadius() const;
    void setRollOff(Curve rollOff);
    Curve getRollOff() const;

    void clear();
    void remove(int channel);
    int getChannelCount() const;
    bool hasChannel(int channel) const;
    std::vector<int> getChannels() const;

    void play(Signal signal);   
    void stop();
    void setVolume(float volume);
    void setPitch(float pitch);

    void save(const std::string& name);
    void load(const std::string& name);
private:
    void update();
private:

    Session* m_session;
    Point m_target;
    float m_radius;
    float m_volume;
    Curve m_rollOff;
    std::map<int,Point> m_positions;
};
    
} // namespace tact
