#define IMGUI_DEFINE_MATH_OPERATORS

#include "Custom.hpp"
#include <sstream>

namespace ImGui
{

// bool MiniSliderScalar(const char *label, ImGuiDataType data_type, void *p_data, const void *p_min, const void *p_max, bool top, float power)
// {
//     ImGuiWindow *window = GetCurrentWindow();
//     if (window->SkipItems)
//         return false;

//     ImGuiContext &g = *GImGui;
//     const ImGuiStyle &style = g.Style;
//     const ImGuiID id = window->GetID(label);
//     const float w = CalcItemWidth();

//     const ImVec2 label_size = CalcTextSize(label, NULL, true);
//     const float h = (label_size.y + style.FramePadding.y * 2.0f);
//     const ImVec2 topLeft = top ? window->DC.CursorPos : window->DC.CursorPos + ImVec2(0, (0.5f * h + 0.5 * style.FramePadding.y));
//     const ImRect frame_bb(topLeft, topLeft + ImVec2(w, (h * 0.5f - 0.5f * style.FramePadding.y)));
//     const ImRect total_bb(frame_bb.Min, frame_bb.Max);

//     ItemSize(total_bb, style.FramePadding.y);
//     if (!ItemAdd(total_bb, id, &frame_bb))
//         return false;

//     // Tabbing or CTRL-clicking on Slider turns it into an input box
//     const bool hovered = ItemHoverable(frame_bb, id);
//     const bool focus_requested = FocusableItemRegister(window, id);
//     const bool clicked = (hovered && g.IO.MouseClicked[0]);
//     if (focus_requested || clicked || g.NavActivateId == id || g.NavInputId == id)
//     {
//         SetActiveID(id, window);
//         SetFocusID(id, window);
//         FocusWindow(window);
//         g.ActiveIdUsingNavDirMask |= (1 << ImGuiDir_Left) | (1 << ImGuiDir_Right);
//     }

//     // Draw frame
//     const ImU32 frame_col = GetColorU32(g.ActiveId == id ? ImGuiCol_FrameBgActive : g.HoveredId == id ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg);
//     RenderNavHighlight(frame_bb, id);
//     RenderFrame(frame_bb.Min, frame_bb.Max, frame_col, true, g.Style.FrameRounding);

//     // Slider behavior
//     ImRect grab_bb;
//     const bool value_changed = SliderBehavior(frame_bb, id, data_type, p_data, p_min, p_max, "", power, ImGuiSliderFlags_None, &grab_bb);
//     if (value_changed)
//         MarkItemEdited(id);

//     // Render grab
//     if (grab_bb.Max.x > grab_bb.Min.x)
//         window->DrawList->AddRectFilled(grab_bb.Min, grab_bb.Max, GetColorU32(g.ActiveId == id ? ImGuiCol_SliderGrabActive : ImGuiCol_SliderGrab), style.GrabRounding);

//     return value_changed;
// }

// bool MiniSliderFloat(const char *label, float *v, float v_min, float v_max, bool top, float power)
// {
//     return MiniSliderScalar(label, ImGuiDataType_Float, v, &v_min, &v_max, top, power);
// }

void RenderGrid(ImRect bb, int nx, int ny, ImU32 gridColor, ImU32 bgColor, float thickness, float rounding)
{
    ImDrawList *DrawList = GetWindowDrawList();
    float dx = bb.GetWidth() / nx;
    float dy = bb.GetHeight() / ny;
    DrawList->AddRectFilled(bb.Min, bb.Max, bgColor, rounding);
    for (int ix = 0; ix < nx + 1; ++ix)
    {
        ImVec2 t = bb.Min + ImVec2(ix * dx, 0);
        ImVec2 b = t;
        b.y += bb.GetHeight();
        DrawList->AddLine(t, b, gridColor, thickness);
    }
    for (int iy = 0; iy < ny + 1; ++iy)
    {
        ImVec2 l = bb.Min + ImVec2(0, iy * dy);
        ImVec2 r = l;
        l.x += bb.GetWidth();
        DrawList->AddLine(l, r, gridColor, thickness);
    }
}

void PlotSignal(const char *label, const tact::Signal &sig, std::vector<ImVec2> &points, float t1, float t2, ImVec4 color, float thickness, ImVec2 size, bool grid, bool text)
{
    ImGuiWindow *window = GetCurrentWindow();
    if (window->SkipItems)
        return;
    ImGuiContext &g = *GImGui;
    const ImGuiStyle &style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImGuiIO &IO = GetIO();
    ImDrawList *DrawList = GetWindowDrawList();
    size = ImGui::CalcItemSize(size, 200, 200);
    const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + size);
    ItemSize(frame_bb, style.FramePadding.y);
    if (!ItemAdd(frame_bb, 0, &frame_bb))
        return;
    const bool hovered = ItemHoverable(frame_bb, id);
    if (grid)
        RenderGrid(frame_bb, 10, 8, GetColorU32(ImGuiCol_WindowBg), GetColorU32(ImGuiCol_FrameBg), 1.0f, style.FrameRounding);
    else
        RenderFrame(frame_bb.Min, frame_bb.Max, GetColorU32(ImGuiCol_FrameBg), true, style.FrameRounding);
    const ImVec2 start = {frame_bb.Min.x, frame_bb.GetCenter().y};
    const float dx = frame_bb.GetWidth() / points.size();
    const float ys = -frame_bb.GetHeight() * 0.5f * 0.95f;

