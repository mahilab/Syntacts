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
    m_root.update();
    ImGui::EndChild();
    bool openHelp = true;
    if (HelpTarget())
        ImGui::OpenPopup("Designer Help");
    if (ImGui::BeginPopupModal("Designer Help", &openHelp, ImGuiWindowFlags_NoResize)) {
        ImGui::BulletText("Drag items from the Palette or Library tabs into empty slots of the Node stack");
        ImGui::BulletText("By default, the resulting output Signal will be the product of the entire Node stack");
        ImGui::BulletText("Each Node has controls to configure it");
        ImGui::BulletText("You can collapse Nodes by clicking their slot header");
        ImGui::BulletText("Click the close button to remove Nodes from the stack");
        ImGui::EndPopup();
    }

    ImGui::PopStyleColor();
}