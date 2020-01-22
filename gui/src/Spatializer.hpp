#pragma once
#include "Widget.hpp"
#include "Custom.hpp"

///////////////////////////////////////////////////////////////////////////////
// CUSTOM IMGUI

namespace ImGui {

struct SpatializerNode {
    int index;
    ImVec2 pos;
    bool held;
};

struct SpatializerTarget {
    ImVec2 pos;
    float radius;
};

bool Spatializer(const char* label, SpatializerTarget& target, tact::Curve rolloff, std::map<int,SpatializerNode>& nodes, float nodeRadius, ImVec4 color, ImVec2 size, const char* dnd, int xdivs = 10, int ydivs = 10, bool snap = false);

} // namespace ImGui

///////////////////////////////////////////////////////////////////////////////

class Spatializer : public Widget {
public:
    Spatializer(Gui& gui);
    tact::Signal getSignal();
    void update();
private:
    void sync();
    void fillGrid();
    void onSessionChange();
    void onSessionDestroy();
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