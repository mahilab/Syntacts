#pragma once
#include <imgui.h>
#include <imgui_internal.h>
#include <vector>
#include <string>
#include <syntacts>

namespace ImGui {

///////////////////////////////////////////////////////////////////////////////

bool MiniSliderScalar(const char* label, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, bool top, float power);
bool MiniSliderFloat(const char* label, float* v, float v_min, float v_max, bool top = true, float power = 1.0f);

void RenderGrid(ImRect bb, int nx, int ny, ImU32 gridColor, ImU32 bgColor, float thickness = 1.0f, float rounding = 0);
void PlotSignal(const char* label,  const tact::Signal& sig, std::vector<ImVec2>& points, float t1, float t2, ImVec4 color, float thickness, ImVec2 size = ImVec2(-1,0), bool grid = true, bool text = true);
void RenderSignalInBounds(ImDrawList* DrawList, const tact::Signal& sig, float t1, float t2, ImRect bb, ImVec4 color, float thickness, int n = 0);


///////////////////////////////////////////////////////////////////////////////

bool TimelineScrollbar(float* ltime, float* rtime, bool* lrgrabbed, bool* rgrabbed, bool* cgrabbed, ImVec2 size = ImVec2(-1, 0), float grabWidth = 10);

///////////////////////////////////////////////////////////////////////////////

/// Begins a fixed size window without a header
inline void BeginFixed(const char* name, const ImVec2& pos, const ImVec2& size) {
    // ImGui::SetNextWindowPos(pos, ImGuiCond_Always);
    // ImGui::SetNextWindowSize(size, ImGuiCond_Always);
    // ImGui::Begin(name, nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
    auto posV = ImGui::GetMainViewport()->Pos;
    posV.x += pos.x;
    posV.y += pos.y;
    ImGui::SetNextWindowPos(posV, ImGuiCond_Appearing);
    ImGui::SetNextWindowSize(size, ImGuiCond_Appearing);
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
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.25f);
    }
}

/// Ends a disabled section
inline void EndDisabled(bool disabled = true) {
    if (disabled) {
        ImGui::PopItemFlag();
        ImGui::PopStyleVar();
    }
}

inline bool TintedButton(const char* label, ImVec4 color, const ImVec2& size = ImVec2(0,0)) {
    ImGui::PushStyleColor(ImGuiCol_Button, color);
    bool ret = ImGui::Button(label, size);
    ImGui::PopStyleColor();
    return ret;
}

inline void EnableButton(const char* label, bool* enabled, const ImVec2& size = ImVec2(0,0)) {
    bool dim = !*enabled;
    if (dim)
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.25f);
    if (ImGui::Button(label, size))
        *enabled = !(*enabled);
    if (dim)
        ImGui::PopStyleVar();
}

inline void ToggleButton(const char* str_id, bool* v)
{
    ImVec2 p = ImGui::GetCursorScreenPos();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    float height = ImGui::GetFrameHeight();
    float width = height * 1.55f;
    float radius = height * 0.50f;
    ImGui::InvisibleButton(str_id, ImVec2(width, height));
    if (ImGui::IsItemClicked())
        *v = !*v;

    float t = *v ? 1.0f : 0.0f;

    ImGuiContext& g = *GImGui;
    float ANIM_SPEED = 0.08f;
    if (g.LastActiveId == g.CurrentWindow->GetID(str_id))// && g.LastActiveIdTimer < ANIM_SPEED)
    {
        float t_anim = ImSaturate(g.LastActiveIdTimer / ANIM_SPEED);
        t = *v ? (t_anim) : (1.0f - t_anim);
    }

    ImU32 col_bg;
    if (ImGui::IsItemHovered())
        col_bg = ImGui::GetColorU32(ImLerp(ImVec4(0.78f, 0.78f, 0.78f, 1.0f), ImVec4(0.64f, 0.83f, 0.34f, 1.0f), t));
    else
        col_bg = ImGui::GetColorU32(ImLerp(ImVec4(0.85f, 0.85f, 0.85f, 1.0f), ImVec4(0.56f, 0.83f, 0.26f, 1.0f), t));

    draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), col_bg, height * 0.5f);
    draw_list->AddCircleFilled(ImVec2(p.x + radius + t * (width - radius * 2.0f), p.y + radius), radius - 1.5f, IM_COL32(255, 255, 255, 255));
}

bool BeginHelpPopup(const char* name);

///////////////////////////////////////////////////////////////////////////////

inline float Remap(float x, float x0, float x1, float y0, float y1) {
    return y0 + (x - x0) * (y1 - y0) / (x1 - x0);
}

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
    return nf * std::pow(10., expv);
}

} // namespace ImGui