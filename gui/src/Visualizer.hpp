#pragma once

#include "Widget.hpp"
#include "ImGui/Custom.hpp"

class Visualizer : public Widget, public carnot::GameObject {
public:
    using Widget::Widget;
    void setRenderedSignal(tact::Signal sig, carnot::Color color);
private:
    void start() override;
    void update() override;
    void render() override;
public:
    carnot::FloatRect rect;
private:
    float m_tl = 0, m_tr = 1;
    bool m_lg = false, m_rg = false, m_cg = false;
    tact::Signal m_signal;
    carnot::Color m_color;
    std::vector<ImVec2> m_points;
};