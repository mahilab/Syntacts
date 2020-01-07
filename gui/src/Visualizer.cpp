#include "Visualizer.hpp"

using namespace carnot;

void Visualizer::setRenderedSignal(tact::Signal sig, carnot::Color color)
{
    m_signal = std::move(sig);
    m_color = color;
}

void Visualizer::start()
{
    m_points.resize(10000);
}

void Visualizer::update()
{
    render();
}

void Visualizer::render()
{
    ImGui::BeginFixed(getName().c_str(), rect.getPosition(), rect.getSize());
    ImGui::PlotSignal("Visualizer",m_signal,m_points,m_color,1.0f,ImVec2(-1,-1));
    ImGui::End();
}