#include "Gui.hpp"
#include "DragAndDrop.hpp"

using namespace mahi::gui;

Gui::Gui() : Application(960, 540, "Syntacts", false),
             theme(*this),
             device(*this),
             player(*this),
             workspace(*this),
             library(*this),
             visualizer(*this),
             status(*this)
{
    static auto inifile = saveDir() + "imgui.ini";
    ImGui::GetIO().IniFilename = inifile.c_str();
    positionWindows();
    
    // ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_ViewportsEnable;
    ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_DockingEnable;

}

void Gui::update() {
    static ImGuiIO& io = ImGui::GetIO();
    theme.update();
    device.update();
    player.update();
    workspace.update();
    library.update();
    visualizer.update();
    status.update();
    UpdateDragAndDrop();
}

const std::string &Gui::saveDir()
{
#ifdef _WIN32
    static std::string appData = std::string(std::getenv("APPDATA"));
    static std::string dir = appData + std::string("\\Syntacts\\GUI\\");
    std::filesystem::create_directories(dir);
#else
    static std::string dir = "";
#endif
    return dir;
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