#define IMGUI_DEFINE_MATH_OPERATORS

#include <Engine/ImGui/imgui.h>
#include <Engine/ImGui/imgui_internal.h>
#include "CustomWidgets.hpp"

namespace ImGui {

bool MiniSliderScalar(const char* label, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, bool top, float power)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const float w = CalcItemWidth();


    const ImVec2 label_size = CalcTextSize(label, NULL, true);
    const float h = (label_size.y + style.FramePadding.y * 2.0f);
    const ImVec2 topLeft = top ? window->DC.CursorPos : window->DC.CursorPos + ImVec2(0, 0.5 * h + style.ItemSpacing.y * 0.5f);
    const ImRect frame_bb(topLeft, topLeft + ImVec2(w, h * 0.5f - style.ItemSpacing.y * 0.5f) );
    const ImRect total_bb(frame_bb.Min, frame_bb.Max);

    ItemSize(total_bb, style.FramePadding.y);
    if (!ItemAdd(total_bb, id, &frame_bb))
        return false;

    // Tabbing or CTRL-clicking on Slider turns it into an input box
    const bool hovered = ItemHoverable(frame_bb, id);
    const bool focus_requested = FocusableItemRegister(window, id);
    const bool clicked = (hovered && g.IO.MouseClicked[0]);
    if (focus_requested || clicked || g.NavActivateId == id || g.NavInputId == id)
    {
        SetActiveID(id, window);
        SetFocusID(id, window);
        FocusWindow(window);
        g.ActiveIdUsingNavDirMask |= (1 << ImGuiDir_Left) | (1 << ImGuiDir_Right);
    }    

    // Draw frame
    const ImU32 frame_col = GetColorU32(g.ActiveId == id ? ImGuiCol_FrameBgActive : g.HoveredId == id ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg);
    RenderNavHighlight(frame_bb, id);
    RenderFrame(frame_bb.Min, frame_bb.Max, frame_col, true, g.Style.FrameRounding);

    // Slider behavior
    ImRect grab_bb;
    const bool value_changed = SliderBehavior(frame_bb, id, data_type, p_data, p_min, p_max, "", power, ImGuiSliderFlags_None, &grab_bb);
    if (value_changed)
        MarkItemEdited(id);

    // Render grab
    if (grab_bb.Max.x > grab_bb.Min.x)
        window->DrawList->AddRectFilled(grab_bb.Min, grab_bb.Max, GetColorU32(g.ActiveId == id ? ImGuiCol_SliderGrabActive : ImGuiCol_SliderGrab), style.GrabRounding);

    return value_changed;
}


bool MiniSliderFloat(const char* label, float* v, float v_min, float v_max, bool top, float power)
{
    return MiniSliderScalar(label, ImGuiDataType_Float, v, &v_min, &v_max, top, power);
}

} // namespace ImGui