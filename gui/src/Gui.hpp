#pragma once
#include "DeviceBar.hpp"
#include "Workspace.hpp"
#include "Player.hpp"
#include "Visualizer.hpp"
#include "Palette.hpp"
#include "StatusBar.hpp"
#include "Library.hpp"
#include "DragAndDrop.hpp"

class Gui : public carnot::GameObject {
public:
    Gui();
    void start() override;
    void update() override;
    void lateUpdate() override;
    void positionWindows();
public:
    DeviceBar* const device;
    Player* const player;
    Workspace* const workspace;
    StatusBar* const status;
    DragAndDrop* const dnd;
    Library* const library;
    Visualizer* const visualizer;
};