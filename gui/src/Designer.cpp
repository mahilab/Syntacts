#include "Designer.hpp"

using namespace carnot;

tact::Signal Designer::buildSignal()
{
    return m_root.signal();
}

void Designer::render()
{
    ImGui::PushStyleColor(ImGuiCol_ChildBg, Color::Transparent);
    ImGui::BeginChild("Designer##TabScroll");
    m_root.gui();
    ImGui::EndChild();
    ImGui::PopStyleColor();
}