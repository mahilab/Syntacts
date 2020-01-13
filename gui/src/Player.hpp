#pragma once

#include "Widget.hpp"

class Player : public Widget, public carnot::GameObject {
public:

    using Widget::Widget;

private:

    void start() override;
    void update() override;
    void render() override;
    void renderChannels();
    void playCh(int ch);
    void playSelected();
    void rechannel();

public:

    carnot::FloatRect rect;

private:
    int m_payload;
    float m_masterVol = 1.0f;
};