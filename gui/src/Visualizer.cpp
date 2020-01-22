#include "Visualizer.hpp"
#include "Custom.hpp"

using namespace mahi::gui;

Visualizer::Visualizer(Gui& gui) : Widget(gui), m_points(10000) {

}

void Visualizer::setRenderedSignal(tact::Signal sig, Color color)
{
    m_signal = std::move(sig);
    m_color = color;
}


void Visualizer::update()
{
    ImGui::BeginFixed("Visualizer", position, size);
    float h = ImGui::GetContentRegionAvail().y - ImGui::GetStyle().ScrollbarSize - ImGui::GetStyle().ItemSpacing.y;
    float len = (float)m_signal.length();
    len = len == tact::INF ? 1 : len;
    float t1 = m_tl * len;
    float t2 = m_tr * len;
    ImGui::PlotSignal("Visualizer",m_signal,m_points,t1,t2,m_color,1.0f,ImVec2(-1,h));
    ImGui::TimelineScrollbar(&m_tl, &m_tr, &m_lg, &m_rg, &m_cg);
    ImGui::End();
}

