#pragma once

#include "Widget.hpp"

class StatusBar : public Widget, public carnot::GameObject {
public:

    enum InfoLevel {
        Info = 0,
        Warning = 1,
        Error = 2
    };

    StatusBar(Gui* gui);
    void start() override;
    void update() override;
    void pushMessage(const std::string& text, InfoLevel level = Info);
    void showTooltip(const std::string& tooltip);
private:
    void render() override;
    void renderText();
    void renderButtons();
    void toggleTheme();
public:
    carnot::FloatRect rect;
private:
    bool dark = true;
    float m_cpuLoad = 0.0;
    carnot::Sequence<carnot::Color> m_cpuGradient;
    carnot::Sequence<carnot::Color> m_fadeColors;
    std::string m_notification;
    std::string m_tooltip;
    float m_fadeTime;
    float m_fadeDuration = 5.0f;
};