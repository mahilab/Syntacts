#ifndef IMGUI_DEFINE_MATH_OPERATORS
    #define IMGUI_DEFINE_MATH_OPERATORS
#endif

#include <carnot>
#include <Syntacts/Syntacts.hpp>
#include <deque>
#include <functional>
#include <chrono>
#include <ctime>  
#include <string>
#include <Engine/ImGui/imgui_internal.h>

//==============================================================================
// MISC
//==============================================================================

// renders an ImGui tooltip
void tooltip(const std::string& tip) {
    if (ImGui::IsItemHovered()) 
        ImGui::SetTooltip(tip.c_str());
}

//==============================================================================
// IMGUI CUSTOM PLOTTING
//==============================================================================

namespace ImGui {

void PlotEx2(ImGuiPlotType plot_type, const char* label, float (*values_getter)(void* data, int idx), void* data1, void* data2, int values_count, int values_offset, const char* overlay_text, float scale_min, float scale_max, ImVec2 graph_size)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;

    const ImVec2 label_size = CalcTextSize(label, NULL, true);
    if (graph_size.x == 0.0f)
        graph_size.x = CalcItemWidth();
    if (graph_size.y == 0.0f)
        graph_size.y = label_size.y + (style.FramePadding.y * 2);

    const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(graph_size.x, graph_size.y));
    const ImRect inner_bb(frame_bb.Min + style.FramePadding, frame_bb.Max - style.FramePadding);
    const ImRect total_bb(frame_bb.Min, frame_bb.Max + ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0));
    ItemSize(total_bb, style.FramePadding.y);
    if (!ItemAdd(total_bb, 0, &frame_bb))
        return;

    // Determine scale from values if not specified
    if (scale_min == FLT_MAX || scale_max == FLT_MAX)
    {
        float v_min = FLT_MAX;
        float v_max = -FLT_MAX;
        for (int i = 0; i < values_count; i++)
        {
            const float v = values_getter(data1, i);
            v_min = ImMin(v_min, v);
            v_max = ImMax(v_max, v);
        }
        if (scale_min == FLT_MAX)
            scale_min = v_min;
        if (scale_max == FLT_MAX)
            scale_max = v_max;
    }

    RenderFrame(frame_bb.Min, frame_bb.Max, GetColorU32(ImGuiCol_FrameBg), true, style.FrameRounding);

    if (values_count > 0)
    {
        int res_w = ImMin((int)graph_size.x, values_count) + ((plot_type == ImGuiPlotType_Lines) ? -1 : 0);
        int item_count = values_count + ((plot_type == ImGuiPlotType_Lines) ? -1 : 0);

        const float t_step = 1.0f / (float)res_w;
        const float inv_scale = (scale_min == scale_max) ? 0.0f : (1.0f / (scale_max - scale_min));

        float v0 = values_getter(data1, (0 + values_offset) % values_count);
        float t0 = 0.0f;
        ImVec2 tp0 = ImVec2( t0, 1.0f - ImSaturate((v0 - scale_min) * inv_scale) );                       // Point in the normalized space of our target rectangle
        float histogram_zero_line_t = (scale_min * scale_max < 0.0f) ? (-scale_min * inv_scale) : (scale_min < 0.0f ? 0.0f : 1.0f);   // Where does the zero line stands

        const ImU32 col_base = GetColorU32(ImGuiCol_PlotLines);
        for (int n = 0; n < res_w; n++)
        {
            const float t1 = t0 + t_step;
            const int v1_idx = (int)(t0 * item_count + 0.5f);
            IM_ASSERT(v1_idx >= 0 && v1_idx < values_count);
            const float v1 = values_getter(data1, (v1_idx + values_offset + 1) % values_count);
            const ImVec2 tp1 = ImVec2( t1, 1.0f - ImSaturate((v1 - scale_min) * inv_scale) );

            // NB: Draw calls are merged together by the DrawList system. Still, we should render our batch are lower level to save a bit of CPU.
            ImVec2 pos0 = ImLerp(inner_bb.Min, inner_bb.Max, tp0);
            ImVec2 pos1 = ImLerp(inner_bb.Min, inner_bb.Max, tp1);
            window->DrawList->AddLine(pos0, pos1, col_base); 

            t0 = t1;
            tp0 = tp1;
        }
    }

    if (values_count > 0)
    {
        int res_w = ImMin((int)graph_size.x, values_count) + ((plot_type == ImGuiPlotType_Lines) ? -1 : 0);
        int item_count = values_count + ((plot_type == ImGuiPlotType_Lines) ? -1 : 0);

        const float t_step = 1.0f / (float)res_w;
        const float inv_scale = (scale_min == scale_max) ? 0.0f : (1.0f / (scale_max - scale_min));

        float v0 = values_getter(data2, (0 + values_offset) % values_count);
        float t0 = 0.0f;
        ImVec2 tp0 = ImVec2( t0, 1.0f - ImSaturate((v0 - scale_min) * inv_scale) );                       // Point in the normalized space of our target rectangle
        float histogram_zero_line_t = (scale_min * scale_max < 0.0f) ? (-scale_min * inv_scale) : (scale_min < 0.0f ? 0.0f : 1.0f);   // Where does the zero line stands

        const ImU32 col_base = GetColorU32(carnot::withAlpha(carnot::Whites::White,0.1f));
        for (int n = 0; n < res_w; n++)
        {
            const float t1 = t0 + t_step;
            const int v1_idx = (int)(t0 * item_count + 0.5f);
            IM_ASSERT(v1_idx >= 0 && v1_idx < values_count);
            const float v1 = values_getter(data2, (v1_idx + values_offset + 1) % values_count);
            const ImVec2 tp1 = ImVec2( t1, 1.0f - ImSaturate((v1 - scale_min) * inv_scale) );

            // NB: Draw calls are merged together by the DrawList system. Still, we should render our batch are lower level to save a bit of CPU.
            ImVec2 pos0 = ImLerp(inner_bb.Min, inner_bb.Max, tp0);
            ImVec2 pos1 = ImLerp(inner_bb.Min, inner_bb.Max, tp1);
            window->DrawList->AddLine(pos0, pos1, col_base); 

            t0 = t1;
            tp0 = tp1;
        }
    }

    // Text overlay
    if (overlay_text)
        RenderTextClipped(ImVec2(frame_bb.Min.x, frame_bb.Min.y + style.FramePadding.y), frame_bb.Max, overlay_text, NULL, NULL, ImVec2(0.5f,0.0f));

    if (label_size.x > 0.0f)
        RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, inner_bb.Min.y), label);
}

