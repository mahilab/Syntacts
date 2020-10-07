#pragma once
#include <Mahi/Gui.hpp>
#include <Mahi/Util.hpp>
#include "DeviceBar.hpp"
#include "Workspace.hpp"
#include "Player.hpp"
#include "Visualizer.hpp"
#include "StatusBar.hpp"
#include "Library.hpp"
#include "Theme.hpp"
#include "Debugger.hpp"

class Gui : public mahi::gui::Application {
public:
    Gui(const Application::Config config);
    void update() override;
    void positionWindows();
    const std::string& saveDir();
public:
    ThemeManager theme;
    StatusBar status;
    DeviceBar device;
    Player player;
    Workspace workspace;
    Library library;
    Visualizer visualizer;
    Debugger debug;
};