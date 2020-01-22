#pragma once
#include <syntacts>
#include <mahi/gui.hpp>

class Gui;

class Widget {
public:
    Widget(Gui& gui);
    virtual void update() = 0;
public:
    mahi::gui::Vec2 position, size;
    Gui& gui;
};