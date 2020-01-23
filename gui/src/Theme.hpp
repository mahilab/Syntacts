#pragma once
#include "Widget.hpp"

struct ThemeManager : public Widget {
public:

    enum Theme {
        DarkDefault = 0,
        LightDefault,
        DarkAlt,
        Valve,
        Theme_Count
    };

    ThemeManager(Gui& gui);
    ~ThemeManager();
    void cycle();
    void setTheme(Theme theme);
public:
    mahi::gui::Color libraryColor     = mahi::gui::Blues::LightSkyBlue;
    mahi::gui::Color designerColor    = mahi::gui::Purples::Plum;
    mahi::gui::Color sequencerColor   = mahi::gui::Oranges::DarkOrange;
    mahi::gui::Color spatializerColor = mahi::gui::Greens::Chartreuse;
private:
    Theme m_theme;
};