#include "Gui.hpp"

using namespace carnot;



Gui::Gui() : 
             device(makeChild<DeviceBar>(this).get()),
             player(makeChild<Player>(this).get()),
             workspace(makeChild<Workspace>(this).get()),
             status(makeChild<StatusBar>(this).get()),
             dnd(makeChild<DragAndDrop>().get()),
             library(makeChild<Library>(this).get()),             
             visualizer(makeChild<Visualizer>(this).get())
{

}

void Gui::start() {
    ImGuiStyle *imStyle = &ImGui::GetStyle();
    ImVec4 *colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_Border] = colors[ImGuiCol_TabActive];
    colors[ImGuiCol_Separator] = colors[ImGuiCol_TabActive];
    colors[ImGuiCol_DragDropTarget] = colors[ImGuiCol_Text];
    positionWindows();
}

void Gui::update() {
}

void Gui::lateUpdate() {
}

void Gui::positionWindows()
{
    auto windowSize = Engine::getWindowSize();
    float margin = 5;
    float barHeight = 35;
    float sidePanelWidth = 180;
    float workspaceHeight = 300;
    float columnHeight = windowSize.y - barHeight - 3 * margin;
    float centerWidth = windowSize.x - 2 * (2 * margin + sidePanelWidth);
    float fullWidth = windowSize.x - 2 * margin;
    float l = margin;
    float t = margin;
    library->rect = FloatRect(l, t, sidePanelWidth, columnHeight);
    t += columnHeight + margin;
    status->rect = FloatRect(l, t, fullWidth, barHeight);
    t = margin;
    l += sidePanelWidth + margin;
    device->rect = FloatRect(l, t, centerWidth, barHeight);
    t += barHeight + margin;
    workspace->rect = FloatRect(l, t, centerWidth, workspaceHeight);
    t += workspaceHeight + margin;
    visualizer->rect = FloatRect(l, t, centerWidth, windowSize.y - workspaceHeight - barHeight * 2 - margin * 5);
    t = margin;
    l += centerWidth + margin;
    player->rect = FloatRect(l, t, sidePanelWidth, columnHeight);
}