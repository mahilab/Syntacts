#pragma once

#include "Widget.hpp"

class Visualizer : public Widget {
public:
    Visualizer(Gui& gui);
    void setRenderedSignal(tact::Signal sig, mahi::gui::Color color);
    void update() override;
private:
    float m_tl = 0, m_tr = 1;
    bool m_lg = false, m_rg = false, m_cg = false;
    tact::Signal m_signal;
    mahi::gui::Color m_color;
    std::vector<ImVec2> m_points;
};