#include "Gui.hpp"
#include "DragAndDrop.hpp"
#include <filesystem>

using namespace mahi::gui;

namespace fs = std::filesystem;

Gui::Gui(const Application::Config config) :
    Application(config),
    theme(*this),
    status(*this),
    device(*this),
    player(*this),
    workspace(*this),
    library(*this),
    visualizer(*this),
    debug(*this)
{
    fs::create_directories(saveDir());
    static auto inifile = saveDir() + "imgui.ini";
    ImGui::GetIO().IniFilename = inifile.c_str();
    positionWindows();    
    ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_DockingEnable;
}

void Gui::update() {
    positionWindows();
    static ImGuiIO& io = ImGui::GetIO();
    theme.update();
    device.update();
    player.update();
    workspace.update();
    library.update();
    visualizer.update();
    status.update();
    debug.update();
    UpdateDragAndDrop();
}

const std::string &Gui::saveDir()
{
#ifdef _WIN32
    static fs::path p = std::string(std::getenv("APPDATA")) + std::string("\\Syntacts\\GUI\\");
    static std::string dir = p.generic_string();
#elif __APPLE__
    static fs::path p = std::string(getenv("HOME")) + "/Library/Syntacts/GUI/";
    static std::string dir = p.generic_string();
#else
    static std::string dir = "";
#endif
    return dir;
}

void Gui::positionWindows()
{
    ImVec2 windowSize = {960, 540};
    ImVec2 vpPos = ImGui::GetMainViewport()->Pos;
    float margin = 5;
    float barHeight = 33;
    float sidePanelWidth = 180;
    float workspaceHeight = 300;
    float columnHeight = windowSize.y - barHeight - 3 * margin;
    float centerWidth = windowSize.x - 2 * (2 * margin + sidePanelWidth);
    float fullWidth = windowSize.x - 2 * margin;
    float l = margin;
    float t = margin;
    library.position = vpPos + ImVec2(l,t);
    library.size =  {sidePanelWidth, columnHeight};
    t += columnHeight + margin;
    status.position = vpPos + ImVec2(l,t);
    status.size = {fullWidth, barHeight};
    t = margin;
    l += sidePanelWidth + margin;
    device.position = vpPos + ImVec2(l,t);
    device.size = {centerWidth, barHeight};
    t += barHeight + margin;
    workspace.position = vpPos + ImVec2(l,t);
    workspace.size = {centerWidth, workspaceHeight};
    t += workspaceHeight + margin;
    visualizer.position = vpPos + ImVec2(l,t);
    visualizer.size = {centerWidth, windowSize.y - workspaceHeight - barHeight * 2 - margin * 5};
    t = margin;
    l += centerWidth + margin;
    player.position = vpPos + ImVec2(l,t);
    player.size = {sidePanelWidth, columnHeight};
}