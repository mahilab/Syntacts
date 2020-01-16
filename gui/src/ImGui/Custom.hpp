#pragma once
#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>
#include <vector>
#include <string>
#include <syntacts>

namespace ImGui {

///////////////////////////////////////////////////////////////////////////////

bool MiniSliderScalar(const char* label, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, bool top, float power);
bool MiniSliderFloat(const char* label, float* v, float v_min, float v_max, bool top = true, float power = 1.0f);

void RenderGrid(ImRect bb, int nx, int ny, ImU32 gridColor, ImU32 bgColor, float thickness = 1.0f, float rounding = 0);
void PlotSignal(const char* label,  const tact::Signal& sig, std::vector<ImVec2>& points, float t1, float t2, ImVec4 color, float thickness, ImVec2 size = ImVec2(-1,0), bool grid = true);

///////////////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////////

bool TimelineScrollbar(float* ltime, float* rtime, bool* lrgrabbed, bool* rgrabbed, bool* cgrabbed, ImVec2 size = ImVec2(-1, 0), float grabWidth = 10);

///////////////////////////////////////////////////////////////////////////////

/// Begins a fixed size window without a header
inline void BeginFixed(const char* name, const ImVec2& pos, const ImVec2& size) {
    ImGui::SetNextWindowPos(pos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(size, ImGuiCond_Always);
    ImGui::Begin(name, nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
}

/// Renders an ImGui tooltip. Call after item to be tooltipped
inline void Tooltip(const char* tip) {
    if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 1.0f)  
        ImGui::SetTooltip(tip);
}

/// Begins a disabled section
inline void BeginDisabled(bool disabled = true) {
    if (disabled) {
        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
    }
}

/// Ends a disabled section
inline void EndDisabled(bool disabled = true) {
    if (disabled) {
        ImGui::PopItemFlag();
        ImGui::PopStyleVar();
    }
}

} // namespace ImGui