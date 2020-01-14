#pragma once
#include "Widget.hpp"
#include "ImGui/Custom.hpp"

class Spatializer : public Widget {
public:
    Spatializer(Gui* gui);
    void render() override;
    tact::Signal getSignal();

private:
    void sync();
    void update();
    void fillGrid();
    void onSessionInit();
private:
    tact::Signal m_signal;
    int m_rollOffIndex = 0;
    std::string m_sigName = "##Empty";
    tact::Spatializer m_spatializer;
    int m_divs[2] = {5,5};
    bool m_snap = true;
    bool m_1d   = false;
    bool xFirst = false;
    char m_inputBuffer[64] = "";
    std::map<int, ImGui::SpatializerNode> m_channels;
    ImGui::SpatializerTarget m_target;
};