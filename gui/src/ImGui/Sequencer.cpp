#define IMGUI_DEFINE_MATH_OPERATORS

#include "Sequencer.hpp"
#include "Custom.hpp"
#include <ImGui/imgui_internal.h>
#include <Engine/FontAwesome5.hpp>
#include "Helpers.hpp"
#include "../Nodes.hpp"

namespace ImGui
{

void RenderTicks(const ImRect& rect, float tMin, float tMax, float major, float mid, float minor, float low, ImU32 color, bool clip, bool text) {
    auto DrawList = GetWindowDrawList();
    int nticks = 4;

    const float high = std::min(0.0f, std::min(major, std::min(mid, minor)));
    const double range = NiceNum(tMax - tMin, 0);
    const double interval = NiceNum(range / (nticks - 1), 1);
    const double graphmin = floor(tMin / interval) * interval;
    const double graphmax = ceil(tMax / interval) * interval;
    const int nfrac = std::max((int)-floor(log10(interval)) , 0); /* # of fractional intervaligits to show */
    if (clip)
        DrawList->PushClipRect(rect.Min + ImVec2(0, high), rect.Max + ImVec2(0, low), true);
    for (double tTickMajor = graphmin; tTickMajor < graphmax + 0.5 * interval; tTickMajor += interval)
    {
        float xPos     = std::round(Remap(tTickMajor, tMin, tMax, rect.Min.x, rect.Max.x));
        float xPosNext = std::round(Remap(tTickMajor + interval, tMin, tMax, rect.Min.x, rect.Max.x));
        if (xPos < rect.Max.x) {
            ImVec2 p1((float)xPos, rect.Min.y + major);
            ImVec2 p2((float)xPos, rect.Max.y + low);        
            if (text) {
                std::stringstream stream;
                stream << tTickMajor;
                DrawList->AddText(ImVec2((float)xPos + 3, rect.Min.y),color,stream.str().c_str());
            }
            DrawList->AddLine(p1, p2,color);
            for (float i = 0.1f; i < 1; i += 0.1f) {
                float xTickMinor = std::round(xPos + (xPosNext - xPos) * i);
                p1 = ImVec2((float)xTickMinor, rect.Min.y + (i == 0.5f ? mid : minor));
                p2 = ImVec2((float)xTickMinor, rect.Max.y + low);        
                DrawList->AddLine(p1, p2,color);
            }
        }
    }
    if (clip)
        DrawList->PopClipRect();
}

bool SequencerRow(bool dim)
{
    return false;
}

bool Sequencer(const char* label, std::vector<Track>& tracks, int& selected, float tMin, float tMax, ImVec4 color)
{
    ImGuiWindow *window = GetCurrentWindow();
    if (window->SkipItems)
        return false;
    ImGuiContext &g = *GImGui;
    const ImGuiStyle &style = g.Style;
    const ImGuiIO &IO = GetIO();
    ImDrawList *DrawList = GetWindowDrawList();
    const ImGuiID id = window->GetID(label);

    const ImVec2 canvasPos = ImGui::GetCursorScreenPos();
    const ImVec2 canvasSize = ImGui::GetContentRegionAvail();

    const int legendWidth = 130;
    const float trackHeight = GetFrameHeight();
    const int trackPadding = 1;
    int dummyRows = 15;

    static float lt = 0.0f;
    static float rt = 1.0f;

    tMin = Remap(lt, 0, 1, tMin, tMax);
    tMax = Remap(rt, 0, 1, tMin, tMax);

    // HEADER
    DrawList->AddRectFilled(canvasPos, ImVec2(canvasSize.x + canvasPos.x, canvasPos.y + trackHeight), ImGui::GetColorU32(ImGuiCol_Button), 0);
    // 
    if (ImGui::Button(ICON_FA_PLUS_SQUARE)) {
        tracks.emplace_back();
        tracks.back().label = "Track " + std::to_string(tracks.size());
        selected = tracks.size() - 1;
    }
    if (NodeDroppedL()) {
        float insertT = 0;
        for (auto& track : tracks) {
            if (track.populated)
                insertT = std::max(insertT, track.t + (float)track.signal.length());
        }
        if (tracks.empty() || tracks.back().populated)
            tracks.emplace_back();
        tracks.back().label = NodePayloadL().first;
        tracks.back().signal = NodePayloadL().second;
        tracks.back().populated = true;
        tracks.back().t = insertT;
        selected = tracks.size() - 1;
    }
    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_MINUS_SQUARE)) {
        if (selected >= 0 && selected < tracks.size())
            tracks.erase(tracks.begin() + selected);
        if (selected >= tracks.size())
            selected--;
        if (selected < 0)
            selected = 0;
    }

    const ImRect timeline_bb(canvasPos + ImVec2(legendWidth, 0), ImVec2(canvasSize.x + canvasPos.x, canvasPos.y + trackHeight));
    RenderTicks(timeline_bb, tMin, tMax, 4, 8, 14, 1, GetColorU32(ImGuiCol_Text), true, true);

    // TRACK BACKGROUND
    ImGui::PushStyleColor(ImGuiCol_ChildBg, {0, 0, 0, 0});
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {7, 1});
    float h = ImGui::GetContentRegionAvail().y - ImGui::GetStyle().ScrollbarSize - ImGui::GetStyle().ItemSpacing.y;

    ImGui::BeginChild(69, ImVec2(-1, h));
    auto child = GetCurrentWindow();
    DrawList->PushClipRect(child->ClipRect.Min, child->ClipRect.Max);

    for (int i = 0; i < tracks.size(); ++i)
    {
        ImGui::PushID(i);

        bool dim = i % 2 == 0;        
        if (ImGui::Selectable(tracks[i].label.c_str(), selected == i, 0, ImVec2(98,0))) {
            selected = i;
        }
        ImGui::SameLine();
        bool visible = tracks[i].visible;
        if (!visible)
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.25f);
        if (ImGui::SmallButton(ICON_FA_EYE))
            tracks[i].visible = !tracks[i].visible;
        if (!visible)
            ImGui::PopStyleVar();
        ImGui::SameLine(legendWidth);
        ImGui::InvisibleButton("##TrackBar", ImVec2(canvasSize.x - legendWidth, 17));
        ImRect track_bb(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
        DrawList->PushClipRect(track_bb.Min - ImVec2(0,1), track_bb.Max + ImVec2(0,1), true);
        DrawList->AddRectFilled(track_bb.Min, track_bb.Max, ImGui::GetColorU32(ImGuiCol_Tab, dim ? 0.75f : 1.0f), 0);
        RenderTicks(track_bb, tMin, tMax, -1,-1,-1,1,GetColorU32(ImGuiCol_Text, 0.1f), false, false);
        // dnd
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("DND_LITEM", ImGuiDragDropFlags_AcceptBeforeDelivery | ImGuiDragDropFlags_AcceptNoDrawDefaultRect))
            {
                double lengthT = NodePayloadL().second.length();
                float x0 = IO.MousePos.x;
                float t0 = Remap(x0, track_bb.Min.x, track_bb.Max.x, tMin, tMax);
                float t1 = lengthT == tact::INF ? tMax : t0 + (float)lengthT;
                float x1 = Remap(t1, tMin, tMax, track_bb.Min.x, track_bb.Max.x);
                DrawList->AddRectFilled({x0, track_bb.Min.y + 2}, {x1, track_bb.Max.y - 2}, GetColorU32(carnot::withAlpha(carnot::Oranges::DarkOrange, 0.75f)), 2);
                if (payload->IsDelivery()) {
                    tracks[i].signal = NodePayloadL().second;
                    tracks[i].label = NodePayloadL().first;
                    tracks[i].populated = true;
                    tracks[i].t = t0;
                }

            }
            ImGui::EndDragDropTarget();
        }
        // render signals
        if (tracks[i].populated) {
            if (tracks[i].held) {
                if (IO.MouseReleased[0] || ! IO.MouseDown[0])
                    tracks[i].held = false;
                float dt = IO.MouseDelta.x * (tMax-tMin) / track_bb.GetWidth();
                tracks[i].t += dt;
            }
            double lengthT = tracks[i].signal.length();
            float t0 = tracks[i].t;
            float t1 = lengthT == tact::INF ? tMax : t0 + (float)lengthT;
            float x0 = Remap(t0, tMin, tMax, track_bb.Min.x, track_bb.Max.x);
            float x1 = Remap(t1, tMin, tMax, track_bb.Min.x, track_bb.Max.x);
            ImRect signal_bb({x0, track_bb.Min.y + 2}, {x1, track_bb.Max.y - 2});
            bool hovered = signal_bb.Contains(IO.MousePos);
            if (hovered) {
                if (IO.MouseClicked[0])
                    tracks[i].held = true;
            }

            auto thisColor = color;
            if (hovered && !tracks[i].held)
                thisColor.w = 1;
            else
                thisColor.w = 0.75f;

            DrawList->AddRectFilled({x0, track_bb.Min.y + 2}, {x1, track_bb.Max.y - 2}, GetColorU32(thisColor), 2);
        }
        DrawList->PopClipRect();
        ImGui::PopID();
    }
    DrawList->PopClipRect();
    ImGui::EndChild();
    ImGui::PopStyleVar();
    ImGui::PopStyleColor();

    static bool lg = false, rg = false, cg = false;
    Indent(legendWidth);
    TimelineScrollbar(&lt, &rt, &lg, &rg, &cg);
    Unindent();
    return true;
}

} // namespace ImGui