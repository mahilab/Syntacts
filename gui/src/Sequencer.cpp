#define IMGUI_DEFINE_MATH_OPERATORS

#include "Sequencer.hpp"
#include "Gui.hpp"
#include "Custom.hpp"
#include "Nodes.hpp"
#include <algorithm>
#include <imgui_internal.h>

inline float Remap(float x, float x0, float x1, float y0, float y1) { return y0 + (x - x0) * (y1 - y0) / (x1 - x0); }

Sequencer::Sequencer(Gui& gui)
    : Widget(gui)
{
    m_interface.tracks.emplace_back();
    m_interface.tracks.back().label = "Track 1";
    m_interface.activeColor =  gui.theme.sequencerColor;
    m_interface.inactiveColor = mahi::gui::Grays::Gray50;
    m_interface.onTooltip = [&](const char* msg) { gui.status.showTooltip(msg); };
}

void Sequencer::update()
{
    ImGui::BeginGroup();
    ImGui::Sequencer("Sequencer", m_interface);  
    ImGui::EndGroup();  
    if (HelpTarget()) 
        ImGui::OpenPopup("Sequencer Help");
    
    if (ImGui::BeginHelpPopup("Sequencer Help")) {
        ImGui::BulletText("Drag Signals from the Library on to Track timelines");
        ImGui::BulletText("Position Track Signals by dragging horizontally");
        ImGui::BulletText("Add new Tracks with the " ICON_FA_PLUS_SQUARE " button");
        ImGui::BulletText("You can also drag Signals on to the " ICON_FA_PLUS_SQUARE " button");
        ImGui::BulletText("Remove the selected Track with the " ICON_FA_MINUS_SQUARE " button");
        ImGui::BulletText("Use the " ICON_FA_EXPAND " button to fit the timeline to all Tracks");
        ImGui::BulletText("Toggle timeline snap with the [ ] button");
        ImGui::BulletText("Disable individual Tracks with the " ICON_FA_EYE " buttons");
        ImGui::BulletText("Use the " ICON_FA_ANGLE_DOWN " button to reveal additional track controls");
        ImGui::BulletText("Ctrl+Scroll to zoom in and out on the timeline");
        ImGui::EndPopup();
    }
    
}

tact::Signal Sequencer::buildSignal() {
    m_seq.clear();
    for (auto& track : m_interface.tracks) {
        if (track.populated && track.visible)
            m_seq.insert(track.signal, track.t);
    }
    return m_seq;
}

///////////////////////////////////////////////////////////////////////////////

