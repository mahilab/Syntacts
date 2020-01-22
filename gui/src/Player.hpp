#pragma once

#include "Widget.hpp"

class Player : public Widget {
public:

    Player(Gui& gui);
    void update() override;

private:

    void updateChannels();
    void playCh(int ch);
    void playSelected();
    void rechannel();

private:
    int m_payload;
    float m_masterVol = 1.0f;
    float m_masterPitch = 0.0f;
};