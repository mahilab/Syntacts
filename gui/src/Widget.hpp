#pragma once
#include <syntacts>
#include <Mahi/Gui.hpp>

class Gui;

class Widget {
public:
    Widget(Gui& gui);
    virtual void update() { };
public:
    mahi::gui::Vec2 position, size;
    Gui& gui;
};