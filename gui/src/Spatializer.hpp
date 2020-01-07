#pragma once
#include "Widget.hpp"
#include "ImGui/Custom.hpp"

class Spatializer : public Widget {
public:
    Spatializer(Gui* gui);
    void render() override;
private:
    void update();
    void fillGrid();
    void onSessionInit();
public:
    tact::Signal signal;
private:
    std::string m_sigName = "##Empty";
    tact::Spatializer m_spatializer;
    int m_divs[2] = {5,5};
    bool m_snap = true;
    bool m_1d   = false;
    float m_volume = 1;
    float m_pitch = 1;
    bool rowsFirst = true;
    char m_inputBuffer[64] = "";
    std::map<int, ImGui::SpatializerNode> m_channels;
    ImGui::SpatializerTarget m_target;
};