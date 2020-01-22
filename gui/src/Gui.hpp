#pragma once
#include <mahi/gui.hpp>
#include "DeviceBar.hpp"
#include "Workspace.hpp"
#include "Player.hpp"
#include "Visualizer.hpp"
#include "StatusBar.hpp"
#include "Library.hpp"
#include "DragAndDrop.hpp"

class Gui : public mahi::gui::Application {
public:
    Gui();
    void update() override;
    void positionWindows();
public:
    DeviceBar device;
    Player player;
    Workspace workspace;
    Library library;
    Visualizer visualizer;
    StatusBar status;
};