#pragma once
#include <syntacts>
#include <carnot>

class Gui;

class Widget {
public:
    Widget(Gui* gui);
    virtual void render() = 0;
public:
    Gui*const gui;
};