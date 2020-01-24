#pragma once
#include <mahi/gui.hpp>
#include "DeviceBar.hpp"
#include "Workspace.hpp"
#include "Player.hpp"
#include "Visualizer.hpp"
#include "StatusBar.hpp"
#include "Library.hpp"
#include "Theme.hpp"

class Gui : public mahi::gui::Application {
public:
    Gui();
    void update() override;
    void positionWindows();
    const std::string& saveDir();
public:
    ThemeManager theme;
    DeviceBar device;
    Player player;
    Workspace workspace;
    Library library;
    Visualizer visualizer;
    StatusBar status;
};