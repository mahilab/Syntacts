#include <Tact/Spatializer.hpp>

namespace tact {

    Spatializer::Spatializer(Session* session) :
        m_session(session),
        m_target({0.5,0.5}),
        m_radius(0.25),
        m_volume(1),
        m_pitch(1),
        m_rollOff(Curves::Linear()),
        m_autoUpdate(true)
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
        // x = clamp01(x);
        // y = clamp01(y);
        m_positions[channel] = {x,y};
        if (m_autoUpdate)
            update();
    }

    void Spatializer::setPosition(int channel, Point p) {
        m_positions[channel] = p;
        if (m_autoUpdate)
            update();
    }

    Spatializer::Point Spatializer::getPosition(int channel) const {
        return m_positions.at(channel);
    }

    void Spatializer::setTarget(double x, double y) {
        // x = clamp01(x);
        // y = clamp01(y);
        m_target = {x,y};
        if (m_autoUpdate)
            update();
    }

    void Spatializer::setTarget(Point p) {
        m_target = p;
        if (m_autoUpdate)
            update();
    }

    Spatializer::Point Spatializer::getTarget() const {
        return m_target;
    }

    void Spatializer::setRadius(double r) {
        assert(r > 0);
        m_radius = r;
        if (m_autoUpdate)
            update();
    }

    double Spatializer::getRadius() const {
        return m_radius;
    }

    void Spatializer::setRollOff(Curve rollOff) {
        m_rollOff = rollOff;
        if (m_autoUpdate)
            update();
    }

    Curve Spatializer::getRollOff() const {
        return m_rollOff;
    }

    bool Spatializer::createGrid(int rows, int cols) {
        clear();
        int n_ch = m_session->getChannelCount();
        int ch = 0; 
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                if (ch == n_ch)
                    return false;
                float x = (float)c / (float)(cols-1);
                float y = (float)r / (float)(rows-1);
                m_positions[ch] = {x,y};
                ch++;
            }
        }
        return true;
    }

    void Spatializer::clear() {
        if (m_session) {
            for (auto& pair : m_positions) {
                int ch = pair.first;
                m_session->stop(ch);
                m_session->setVolume(ch, 1.0);
                m_session->setPitch(ch, 1.0);
            }
        } 
        m_positions.clear();        
    }

    void Spatializer::remove(int channel) {
        if (m_positions.count(channel)) {
            if (m_session) {
                m_session->stop(channel);
                m_session->setVolume(channel, 1.0);
                m_session->setPitch(channel, 1.0);
            }
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
        if (m_session == nullptr)
            return;
        for (auto& pair : m_positions) 
            m_session->play(pair.first, signal);
    }

    void Spatializer::stop() {
        if (m_session == nullptr)
            return;
        for (auto& pair : m_positions) 
            m_session->stop(pair.first);
    }

    void Spatializer::setVolume(double volume) {
        m_volume = volume;
        if (m_autoUpdate)
            update();
    }

    double Spatializer::getVolume() const {
        return m_volume;
    }

    void Spatializer::setPitch(double pitch) {
        m_pitch = pitch;
        if (m_session == nullptr)
            return;
        for (auto& pair : m_positions) 
            m_session->setPitch(pair.first, m_pitch);
    }    

    double Spatializer::getPitch() const {
        return m_pitch;
    }

    void Spatializer::autoUpdate(bool enable) {
        m_autoUpdate = enable;
    }

    void Spatializer::update() {
        if (m_session == nullptr)
            return;
        for (auto& pair : m_positions) {
            int ch = pair.first;
            Point v;
            v.x = pair.second.x - m_target.x;
            v.y = pair.second.y - m_target.y;
            double d = std::sqrt(v.x*v.x + v.y*v.y);
            double vol = 1.0 - clamp01(d / m_radius);
            vol = m_rollOff(vol);
            vol *= m_volume;
            m_session->setVolume(ch, vol);
        }
    }
}
