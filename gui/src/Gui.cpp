#include "Gui.hpp"
#include "DragAndDrop.hpp"

using namespace mahi::gui;

Gui::Gui() : Application(960, 540, "Syntacts"),
             device(*this),
             player(*this),
             workspace(*this),
             library(*this),
             visualizer(*this),
             status(*this)
{
    backgroundColor = Grays::Gray5;
    ImGuiStyle *imStyle = &ImGui::GetStyle();
    ImVec4 *colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_Border] = colors[ImGuiCol_TabActive];
    colors[ImGuiCol_Separator] = colors[ImGuiCol_TabActive];
    colors[ImGuiCol_DragDropTarget] = colors[ImGuiCol_Text];
    positionWindows();
}

void Gui::update() {
    device.update();
    player.update();
    workspace.update();
    library.update();
    visualizer.update();
    status.update();
    UpdateDragAndDrop();
}

void Gui::positionWindows()
{
    ImVec2 windowSize = {960, 540};
    float margin = 5;
    float barHeight = 33;
    float sidePanelWidth = 180;
    float workspaceHeight = 300;
    float columnHeight = windowSize.y - barHeight - 3 * margin;
    float centerWidth = windowSize.x - 2 * (2 * margin + sidePanelWidth);
    float fullWidth = windowSize.x - 2 * margin;
    float l = margin;
    float t = margin;
    library.position = {l,t};
    library.size =  {sidePanelWidth, columnHeight};
    t += columnHeight + margin;
    status.position = {l,t};
    status.size = {fullWidth, barHeight};
    t = margin;
    l += sidePanelWidth + margin;
    device.position = {l,t};
    device.size = {centerWidth, barHeight};
    t += barHeight + margin;
    workspace.position = {l,t};
    workspace.size = {centerWidth, workspaceHeight};
    t += workspaceHeight + margin;
    visualizer.position = {l,t};
    visualizer.size = {centerWidth, windowSize.y - workspaceHeight - barHeight * 2 - margin * 5};
    t = margin;
    l += centerWidth + margin;
    player.position = {l,t};
    player.size = {sidePanelWidth, columnHeight};
}