    float dt = (t2 - t1) / points.size();
    float t = t1;
    float x = start.x;
    for (int i = 0; i < points.size(); ++i)
    {
        float s = ImClamp((float)sig.sample(t), -1.0f, 1.0f);
        float y = start.y + s * ys;
        points[i].x = x;
        points[i].y = y;
        t += dt;
        x += dx;
    }
    
    // DrawList->Flags &= ~ImDrawListFlags_AntiAliasedLines;
    // DrawList->AddPolyline(&points[0], (int)points.size(), ImGui::ColorConvertFloat4ToU32(color), false, thickness);
    auto col32 = ImGui::ColorConvertFloat4ToU32(color);
    for (int i = 0; i < points.size()-1; ++i)
        DrawList->AddLine(points[i], points[i+1], col32, thickness);
    // DrawList->Flags |= ImDrawListFlags_AntiAliasedLines;

    if (text) {
        std::string txt;
        std::stringstream ss;
        ss << sig.length();
        if (sig.length() != tact::INF)
            ss << " s";
        txt = ss.str();
        ImVec2 txtSize = CalcTextSize(txt.c_str());
        RenderText(frame_bb.Max - txtSize - style.FramePadding, txt.c_str());
    }
}

void RenderSignalInBounds(ImDrawList* DrawList, const tact::Signal& sig, float t1, float t2, ImRect bb, ImVec4 color, float thickness, int n) {
    static std::vector<ImVec2> buffer;
    if (n == 0)
        n = (int)bb.GetWidth() * 2;
    buffer.resize(n);
    const ImVec2 start = {bb.Min.x, bb.GetCenter().y};
    const float dx = bb.GetWidth() / buffer.size();
    const float ys = -bb.GetHeight() * 0.5f * 0.95f;
    float dt = (t2 - t1) / buffer.size();
    float t = t1;
    float x = start.x;
    for (int i = 0; i < buffer.size(); ++i)
    {
        float s = ImClamp((float)sig.sample(t), -1.0f, 1.0f);
        float y = start.y + s * ys;
        buffer[i].x = x;
        buffer[i].y = y;
        t += dt;
        x += dx;
    }
    
    DrawList->PushClipRect(bb.Min, bb.Max, true);
    // DrawList->Flags &= ~ImDrawListFlags_AntiAliasedLines;
    // DrawList->AddPolyline(&buffer[0], n, ImGui::GetColorU32(color), false, thickness);
    auto col32 = ImGui::ColorConvertFloat4ToU32(color);
    for (int i = 0; i < buffer.size()-1; ++i)
        DrawList->AddLine(buffer[i], buffer[i+1], col32, thickness);
    // DrawList->Flags |= ImDrawListFlags_AntiAliasedLines;
    DrawList->PopClipRect();
}

///////////////////////////////////////////////////////////////////////////////