struct ImGuiPlotArrayGetterData2
{
    const float* Values;
    int Stride;
    ImGuiPlotArrayGetterData2(const float* values, int stride) { Values = values; Stride = stride; }
};

static float Plot_ArrayGetter2(void* data, int idx)
{
    ImGuiPlotArrayGetterData2* plot_data = (ImGuiPlotArrayGetterData2*)data;
    const float v = *(const float*)(const void*)((const unsigned char*)plot_data->Values + (size_t)idx * plot_data->Stride);
    return v;
}

void PlotLines2(const char* label, const float* values1, const float* values2, int values_count, int values_offset = 0, const char* overlay_text = NULL, float scale_min = FLT_MAX, float scale_max = FLT_MAX, ImVec2 graph_size = ImVec2(0, 0), int stride = sizeof(float)) {
    ImGuiPlotArrayGetterData2 data1(values1, stride);
    ImGuiPlotArrayGetterData2 data2(values2, stride);
    PlotEx2(ImGuiPlotType_Lines, label, &Plot_ArrayGetter2, (void*)&data1, (void*)&data2, values_count, values_offset, overlay_text, scale_min, scale_max, graph_size);
}

 template<int steps>
    void bezier_table(ImVec2 P[4], ImVec2 results[steps + 1]) {
        static float C[(steps + 1) * 4], *K = 0;
        if (!K) {
            K = C;
            for (unsigned step = 0; step <= steps; ++step) {
                float t = (float) step / (float) steps;
                C[step * 4 + 0] = (1 - t)*(1 - t)*(1 - t);   // * P0
                C[step * 4 + 1] = 3 * (1 - t)*(1 - t) * t; // * P1
                C[step * 4 + 2] = 3 * (1 - t) * t*t;     // * P2
                C[step * 4 + 3] = t * t*t;               // * P3
            }
        }
        for (unsigned step = 0; step <= steps; ++step) {
            ImVec2 point = {
                K[step * 4 + 0] * P[0].x + K[step * 4 + 1] * P[1].x + K[step * 4 + 2] * P[2].x + K[step * 4 + 3] * P[3].x,
                K[step * 4 + 0] * P[0].y + K[step * 4 + 1] * P[1].y + K[step * 4 + 2] * P[2].y + K[step * 4 + 3] * P[3].y
            };
            results[step] = point;
        }
    }

    float BezierValue(float dt01, float P[4]) {
        enum { STEPS = 256 };
        ImVec2 Q[4] = { { 0, 0 }, { P[0], P[1] }, { P[2], P[3] }, { 1, 1 } };
        ImVec2 results[STEPS + 1];
        bezier_table<STEPS>(Q, results);
        return results[(int) ((dt01 < 0 ? 0 : dt01 > 1 ? 1 : dt01) * STEPS)].y;
    }

    int Bezier(const char *label, float P[5]) {
        // visuals
        enum { SMOOTHNESS = 64 }; // curve smoothness: the higher number of segments, the smoother curve
        enum { CURVE_WIDTH = 4 }; // main curved line width
        enum { LINE_WIDTH = 1 }; // handlers: small lines width
        enum { GRAB_RADIUS = 6 }; // handlers: circle radius
        enum { GRAB_BORDER = 2 }; // handlers: circle border width
        enum { AREA_CONSTRAINED = false }; // should grabbers be constrained to grid area?
        enum { AREA_WIDTH = 256 }; // area width in pixels. 0 for adaptive size (will use max avail width)

        // curve presets
        static struct { const char *name; float points[4]; } presets [] = {
            { "Linear", 0.000f, 0.000f, 1.000f, 1.000f },

            { "In Sine", 0.470f, 0.000f, 0.745f, 0.715f },
            { "In Quad", 0.550f, 0.085f, 0.680f, 0.530f },
            { "In Cubic", 0.550f, 0.055f, 0.675f, 0.190f },
            { "In Quart", 0.895f, 0.030f, 0.685f, 0.220f },
            { "In Quint", 0.755f, 0.050f, 0.855f, 0.060f },
            { "In Expo", 0.950f, 0.050f, 0.795f, 0.035f },
            { "In Circ", 0.600f, 0.040f, 0.980f, 0.335f },
            { "In Back", 0.600f, -0.28f, 0.735f, 0.045f },

            { "Out Sine", 0.390f, 0.575f, 0.565f, 1.000f },
            { "Out Quad", 0.250f, 0.460f, 0.450f, 0.940f },
            { "Out Cubic", 0.215f, 0.610f, 0.355f, 1.000f },
            { "Out Quart", 0.165f, 0.840f, 0.440f, 1.000f },
            { "Out Quint", 0.230f, 1.000f, 0.320f, 1.000f },
            { "Out Expo", 0.190f, 1.000f, 0.220f, 1.000f },
            { "Out Circ", 0.075f, 0.820f, 0.165f, 1.000f },
            { "Out Back", 0.175f, 0.885f, 0.320f, 1.275f },

            { "InOut Sine", 0.445f, 0.050f, 0.550f, 0.950f },
            { "InOut Quad", 0.455f, 0.030f, 0.515f, 0.955f },
            { "InOut Cubic", 0.645f, 0.045f, 0.355f, 1.000f },
            { "InOut Quart", 0.770f, 0.000f, 0.175f, 1.000f },
            { "InOut Quint", 0.860f, 0.000f, 0.070f, 1.000f },
            { "InOut Expo", 1.000f, 0.000f, 0.000f, 1.000f },
            { "InOut Circ", 0.785f, 0.135f, 0.150f, 0.860f },
            { "InOut Back", 0.680f, -0.55f, 0.265f, 1.550f },

            // easeInElastic: not a bezier
            // easeOutElastic: not a bezier
            // easeInOutElastic: not a bezier
            // easeInBounce: not a bezier
            // easeOutBounce: not a bezier
            // easeInOutBounce: not a bezier
        };


        // preset selector

        bool reload = 0;
        ImGui::PushID(label);
        if (ImGui::ArrowButton("##lt", ImGuiDir_Left)) { // ImGui::ArrowButton(ImGui::GetCurrentWindow()->GetID("##lt"), ImGuiDir_Left, ImVec2(0, 0), 0)
            if (--P[4] >= 0) reload = 1; else ++P[4];
        }
        ImGui::SameLine();

        if (ImGui::Button("Presets")) {
            ImGui::OpenPopup("!Presets");
        }
        if (ImGui::BeginPopup("!Presets")) {
            for (int i = 0; i < IM_ARRAYSIZE(presets); ++i) {
                if( i == 1 || i == 9 || i == 17 ) ImGui::Separator();
                if (ImGui::MenuItem(presets[i].name, NULL, P[4] == i)) {
                    P[4] = i;
                    reload = 1;
                }
            }
            ImGui::EndPopup();
        }
        ImGui::SameLine();

        if (ImGui::ArrowButton("##rt", ImGuiDir_Right)) { // ImGui::ArrowButton(ImGui::GetCurrentWindow()->GetID("##rt"), ImGuiDir_Right, ImVec2(0, 0), 0)
            if (++P[4] < IM_ARRAYSIZE(presets)) reload = 1; else --P[4];
        }
        ImGui::SameLine();
        ImGui::PopID();

        if (reload) {
            memcpy(P, presets[(int) P[4]].points, sizeof(float) * 4);
        }

        // bezier widget

        const ImGuiStyle& Style = GetStyle();
        const ImGuiIO& IO = GetIO();
        ImDrawList* DrawList = GetWindowDrawList();
        ImGuiWindow* Window = GetCurrentWindow();
        if (Window->SkipItems)
            return false;

        // header and spacing
        int changed = SliderFloat4(label, P, 0, 1, "%.3f", 1.0f);
        int hovered = IsItemActive() || IsItemHovered(); // IsItemDragged() ?
        Dummy(ImVec2(0, 3));

        // prepare canvas
        const float avail = GetContentRegionAvailWidth();
        const float dim = AREA_WIDTH > 0 ? AREA_WIDTH : avail;
        ImVec2 Canvas(dim, dim);

        ImRect bb(Window->DC.CursorPos, Window->DC.CursorPos + Canvas);
        ItemSize(bb);
        if (!ItemAdd(bb, NULL))
            return changed;

        const ImGuiID id = Window->GetID(label);
        hovered |= 0 != ItemHoverable(ImRect(bb.Min, bb.Min + ImVec2(avail, dim)), id);

        RenderFrame(bb.Min, bb.Max, GetColorU32(ImGuiCol_FrameBg, 1), true, Style.FrameRounding);

        // background grid
        for (int i = 0; i <= Canvas.x; i += (Canvas.x / 4)) {
            DrawList->AddLine(
                ImVec2(bb.Min.x + i, bb.Min.y),
                ImVec2(bb.Min.x + i, bb.Max.y),
                GetColorU32(ImGuiCol_TextDisabled));
        }
        for (int i = 0; i <= Canvas.y; i += (Canvas.y / 4)) {
            DrawList->AddLine(
                ImVec2(bb.Min.x, bb.Min.y + i),
                ImVec2(bb.Max.x, bb.Min.y + i),
                GetColorU32(ImGuiCol_TextDisabled));
        }

        // eval curve
        ImVec2 Q[4] = { { 0, 0 }, { P[0], P[1] }, { P[2], P[3] }, { 1, 1 } };
        ImVec2 results[SMOOTHNESS + 1];
        bezier_table<SMOOTHNESS>(Q, results);

        // control points: 2 lines and 2 circles
        {
            // handle grabbers
            ImVec2 mouse = GetIO().MousePos, pos[2];
            float distance[2];

            for (int i = 0; i < 2; ++i) {
                pos[i] = ImVec2(P[i * 2 + 0], 1 - P[i * 2 + 1]) * (bb.Max - bb.Min) + bb.Min;
                distance[i] = (pos[i].x - mouse.x) * (pos[i].x - mouse.x) + (pos[i].y - mouse.y) * (pos[i].y - mouse.y);
            }

            int selected = distance[0] < distance[1] ? 0 : 1;
            if( distance[selected] < (4*GRAB_RADIUS * 4*GRAB_RADIUS) )
            {
                SetTooltip("(%4.3f, %4.3f)", P[selected * 2 + 0], P[selected * 2 + 1]);

                if (/*hovered &&*/ (IsMouseClicked(0) || IsMouseDragging(0))) {
                    float &px = (P[selected * 2 + 0] += GetIO().MouseDelta.x / Canvas.x);
                    float &py = (P[selected * 2 + 1] -= GetIO().MouseDelta.y / Canvas.y);

                    if (AREA_CONSTRAINED) {
                        px = (px < 0 ? 0 : (px > 1 ? 1 : px));
                        py = (py < 0 ? 0 : (py > 1 ? 1 : py));
                    }

                    changed = true;
                }
            }
        }

        // if (hovered || changed) DrawList->PushClipRectFullScreen();

        // draw curve
        {
            ImColor color(GetStyle().Colors[ImGuiCol_PlotLines]);
            for (int i = 0; i < SMOOTHNESS; ++i) {
                ImVec2 p = { results[i + 0].x, 1 - results[i + 0].y };
                ImVec2 q = { results[i + 1].x, 1 - results[i + 1].y };
                ImVec2 r(p.x * (bb.Max.x - bb.Min.x) + bb.Min.x, p.y * (bb.Max.y - bb.Min.y) + bb.Min.y);
                ImVec2 s(q.x * (bb.Max.x - bb.Min.x) + bb.Min.x, q.y * (bb.Max.y - bb.Min.y) + bb.Min.y);
                DrawList->AddLine(r, s, color, CURVE_WIDTH);
            }
        }

        // draw preview (cycles every 1s)
        static clock_t epoch = clock();
        ImVec4 white(GetStyle().Colors[ImGuiCol_Text]);
        // for (int i = 0; i < 3; ++i) {
        //     double now = ((clock() - epoch) / (double)CLOCKS_PER_SEC);
        //     float delta = ((int) (now * 1000) % 1000) / 1000.f; delta += i / 3.f; if (delta > 1) delta -= 1;
        //     int idx = (int) (delta * SMOOTHNESS);
        //     float evalx = results[idx].x; // 
        //     float evaly = results[idx].y; // ImGui::BezierValue( delta, P );
        //     ImVec2 p0 = ImVec2(evalx, 1 - 0) * (bb.Max - bb.Min) + bb.Min;
        //     ImVec2 p1 = ImVec2(0, 1 - evaly) * (bb.Max - bb.Min) + bb.Min;
        //     ImVec2 p2 = ImVec2(evalx, 1 - evaly) * (bb.Max - bb.Min) + bb.Min;
        //     DrawList->AddCircleFilled(p0, GRAB_RADIUS / 2, ImColor(white));
        //     DrawList->AddCircleFilled(p1, GRAB_RADIUS / 2, ImColor(white));
        //     DrawList->AddCircleFilled(p2, GRAB_RADIUS / 2, ImColor(white));
        // }

        // draw lines and grabbers
        float luma = IsItemActive() || IsItemHovered() ? 0.5f : 1.0f;
        ImVec4 pink(1.00f, 0.00f, 0.75f, luma), cyan(0.00f, 0.75f, 1.00f, luma);
        ImVec2 p1 = ImVec2(P[0], 1 - P[1]) * (bb.Max - bb.Min) + bb.Min;
        ImVec2 p2 = ImVec2(P[2], 1 - P[3]) * (bb.Max - bb.Min) + bb.Min;
        DrawList->AddLine(ImVec2(bb.Min.x, bb.Max.y), p1, ImColor(white), LINE_WIDTH);
        DrawList->AddLine(ImVec2(bb.Max.x, bb.Min.y), p2, ImColor(white), LINE_WIDTH);
        DrawList->AddCircleFilled(p1, GRAB_RADIUS, ImColor(white));
        DrawList->AddCircleFilled(p1, GRAB_RADIUS - GRAB_BORDER, ImColor(pink));
        DrawList->AddCircleFilled(p2, GRAB_RADIUS, ImColor(white));
        DrawList->AddCircleFilled(p2, GRAB_RADIUS - GRAB_BORDER, ImColor(cyan));

        // if (hovered || changed) DrawList->PopClipRect();

        return changed;
    }

    void ShowBezierDemo() {
        { static float v[5] = { 0.950f, 0.050f, 0.795f, 0.035f }; Bezier("easeInExpo", v); }
    }

} // namespace imGui