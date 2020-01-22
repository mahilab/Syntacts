#include "Designer.hpp"

using namespace mahi::gui;

tact::Signal Designer::buildSignal()
{
    return m_root.signal();
}

void Designer::update()
{
    ImGui::PushStyleColor(ImGuiCol_ChildBg, {0,0,0,0});
    ImGui::BeginChild("Designer##TabScroll");
    m_root.gui();
    ImGui::EndChild();
    ImGui::PopStyleColor();
}