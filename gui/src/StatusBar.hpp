#pragma once

#include "Widget.hpp"

class StatusBar : public Widget {
public:

    enum InfoLevel {
        Info = 0,
        Warning = 1,
        Error = 2
    };

    StatusBar(Gui& gui);
    void update() override;
    void pushMessage(const std::string& text, InfoLevel level = Info);
    void showTooltip(const std::string& tooltip);
private:
    void renderText();
    void renderButtons();
private:
    bool m_dark = true;
    double m_cpuLoad = 0.0;
    mahi::gui::Sequence<mahi::gui::Color> m_cpuGradient;
    mahi::gui::Sequence<mahi::gui::Color> m_fadeColors;
    std::string m_notification;
    std::string m_tooltip;
    float m_fadeTime;
    float m_fadeDuration = 5.0f;
};