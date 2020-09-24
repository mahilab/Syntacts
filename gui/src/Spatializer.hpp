#pragma once
#include "Widget.hpp"
#include "Custom.hpp"

///////////////////////////////////////////////////////////////////////////////
// CUSTOM IMGUI

namespace ImGui {

struct SpatializerNode {
    int index;
    ImVec2 pos;
    bool held   = false;
    float level = 0;
};

struct SpatializerTarget {
    ImVec2 pos;
    float radius;
};

bool Spatializer(const char* label, SpatializerTarget& target, tact::Curve rolloff, 
                 std::map<int,SpatializerNode>& nodes, float nodeRadius, ImVec4 color, 
                 ImVec2 size, const char* dnd, int xdivs = 10, int ydivs = 10, bool snap = false, bool wrap = false);

} // namespace ImGui

///////////////////////////////////////////////////////////////////////////////

class Spatializer : public Widget {
public:
    Spatializer(Gui& gui);
    ~Spatializer();
    tact::Signal getSignal();
    void update();
    tact::Spatializer spatializer;
private:
    void sync();
    void fillGrid();
    void onSessionChange();
    void onSessionDestroy();

private:
    tact::Signal m_signal;
    int m_rollOffIndex = 0;
    int m_rollOffHoveredIdx = -1;
    std::string m_sigName = "##Empty";
    int m_divs[2] = {5,5};
    bool m_snap = true;
    bool m_2d   = true;
    bool m_wrap = false;
    bool xFirst = false;
    char m_inputBuffer[64] = "";
    std::map<int, ImGui::SpatializerNode> m_channels;
    ImGui::SpatializerTarget m_target;
    std::size_t m_openId, m_destroyId;
};