bool TimelineScrollbar(float* ltime, float* rtime, bool* lGrabbed, bool* rGrabbed, bool* cGrabbed, ImVec2 size, float grabWidth) {
    bool changed = false;
    // skip render if SkipItems on
    ImGuiWindow *window = GetCurrentWindow();
    if (window->SkipItems)
        return false;
    // get ImGuiContext and Style
    ImGuiContext &g = *GImGui;
    const ImGuiStyle &style = g.Style;
    const ImGuiIO &IO = GetIO();
    ImDrawList *DrawList = GetWindowDrawList();

    size = CalcItemSize(size, 100, style.ScrollbarSize);

    const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + size);
    const ImRect inner_bb(frame_bb.Min + style.FramePadding, frame_bb.Max - style.FramePadding);
    const ImVec2 grab_offset(3,3);

    ItemSize(frame_bb);

    // centerline
    const ImVec2 sMin(frame_bb.Min.x + grab_offset.x + grabWidth * 0.5f, frame_bb.GetCenter().y);
    const ImVec2 sMax(frame_bb.Max.x - grab_offset.x - grabWidth * 0.5f, frame_bb.GetCenter().y);

    ImVec2 lPos = ImLerp(sMin, sMax, *ltime);
    ImVec2 rPos = ImLerp(sMin, sMax, *rtime);
    ImVec2 lMin = sMin;
    ImVec2 rMin = lPos + ImVec2(2 * grabWidth,0);
    ImVec2 lMax = rPos - ImVec2(2 * grabWidth,0);
    ImVec2 rMax = sMax;

    // grab center    
    ImVec2 grab_half = ImVec2(grabWidth, frame_bb.GetHeight() - 2 * grab_offset.y) * 0.5f;

    const ImRect cGrab_bb(lPos - grab_half, rPos + grab_half);

    // draw grab ends
    ImRect lGrab_bb(lPos - grab_half, lPos + grab_half);
    ImRect rGrab_bb(rPos - grab_half, rPos + grab_half);

    bool lHovered = lGrab_bb.Contains(IO.MousePos);
    bool rHovered = lHovered ? false : rGrab_bb.Contains(IO.MousePos);
    bool cHovered = lHovered || rHovered ? false : cGrab_bb.Contains(IO.MousePos);

    if (lHovered || *lGrabbed) {
        if (IO.MouseClicked[0])
            *lGrabbed = true;  
        if (IO.MouseDoubleClicked[0]) {
            lPos.x = lMin.x;          
            changed = true;
        }
        if (*lGrabbed) {
            lPos.x += IO.MouseDelta.x;
            lPos.x = ImClamp(lPos.x, lMin.x, lMax.x);
            *ltime = (lPos.x - sMin.x) / (sMax.x - sMin.x);
            rHovered = false; cHovered = false;
            changed = true;
        }
    }
    else if (rHovered || *rGrabbed) {
        if (IO.MouseClicked[0])
            *rGrabbed = true;   
        if (IO.MouseDoubleClicked[0]) {
            rPos.x = rMax.x;     
            changed = true;
        }
        if (*rGrabbed) {
            rPos.x += IO.MouseDelta.x;
            rPos.x = ImClamp(rPos.x, rMin.x, rMax.x);
            *rtime = (rPos.x - sMin.x) / (sMax.x - sMin.x);
            lHovered = false; cHovered = false;
            changed = true;
        }
    }
    else if (cHovered || *cGrabbed) {
        if (IO.MouseClicked[0])
            *cGrabbed = true;
        if (IO.MouseDoubleClicked[0]) {
            lPos.x = lMin.x;          
            rPos.x = rMax.x;     
            changed = true;
        }
        if (*cGrabbed) {
            float shift = ImClamp(IO.MouseDelta.x, sMin.x - lPos.x, sMax.x - rPos.x);
            lPos.x += shift;
            rPos.x += shift;
            *ltime = (lPos.x - sMin.x) / (sMax.x - sMin.x);
            *rtime = (rPos.x - sMin.x) / (sMax.x - sMin.x);
            lHovered = false; rHovered = false;
            changed = true;
        }
    }
    if (IO.MouseReleased[0]) {
        *lGrabbed = false;
        *rGrabbed = false;
        *cGrabbed = false;
    }
    // render
    DrawList->AddRectFilled(frame_bb.Min, frame_bb.Max, GetColorU32(ImGuiCol_ScrollbarBg), style.ChildRounding);
    DrawList->AddRectFilled(cGrab_bb.Min, cGrab_bb.Max, *cGrabbed ? GetColorU32(ImGuiCol_ScrollbarGrabActive) : cHovered ? GetColorU32(ImGuiCol_ScrollbarGrabHovered,0.75f) : GetColorU32(ImGuiCol_ScrollbarGrab), style.ScrollbarRounding);
    DrawList->AddRectFilled(lGrab_bb.Min, lGrab_bb.Max, *lGrabbed ? GetColorU32(ImGuiCol_ScrollbarGrabActive) : lHovered ? GetColorU32(ImGuiCol_ScrollbarGrabHovered) : GetColorU32(ImGuiCol_ScrollbarGrabHovered, 0.5f), style.ScrollbarRounding);
    DrawList->AddRectFilled(rGrab_bb.Min, rGrab_bb.Max, *rGrabbed ? GetColorU32(ImGuiCol_ScrollbarGrabActive) : rHovered ? GetColorU32(ImGuiCol_ScrollbarGrabHovered) : GetColorU32(ImGuiCol_ScrollbarGrabHovered, 0.5f), style.ScrollbarRounding);

    return changed;
}

bool BeginHelpPopup(const char* name)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    const ImGuiID id = window->GetID(name);
    if (!IsPopupOpen(name))
    {
        g.NextWindowData.ClearFlags(); // We behave like Begin() and need to consume those values
        return false;
    }

    // Center modal windows by default
    // FIXME: Should test for (PosCond & window->SetWindowPosAllowFlags) with the upcoming window.
    if ((g.NextWindowData.Flags & ImGuiNextWindowDataFlags_HasPos) == 0)
    {
        ImGuiViewportP* viewport = window->WasActive ? window->Viewport : (ImGuiViewportP*)GetMainViewport(); // FIXME-VIEWPORT: What may be our reference viewport?
        SetNextWindowPos(viewport->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    }

    ImGuiWindowFlags flags = ImGuiWindowFlags_Popup | ImGuiWindowFlags_Modal | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoResize;
    bool p_open = true;
    const bool is_open = Begin(name, &p_open, flags);
    if (!is_open || (!p_open)) // NB: is_open can be 'false' when the popup is completely clipped (e.g. zero size display)
    {
        EndPopup();
        if (is_open)
            ClosePopupToLevel(g.BeginPopupStack.Size, true);
        return false;
    }
    return is_open;
}




} // namespace ImGui