namespace ImGui
{

//=============================================================================
// TICKS
//=============================================================================

inline void GetTicks(float tMin, float tMax, int nMajor, int nMinor, std::vector<double> &out)
{
    out.clear();
    const double range = NiceNum(tMax - tMin, 0);
    const double interval = NiceNum(range / (nMajor - 1), 1);
    const double graphmin = floor(tMin / interval) * interval;
    const double graphmax = ceil(tMax / interval) * interval;
    const int nfrac = std::max((int)-floor(log10(interval)), 0);

    for (double major = graphmin; major < graphmax + 0.5 * interval; major += interval)
    {
        out.push_back(major);
        for (int i = 1; i < nMinor; ++i)
            out.push_back(major + i * interval / nMinor);
    }
    out.resize(out.size() - nMinor + 1);
}

inline void RenderTicks(ImDrawList* DrawList, const std::vector<double> &ticks, const ImRect rect, float tMin, float tMax, int nMajor, int nMinor, float oMajor, float oMid, float oMinor, float oLow, ImU32 color, bool clip, bool text, double tScale)
{
    const float oHigh = std::min(0.0f, std::min(oMajor, std::min(oMid, oMinor)));
    if (clip)
        DrawList->PushClipRect(rect.Min + ImVec2(0, oHigh), rect.Max + ImVec2(0, oLow), true);
    for (int i = 0; i < ticks.size(); ++i)
    {
        float x = std::round(Remap((float)ticks[i], tMin, tMax, rect.Min.x, rect.Max.x));
        if (x < rect.Max.x)
        {
            float o = i % nMinor == 0 ? oMajor : i % (nMinor / 2) == 0 ? oMid : oMinor;
            ImVec2 p1(x, rect.Max.y + oLow);
            ImVec2 p2(x, rect.Min.y + o);
            DrawList->AddLine(p1, p2, color);

            if (text && i % nMinor == 0)
            {
                std::stringstream stream;
                stream << ticks[i] * tScale;
                DrawList->AddText(ImVec2(x + 3, rect.Min.y), color, stream.str().c_str());
            }
        }
    }
    if (clip)
        DrawList->PopClipRect();
}

//=============================================================================
// SNAP
//=============================================================================

struct SnapInfo {
    enum Type {
        LL   = 0, // left of left side
        RL   = 1, // right of left side
        LR   = 2, // left or right side
        RR   = 3, // right of right side,
        None = 4, // No snap
    };
    Type type = None;
    double snapL = 0;
    double snapR = 0;
};

inline SnapInfo FindSnapPoint(const std::vector<double> &ticks, double tL, double tR, double tMin, double tMax, bool ignoreL = false, bool ignoreR = false)
{
    if (tL < tMin)
        ignoreL = true;
    if (tR > tMax)
        ignoreR = true;
    std::array<double, 4> snapPoints    = {tact::INF, tact::INF, tact::INF, tact::INF};
    std::array<double, 4> snapDeltas    = {tact::INF, tact::INF, tact::INF, tact::INF};
    std::array<double, 4> snapDistances = {tact::INF, tact::INF, tact::INF, tact::INF};
    if (!ignoreL)
    {
        auto toR = std::lower_bound(ticks.begin(), ticks.end(), tL);
        if (toR != ticks.end())
        {
            snapPoints[1] = *toR;
            snapDeltas[1] = snapPoints[1] - tL;
            if (toR != ticks.begin())
            {
                snapPoints[0] = *(--toR);
                snapDeltas[0] = snapPoints[0] - tL;
            }
        }
    }
    if (!ignoreR)
    {
        auto toR = std::lower_bound(ticks.begin(), ticks.end(), tR);
        if (toR != ticks.end())
        {
            snapPoints[3] = *toR;
            snapDeltas[3] = snapPoints[3] - tR;
            if (toR != ticks.begin())
                snapPoints[2] = *(--toR);
            snapDeltas[2] = snapPoints[2] - tR;
        }
    }
    for (int i = 0; i < 4; ++i)
        snapDistances[i] = std::abs(snapDeltas[i]);
    auto smallest = std::min_element(snapDistances.begin(), snapDistances.end());
    int idx = smallest - snapDistances.begin();
    if (*smallest == tact::INF)
        return {SnapInfo::None, tL + 0, tR + 0};
    return {(SnapInfo::Type)idx, tL + snapDeltas[idx], tR + snapDeltas[idx]};
}

inline void RenderSnapHint(ImDrawList* DrawList, const ImRect& track_bb, float tMin, float tMax, const SnapInfo& snapInfo) {
    if (snapInfo.type == SnapInfo::None)
        return;
    float snapX, snapXOffset;
    if (snapInfo.type == SnapInfo::LL || snapInfo.type == SnapInfo::RL) {
        snapX = std::round(Remap(snapInfo.snapL, tMin, tMax, track_bb.Min.x, track_bb.Max.x));
        snapXOffset = 3;
    }
    else if (snapInfo.type == SnapInfo::LR || snapInfo.type == SnapInfo::RR) {
        snapX = std::round(Remap(snapInfo.snapR, tMin, tMax, track_bb.Min.x, track_bb.Max.x));
        snapXOffset = -3;
    }
    ImVec2 P[4];
    P[0] = ImVec2(snapX + snapXOffset, track_bb.Min.y);
    P[1] = ImVec2(snapX, track_bb.Min.y);
    P[2] = ImVec2(snapX, track_bb.Max.y);
    P[3] = ImVec2(snapX + snapXOffset, track_bb.Max.y);
    DrawList->AddPolyline(P, 4, GetColorU32(ImGuiCol_Text), false, 2);
}

//=============================================================================
// TRACK ITEMS
//=============================================================================

inline void TrackSignal(ImDrawList * DrawList, const tact::Signal& signal, const ImRect& track_bb, float t, float tMin, float tMax, ImVec4 color, bool plot, bool& held) {
    const ImGuiIO &IO = GetIO();
    float lengthT = (float)signal.length();
    float t0 = t;
    float t1 = lengthT == tact::INF ? tMax : t0 + lengthT;
    float x0 = std::round(Remap(t0, tMin, tMax, track_bb.Min.x, track_bb.Max.x));
    float x1 = std::round(Remap(t1, tMin, tMax, track_bb.Min.x, track_bb.Max.x));
    ImRect trackItem_bb({x0, track_bb.Min.y + 2}, {x1, track_bb.Max.y - 2});
    bool hovered = trackItem_bb.Contains(IO.MousePos);
    if (hovered && IO.MouseClicked[0])
        held = true;
    auto bgColor = color;
    bgColor.w = hovered && !held ? 0.9f : 0.6f;
    DrawList->AddRectFilled(trackItem_bb.Min, trackItem_bb.Max, GetColorU32(bgColor), 3);
    if (plot)
    {
        float ts1 = 0;
        float ts2 = signal.length() == tact::INF ? tMax - t : (float)signal.length();
        ImVec4 plotColor = ImLerp({1, 1, 1, 1}, color, 0.9f);
        RenderSignalInBounds(DrawList, signal, ts1, ts2, trackItem_bb, plotColor, 1, 2000);
    }
}

//=============================================================================
// SEQUENCER
//=============================================================================

bool Sequencer(const char *label, SeqInterface &I, bool fullFrame)
{
    //=========================================================================
    // IMGUI FRONT MATTER
    ImGuiWindow *window = GetCurrentWindow();
    if (window->SkipItems)
        return false;
    ImGuiContext &g = *GImGui;
    const ImGuiStyle &style = g.Style;
    const ImGuiIO &IO = GetIO();
    ImDrawList *DrawList = GetWindowDrawList();
    const ImGuiID id = window->GetID(label);
    // SIZES
    const float controlsWidth = 140;
    // CANVAS AREA
    const ImVec2 canvasPos = ImGui::GetCursorScreenPos();
    const ImVec2 canvasSize = ImGui::GetContentRegionAvail();
    const ImRect canvas_bb(canvasPos, canvasPos + canvasSize);
    // FLAGS
    bool fitTimeline = I.fitThisFrame;
    I.fitThisFrame = false;

    //=========================================================================
    // HEADER

    // BACKGROUND
    const float headerHeight  = GetFrameHeight();
    DrawList->AddRectFilled(canvas_bb.Min, ImVec2(canvas_bb.Max.x, canvas_bb.Min.y + headerHeight), ImGui::GetColorU32(ImGuiCol_Button), 0);

    // HEADER CONTROLS
    ImGui::PushStyleColor(ImGuiCol_Button, 0);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {0, 2 * style.FramePadding.y});
    int nButtons = 5;
    ImVec2 buttonSize(controlsWidth / nButtons, 0);
    BeginPulsable(false, true, ImGuiCol_Text);
    if (ImGui::Button(ICON_FA_PLUS_SQUARE, buttonSize))
    {
        I.tracks.emplace_back();
        I.tracks.back().label = "Track " + std::to_string(I.tracks.size());
        I.selected = (int)I.tracks.size() - 1;
    }
    EndPulsable();
    if (ImGui::IsItemHovered())
        I.onTooltip("Add New Track");
    if (SignalTarget())
    {
        double insertT = 0;
        for (auto &track : I.tracks)
        {
            if (track.populated)
                insertT = std::max(insertT, track.t + track.signal.length());
        }
        if (insertT == tact::INF)
            insertT = 0;
        if (I.tracks.empty() || I.tracks.back().populated)
            I.tracks.emplace_back();
        I.tracks.back().label = SignalPayload().first;
        I.tracks.back().signal = SignalPayload().second;
        I.tracks.back().populated = true;
        I.tracks.back().t = insertT;
        I.selected = (int)I.tracks.size() - 1;
        fitTimeline = true;
    }
    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_MINUS_SQUARE, buttonSize))
    {
        if (I.selected >= 0 && I.selected < I.tracks.size())
            I.tracks.erase(I.tracks.begin() + I.selected);
        if (I.selected >= I.tracks.size())
            I.selected--;
        if (I.selected < 0)
            I.selected = 0;
    }
    if (ImGui::IsItemHovered())
        I.onTooltip("Delete Selected Track");
    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_EXPAND, buttonSize))
        fitTimeline = true;
    if (ImGui::IsItemHovered())
        I.onTooltip("Fit Timeline");
    ImGui::SameLine();
    bool fade = !I.snap;
    if (fade)
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.25f);
    if (ImGui::Button("[ ]", buttonSize))
        I.snap = !I.snap;
    if (fade)
        ImGui::PopStyleVar();
    if (ImGui::IsItemHovered())
        I.onTooltip("Toggle Timeline Snap");
    ImGui::SameLine();
    if (ImGui::Button(I.milliseconds ? "ms" : "s", buttonSize))
    {
        I.milliseconds = !I.milliseconds;
    }
    if (ImGui::IsItemHovered())
        I.onTooltip("Toggle Timeline Units");
    ImGui::PopStyleColor();
    ImGui::PopStyleVar();

    // HEADER TIMELINE
    const int nMajorTicks = 4;
    const int nMinorTicks = 10;
    static std::vector<double> tTicks(100);
    double tScale = I.milliseconds ? 1000 : 1.0;
    GetTicks(I.tMin, I.tMax, nMajorTicks, nMinorTicks, tTicks);

    const ImRect timeline_bb(canvasPos + ImVec2(controlsWidth, 0), ImVec2(canvasSize.x + canvasPos.x, canvasPos.y + headerHeight));
    RenderTicks(DrawList, tTicks, timeline_bb, I.tMin, I.tMax, nMajorTicks, nMinorTicks, 4, 8, 14, 1, GetColorU32(ImGuiCol_Text), true, true, tScale);

    //=========================================================================
    // TRACKS
    ImRect allTracks_bb = timeline_bb;
    float maxTrackT = 0;

    // TRACKS BACKGROUND
    ImGui::PushStyleColor(ImGuiCol_ChildBg, {0, 0, 0, 0});
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {0, 1});
    float h = ImGui::GetContentRegionAvail().y - ImGui::GetStyle().ScrollbarSize - ImGui::GetStyle().ItemSpacing.y;
    if (fullFrame)
        ImGui::BeginChild(69, ImVec2(-1, h));
    auto child = GetCurrentWindow();
    DrawList->PushClipRect(child->ClipRect.Min - ImVec2(0,1), child->ClipRect.Max + ImVec2(1, 0));

    // TRACKS FOR LOOP
    for (int i = 0; i < I.tracks.size(); ++i)
    {
        SeqInterface::Track& track = I.tracks[i];
        ImGui::PushID(i);

        //=====================================================================
        // BASIC TRACK CONTROLS
        ImGui::BeginGroup();
        if (ImGui::Selectable(track.label.c_str(), I.selected == i, 0, ImVec2(100, 0)))
            I.selected = i;
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0);
        ImGui::SameLine(102);
        bool visible = track.visible;
        if (!visible)
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.25f);
        if (ImGui::Button("##Visible", {18, GetTextLineHeightWithSpacing()}))
            track.visible = !track.visible;
        RenderText(GetItemRectMin() + GetItemRectSize() * 0.5f - ImVec2(8, 8), ICON_FA_EYE);
        if (!visible)
            ImGui::PopStyleVar();
        ImGui::SameLine(122);
        if (ImGui::Button("##Expand", {16, GetTextLineHeightWithSpacing()}))        
            track.expanded = !track.expanded;        
        RenderText(GetItemRectMin() + GetItemRectSize() * 0.5f - ImVec2(6, 8), track.expanded ? ICON_FA_ANGLE_UP : ICON_FA_ANGLE_DOWN);
        ImGui::PopStyleVar();

        //=====================================================================
        // EXPANDED TRACK CONTROLS
        if (track.expanded)
        {
            ImGui::Indent(10);
            PushStyleVar(ImGuiStyleVar_FramePadding, {0, 0});
            PushStyleColor(ImGuiCol_FrameBg, 0);
            PushItemWidth(90);
            float tempT = (float)(track.t * tScale);
            ImGui::DragFloat("Time", &tempT, (I.tMax - I.tMin) * 0.005f * (float)tScale);
            track.t = tempT / tScale;
            float gain = (float)track.signal.gain;
            if (ImGui::DragFloat("Gain", &gain, 0.01f))
                track.signal.gain = gain;
            float bias = (float)track.signal.bias;
            if (ImGui::DragFloat("Bias", &bias, 0.01f))
                track.signal.bias = bias;
            PopItemWidth();
            PopStyleColor();
            PopStyleVar();
            ImGui::Unindent();
        }
        ImGui::EndGroup(); // track controls

        //=====================================================================
        // TRACK TIMELINE
        bool dim = i % 2 == 0;
        float trackHeight = ImGui::GetItemRectSize().y; // height of the control group
        ImGui::SameLine(controlsWidth + fullFrame ? 0 : 20); // IDK WHY 20 WORKS!
        ImGui::InvisibleButton("##TrackBar", ImVec2(canvasSize.x - controlsWidth, trackHeight));
        ImRect track_bb(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
        allTracks_bb.Max = track_bb.Max;
        DrawList->PushClipRect(track_bb.Min - ImVec2(1,1), track_bb.Max + ImVec2(1,1), true);
        DrawList->AddRectFilled(track_bb.Min, track_bb.Max, ImGui::GetColorU32(ImGuiCol_Tab, dim ? 0.75f : 1.0f), 0);
        RenderTicks(DrawList, tTicks, track_bb, I.tMin, I.tMax, nMajorTicks, nMinorTicks, -1, -1, -1, 1, GetColorU32(ImGuiCol_Text, 0.1f), true, false, tScale);

        //=====================================================================
        // DRAG AND DROP
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("DND_LITEM", ImGuiDragDropFlags_AcceptBeforeDelivery | ImGuiDragDropFlags_AcceptNoDrawDefaultRect))
            {
                float x0 = IO.MousePos.x;
                float t0 = Remap(x0, track_bb.Min.x, track_bb.Max.x, I.tMin, I.tMax);
                bool held = true;
                TrackSignal(DrawList, SignalPayload().second, track_bb, t0, I.tMin, I.tMax, I.activeColor, track.expanded, held);
                SnapInfo snapInfo;
                if (I.snap) {
                    snapInfo = FindSnapPoint(tTicks, t0, t0 + SignalPayload().second.length(), I.tMin, I.tMax);
                    RenderSnapHint(DrawList, track_bb, I.tMin, I.tMax, snapInfo);
                }
                if (payload->IsDelivery())
                {
                    track.signal = SignalPayload().second;
                    track.label = SignalPayload().first;
                    track.populated = true;
                    track.t = t0;
                    if (I.snap)
                        track.t = snapInfo.snapL;
                }
            }
            ImGui::EndDragDropTarget();
        }

        //=====================================================================
        // TRACK ITEMS
        if (track.populated)
        {
            SnapInfo snapInfo;
            if (track.held)
            {
                if (I.snap) 
                    snapInfo = FindSnapPoint(tTicks, track.t, track.t + track.signal.length(), I.tMin, I.tMax);
                if (IO.MouseReleased[0] || !IO.MouseDown[0])
                {
                    track.held = false;
                    if (I.snap)
                        track.t = snapInfo.snapL;
                }
                float dt = IO.MouseDelta.x * (I.tMax - I.tMin) / track_bb.GetWidth();
                track.t += dt;
            }
            TrackSignal(DrawList, track.signal, track_bb, track.t, I.tMin, I.tMax, track.visible ? I.activeColor : I.inactiveColor, track.expanded, track.held );

            // Render Snap Hint
            if (I.snap && track.held) 
                RenderSnapHint(DrawList, track_bb, I.tMin, I.tMax, snapInfo);

            maxTrackT = track.t + track.signal.length();
        }
        DrawList->PopClipRect(); // track bb
        ImGui::PopID();
    }
    DrawList->PopClipRect(); // child bb
    if (fullFrame)
        ImGui::EndChild();
    ImGui::PopStyleVar();
    ImGui::PopStyleColor();

    //=========================================================================
    // SCROLL / FIT CONTROLS
    const float zoomRate = 0.1f;
    if (allTracks_bb.Contains(IO.MousePos) && IO.KeyCtrl)
    {
        float tRange = (I.tMax - I.tMin);
        if (IO.MouseWheel < 0)
        {
            // tMin -= zoomRate * tRange;
            I.tMax += zoomRate * tRange;
        }
        else if (IO.MouseWheel > 0)
        {
            // tMin += zoomRate * tRange / (1.0f + 2.0f * zoomRate);
            I.tMax -= zoomRate * tRange / (1.0f + 2.0f * zoomRate);
            I.tEnd -= zoomRate * tRange / (1.0f + 2.0f * zoomRate);
        }
    }
    if (I.tMax > I.tEnd)
        I.tEnd = I.tMax;
    if (I.tEnd < maxTrackT && maxTrackT != std::numeric_limits<float>::infinity())
        I.tEnd = maxTrackT;
    if (fitTimeline)
    {
        I.tMin = 0;
        I.tMax = maxTrackT == 0 || maxTrackT == std::numeric_limits<float>::infinity() ? 1 : maxTrackT;
        I.tEnd = I.tMax;
    }

    //=========================================================================
    // TIMELINE SCROLLBAR
    float lt = Remap(I.tMin, 0, I.tEnd, 0, 1);
    float rt = Remap(I.tMax, 0, I.tEnd, 0, 1);
    if (lt < 0)
        lt = 0;
    if (rt > 1)
        rt = 1;
    Indent(controlsWidth);
    if (TimelineScrollbar(&lt, &rt, &I.lg, &I.rg, &I.cg)) {
        I.tMin = Remap(lt, 0, 1, 0, I.tEnd);
        I.tMax = Remap(rt, 0, 1, 0, I.tEnd);
    }
    Unindent(controlsWidth);

    return true;
}

} // namespace ImGui