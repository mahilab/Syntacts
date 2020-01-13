#include <Tact/Spatializer.hpp>

namespace tact {

    Spatializer::Spatializer(Session* session) :
        m_session(session),
        m_target({0.5,0.5}),
        m_radius(0.25),
        m_volume(1),
        m_rollOff(Curves::Linear())
    {
        
    }

    Spatializer::~Spatializer() {
        clear();
    }

    void Spatializer::bind(Session* session) {
        unbind();
        m_session = session;
    } 

    void Spatializer::unbind() {
        if (m_session)
        {
            for (auto& pair : m_positions) {
                int ch = pair.first;
                m_session->stop(ch);
                m_session->setVolume(ch, 1.0f);
                m_session->setPitch(ch, 1.0f);
            }
        }
        m_session = nullptr;
    }

    void Spatializer::setPosition(int channel, double x, double y) {
        x = clamp01(x);
        y = clamp01(y);
        m_positions[channel] = {x,y};
        update();
    }

    void Spatializer::setPosition(int channel, Point p) {
        m_positions[channel] = p;
        update();
    }

    Spatializer::Point Spatializer::getPosition(int channel) const {
        return m_positions.at(channel);
    }

    void Spatializer::setTarget(double x, double y) {
        x = clamp01(x);
        y = clamp01(y);
        m_target = {x,y};
        update();
    }

    void Spatializer::setTarget(Point p) {
        m_target = p;
        update();
    }

    Spatializer::Point Spatializer::getTarget() const {
        return m_target;
    }

    void Spatializer::setRadius(double r) {
        assert(r > 0);
        m_radius = r;
        update();
    }

    double Spatializer::getRadius() const {
        return m_radius;
    }

    void Spatializer::setRollOff(Curve rollOff) {
        m_rollOff = rollOff;
        update();
    }

    Curve Spatializer::getRollOff() const {
        return m_rollOff;
    }

    void Spatializer::clear() {
        for (auto& pair : m_positions) {
            int ch = pair.first;
            m_session->stop(ch);
            m_session->setVolume(ch, 1.0f);
            m_session->setPitch(ch, 1.0f);
        }
        m_positions.clear();        
    }

    void Spatializer::remove(int channel) {
        if (m_positions.count(channel)) {
            m_session->stop(channel);
            m_session->setVolume(channel, 1.0f);
            m_session->setPitch(channel, 1.0f);
            m_positions.erase(channel);
        }
    }

    int Spatializer::getChannelCount() const {
        return (int)m_positions.size();
    }

    std::vector<int> Spatializer::getChannels() const {
        std::vector<int> channels;
        channels.reserve(m_positions.size());
        for (auto& c : m_positions)
            channels.push_back(c.first);
        return channels;
    }

    bool Spatializer::hasChannel(int channel) const {
        return m_positions.count(channel) > 0;
    }

    void Spatializer::play(Signal signal) {
        update();
        for (auto& pair : m_positions) {
            m_session->play(pair.first, signal);
        }
    }

    void Spatializer::stop() {
        for (auto& pair : m_positions) {
            m_session->stop(pair.first);
        }
    }

    void Spatializer::setVolume(double volume) {
        m_volume = volume;
        update();
    }

    void Spatializer::setPitch(double pitch) {
        for (auto& pair : m_positions) {
            m_session->setPitch(pair.first, pitch);
        }
    }    

    void Spatializer::update() {
        // std::cout << "------------------" << std::endl; 
        for (auto& pair : m_positions) {
            int ch = pair.first;
            Point v;
            v.x = pair.second.x - m_target.x;
            v.y = pair.second.y - m_target.y;
            double d = std::sqrt(v.x*v.x + v.y*v.y);
            double vol = 1.0 - clamp01(d / m_radius);
            vol = m_rollOff(vol);
            vol *= m_volume;
            // std::cout << vol << std::endl;
            m_session->setVolume(ch, vol);
        }
    }
}
