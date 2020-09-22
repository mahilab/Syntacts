#pragma once
#include <Mahi/Gui/imgui_custom.hpp>
#include <imgui_internal.h>
#include <vector>
#include <string>
#include <syntacts>

namespace ImGui {

///////////////////////////////////////////////////////////////////////////////

// bool MiniSliderScalar(const char* label, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, bool top, float power);
// bool MiniSliderFloat(const char* label, float* v, float v_min, float v_max, bool top = true, float power = 1.0f);

void RenderGrid(ImRect bb, int nx, int ny, ImU32 gridColor, ImU32 bgColor, float thickness = 1.0f, float rounding = 0);
void PlotSignal(const char* label,  const tact::Signal& sig, std::vector<ImVec2>& points, float t1, float t2, ImVec4 color, float thickness, ImVec2 size = ImVec2(-1,0), bool grid = true, bool text = true);
void RenderSignalInBounds(ImDrawList* DrawList, const tact::Signal& sig, float t1, float t2, ImRect bb, ImVec4 color, float thickness, int n = 0);


///////////////////////////////////////////////////////////////////////////////

bool TimelineScrollbar(float* ltime, float* rtime, bool* lrgrabbed, bool* rgrabbed, bool* cgrabbed, ImVec2 size = ImVec2(-1, 0), float grabWidth = 10);

///////////////////////////////////////////////////////////////////////////////

inline bool TintedButton(const char* label, ImVec4 color, const ImVec2& size = ImVec2(0,0)) {
    ImGui::PushStyleColor(ImGuiCol_Button, color);
    bool ret = ImGui::Button(label, size);
    ImGui::PopStyleColor();
    return ret;
}
bool BeginHelpPopup(const char* name);

///////////////////////////////////////////////////////////////////////////////

inline double NiceNum(double x, bool round)
{
    int expv;  /* exponent of x */
    double f;  /* fractional part of x */
    double nf; /* nice, rounded fraction */
    expv = floor(log10(x));
    f = x / std::pow(10, expv); /* between 1 and 10 */
    if (round)
        if (f < 1.5)
            nf = 1;
        else if (f < 3)
            nf = 2;
        else if (f < 7)
            nf = 5;
        else
            nf = 10;
    else if (f <= 1)
        nf = 1;
    else if (f <= 2)
        nf = 2;
    else if (f <= 5)
        nf = 5;
    else
        nf = 10;
    return nf * std::pow(10, expv);
}

} // namespace ImGui