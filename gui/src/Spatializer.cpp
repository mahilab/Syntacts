#define IMGUI_DEFINE_MATH_OPERATORS

#include "Spatializer.hpp"
#include "Gui.hpp"
#include "Custom.hpp"
#include "Nodes.hpp"

using namespace mahi::gui;

namespace {
    std::vector<std::pair<std::string, tact::Curve>> g_curveMap = {
        {"Linear", tact::Curves::Linear()},
        {"Smoothstep", tact::Curves::Smoothstep()},
        {"Smootherstep", tact::Curves::Smootherstep()},
        {"Smootheststep", tact::Curves::Smootheststep()},
        {"Logarithmic", tact::Curves::Exponential::In()},
        {"Exponential", tact::Curves::Exponential::Out()}

    };

    // future evan: wtf is this?
    // present evan: turning a drop-off curve into a signal so I can render it with those funcs
    class CurveSignal {
    public:
        CurveSignal(tact::Curve curve) : m_curve(curve) { }
        double sample(double t) const { 
            double s = 1 - std::abs(t - 1);
            return 2*m_curve(s)-1;
        }
        
        double length() const { return 2.0; }
        tact::Curve m_curve;
    };

}

Spatializer::Spatializer(Gui& gui) :
    Widget(gui)
{
    m_target.pos.x = (float)spatializer.getTarget().x;
    m_target.pos.y = (float)spatializer.getTarget().y;
    m_target.radius =(float)spatializer.getRadius();
    m_openId = gui.device.onSessionOpen.connect(this, &Spatializer::onSessionChange);
    m_destroyId = gui.device.onSessionDestroy.connect(this, &Spatializer::onSessionDestroy);
    if (gui.device.session)
        onSessionChange();
    spatializer.autoUpdate(false);
}

Spatializer::~Spatializer() {
    // disconnect so this object's destroyed callbacks don't get called
    gui.device.onSessionOpen.disconnect(m_openId);
    gui.device.onSessionDestroy.disconnect(m_destroyId);
}

void Spatializer::update()
{
    ImGui::BeginGroup(); //  help group
    if (ImGui::Spatializer("Spatializer##Grid", m_target, g_curveMap[m_rollOffIndex].second, m_channels, 10, 
                            gui.theme.spatializerColor, ImVec2(260, -1), "DND_CHANNEL", m_divs[0], !m_2d ? 1 : m_divs[1], m_snap, m_wrap)) {
        sync();
    }
    ImGui::SameLine();
    ImGui::BeginGroup();
    float control_width = 225;
    ImGui::PushItemWidth(control_width);
    ImGui::Text(m_2d ? "X Divisions" : "Divisions");
    if (m_2d) {
        ImGui::SameLine(112);
        ImGui::Text("Y Divisions");
    }
    if (!m_2d)
        ImGui::SliderInt("##Grid Size", m_divs, 1, 25);
    else
            ImGui::SliderInt2("##Grid Size", m_divs, 1, 25);    
    
    ImGui::SameLine();
    ImGui::ToggleButton(ICON_FA_MAGNET, &m_snap);
    gui.status.showTooltip("Toggle Snap");

    ImGui::SameLine();
    ImGui::ToggleButton(ICON_FA_TH, &m_2d);
    gui.status.showTooltip("Toggle 2D");

    ImGui::SameLine();
    if (ImGui::ToggleButton(ICON_FA_CIRCLE_NOTCH, &m_wrap)) {
        if (m_wrap)
            spatializer.setWrap(1,1);
        else
            spatializer.setWrap(0,0);
    }
    gui.status.showTooltip("Toggle Wrapping");


    if (ImGui::Button("Fill", ImVec2(control_width, 0))) {
        spatializer.clear();
        fillGrid();
    }
    gui.status.showTooltip("Fill All Channels");

    if (ImGui::Button("Clear", ImVec2(control_width, 0))) {
        spatializer.clear();
        m_channels.clear();
    }
    gui.status.showTooltip("Clear All Channels");


    // ImGui::PushItemWidth(175);
    // bool entered = ImGui::InputText("##SignalName", m_inputBuffer, 64, ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue);
    // ImGui::PopItemWidth();
    // ImGui::SameLine();
    // ImGui::Button(ICON_FA_SAVE);

    ImGui::Separator();
    if (m_2d)
        ImGui::DragFloat2("Position", &m_target.pos.x, 0.005f, 0.0f, 1.0f);
    else
        ImGui::DragFloat("Position", &m_target.pos.x, 0.005f, 0.0f, 1.0f);
    ImGui::DragFloat("Radius", &m_target.radius, 0.005f, 0.0f, 1.0f);
    static std::vector<ImVec2> points(100);
    ImGui::PlotSignal("##Empty", CurveSignal(g_curveMap[m_rollOffHoveredIdx == -1 ? m_rollOffIndex : m_rollOffHoveredIdx].second), points, 0, 2, Greens::Chartreuse, 1, ImVec2(control_width,45), false, false);     
    m_rollOffHoveredIdx = -1;
    ImGui::SameLine();
    ImGui::Text("Roll-Off");
    if (ImGui::BeginCombo("##Roll-Off", g_curveMap[m_rollOffIndex].first.c_str()))
    {
        for (int i = 0; i < g_curveMap.size(); ++i) {
            bool selected = i == m_rollOffIndex;
            if (ImGui::Selectable(g_curveMap[i].first.c_str(), selected)) {
                m_rollOffIndex = i;
                spatializer.setRollOff(g_curveMap[m_rollOffIndex].second);
            }
            if (selected)
                ImGui::SetItemDefaultFocus();
            if (ImGui::IsItemHovered())            
                m_rollOffHoveredIdx = i;
        }
        ImGui::EndCombo();
    }

    ImGui::Separator();
    NodeSlot(m_sigName.c_str(), ImVec2(control_width, 0));
    if (SignalTarget()) {
        m_sigName = SignalPayload().first;
        m_signal = SignalPayload().second;

    }
    ImGui::SameLine();
    ImGui::Text("Signal");
    if (ImGui::Button(ICON_FA_PLAY, ImVec2(25,0)))
        spatializer.play(m_signal);
    if (ImGui::IsItemClicked(1))
        spatializer.stop();
    ImGui::SameLine();


    ImGui::PushItemWidth(96);
    float v = (float)spatializer.getVolume();
    if (ImGui::SliderFloat("##V", &v, 0, 1, "V"))
        spatializer.setVolume(v);
    ImGui::SameLine();
    float p = (float)spatializer.getPitch();
    p = std::log10(p);
    if (ImGui::SliderFloat("##P", &p, -1, 1, "P"))
        spatializer.setPitch(std::pow(10, p));
    ImGui::PopItemWidth();

    ImGui::PopItemWidth();
    ImGui::EndGroup();
    ImGui::EndGroup();
    if (HelpTarget()) 
        ImGui::OpenPopup("Spatializer Help");
    if (ImGui::BeginHelpPopup("Spatializer Help")) {
        ImGui::BulletText("Drag channels on to the Spatializer grid and position them as desired");
        ImGui::BulletText("Remove channels by double-clicking them in the Spatializer");
        ImGui::BulletText("Set X/Y grid divisions and enable snap with the " ICON_FA_MAGNET " button for easy layout");
        ImGui::BulletText("Switch to 1D mode by clicking the " ICON_FA_TH " button");
        ImGui::BulletText("Use the Fill button to uniformly place all channels in the current grid (alternates direction)");
        ImGui::BulletText("Use the Clear button to remove all channels from the Spatializer");
        ImGui::Separator();
        ImGui::BulletText("Move the Target location by right-mouse dragging the grid");
        ImGui::BulletText("Change the Target radius with the mouse scroll wheel");
        ImGui::BulletText("Select a volume roll-off method from the drop down list");
        ImGui::Separator();
        ImGui::BulletText("Drag Signals from the Library into the Signal slot");
        ImGui::BulletText("Left-click and right-click the " ICON_FA_PLAY " button to play and stop the Signal, respectively");
        ImGui::BulletText("Use the V and P sliders to adjust master volume and pitch, respectively");
        ImGui::EndPopup();
    }

    

    for (auto& chan : m_channels)  {
        spatializer.setPosition(chan.first, chan.second.pos.x, chan.second.pos.y);
        if (gui.device.session)
            chan.second.level = (float)gui.device.session->getLevel(chan.first);
    }
    spatializer.setTarget(m_target.pos.x, m_target.pos.y);
    spatializer.setRadius(m_target.radius);
    spatializer.update();   

}

tact::Signal Spatializer::getSignal() {
    return m_signal;
}

void Spatializer::onSessionChange() {
    spatializer.bind(gui.device.session.get());
}

void Spatializer::onSessionDestroy() {
    spatializer.unbind();
}

void Spatializer::sync() {
    auto existing = spatializer.getChannels();
    std::vector<int> remove;
    for (auto& e : existing) {
        if (m_channels.count(e) == 0)
            remove.push_back(e);
    }
    for (auto& r : remove)
        spatializer.remove(r);
    update();
}

void Spatializer::fillGrid() {
        m_channels.clear();
        if (!gui.device.session)
            return;
        int chs = gui.device.session->getChannelCount();        
        float div[2] = {1.0f / (m_divs[0]-1), 1.0f / (m_divs[1]-1)};
        int k = 0;
        xFirst = !xFirst;
        if (xFirst || !m_2d)
        {
            for (int y = 0; y < m_divs[1]; ++y)
            {
                for (int x = 0; x < m_divs[0]; ++x)
                {
                    ImGui::SpatializerNode node;
                    node.index = k;
                    node.pos.x = x * div[0];
                    node.pos.y = y * div[1];
                    node.held = false;
                    m_channels[k] = node;
                    if (++k == chs)
                        return;
                }
            }
        }
        else
        {
            for (int x = 0; x < m_divs[0]; ++x)
            {
                for (int y = 0; y < m_divs[1]; ++y)
                {
                    ImGui::SpatializerNode node;
                    node.index = k;
                    node.pos.x = x * div[0];
                    node.pos.y = y * div[1];
                    node.held = false;
                    m_channels[k] = node;
                    if (++k == chs)
                        return;
                }
            }
        }
    }


///////////////////////////////////////////////////////////////////////////////

namespace ImGui {


bool Spatializer(const char *label, SpatializerTarget &target, tact::Curve rolloff, std::map<int, SpatializerNode> &nodes, float nodeRadius,
                 ImVec4 color, ImVec2 size, const char *dnd, int xdivs, int ydivs, bool snap, bool wrap)
{

    ImU32 gridColor = GetColorU32(ImGuiCol_FrameBg);
    ImU32 gridBg    = GetColorU32(ImGuiCol_WindowBg);

    bool changed = false;
    // skip render if SkipItems on
    ImGuiWindow *window = GetCurrentWindow();
    if (window->SkipItems)
        return false;
    // get ImGuiContext and Style
    ImGuiContext &g = *GImGui;
    const ImGuiStyle &style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImGuiIO &IO = GetIO();
    ImDrawList *DrawList = GetWindowDrawList();

    size = CalcItemSize(size, 100, 100);
    // background
    const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + size);
    const ImRect inner_bb(frame_bb.Min + style.FramePadding, frame_bb.Max - style.FramePadding);
    const ImRect total_bb(frame_bb.Min, frame_bb.Max);
    ItemSize(total_bb, style.FramePadding.y);
    if (!ItemAdd(total_bb, 0, &frame_bb))
        return false;
    const bool hovered = ItemHoverable(inner_bb, id);
    RenderFrame(frame_bb.Min, frame_bb.Max, GetColorU32(ImGuiCol_FrameBg), true, style.FrameRounding);

    // render grid
    const float gridSize = ImMin(frame_bb.GetWidth(), frame_bb.GetHeight()) - 3 * nodeRadius;
    const ImVec2 offset = ImVec2(gridSize, gridSize) * 0.5f;
    const ImRect grid = ImRect(frame_bb.GetCenter() - offset, frame_bb.GetCenter() + offset);
    const ImVec2 grid_cntr = grid.GetCenter();
    const float grid_width = grid.GetWidth();
    const float grid_height = grid.GetHeight();
    RenderGrid(grid, xdivs - 1, ydivs - 1, gridColor, gridBg, 2.0f);
    // if (xdivs == 1)
    //     DrawList->AddLine(ImVec2(grid_cntr.x, grid.Min.y), ImVec2(grid_cntr.x, grid.Max.y), gridColor, 1.0f);
    // if (ydivs == 1)
    //     DrawList->AddLine(ImVec2(grid.Min.x, grid_cntr.y), ImVec2(grid.Max.x, grid_cntr.y), gridColor, 1.0f);
    // transform lambdas
    auto toPx = [&](const ImVec2 &nm) -> ImVec2 {
        float pxX = xdivs > 1 ? grid.Min.x + nm.x * grid_width : grid_cntr.x;
        float pxY = ydivs > 1 ? grid.Max.y - nm.y * grid_height : grid_cntr.y;
        return ImVec2(pxX, pxY);
    };
    auto toNm = [&](const ImVec2 px) -> ImVec2 {
        float nmX = xdivs > 1 ? (px.x - grid.Min.x) / grid_width : 0;
        float nmY = ydivs > 1 ? -(px.y - grid.Max.y) / grid_height : 0;
        return ImVec2(nmX, nmY);
    };
    auto snapNode = [&](SpatializerNode &node) {
        if (xdivs > 1)
            node.pos.x = mahi::util::round_nearest(node.pos.x, 1.0f / (xdivs - 1));
        else
            node.pos.x = 0;
        if (ydivs > 1)
            node.pos.y = mahi::util::round_nearest(node.pos.y, 1.0f / (ydivs - 1));
        else
            node.pos.y = 0;
    };

    // node ineractions
    const ImVec2 mousePos = IO.MousePos;
    const float r2 = nodeRadius * nodeRadius;
    int toDelete = -1;
    bool firstGrab = true;
    for (auto rit = nodes.rbegin(); rit != nodes.rend(); ++rit)
    {
        SpatializerNode &node = rit->second;
        ImVec2 posPx = toPx(node.pos);
        ImVec2 v = mousePos - posPx;
        float d = ImLengthSqr(v);
        bool inside = d < r2;
        if (inside && IO.MouseClicked[0] && firstGrab)
        {
            node.held = true;
            firstGrab = false;
        }
        if (inside && IO.MouseDoubleClicked[0] && toDelete == -1)
            toDelete = node.index;
        if (node.held && IO.MouseReleased[0])
            node.held = false;
        // drag
        if (node.held)
        {
            if (IO.MouseDelta.x != 0 || IO.MouseDelta.y != 0) {
                // changed = true;
                posPx += IO.MouseDelta;
                posPx.x = ImClamp(posPx.x, xdivs == 1 ? grid_cntr.x : grid.Min.x, xdivs == 1 ? grid_cntr.x : grid.Max.x);
                posPx.y = ImClamp(posPx.y, ydivs == 1 ? grid_cntr.y : grid.Min.y, ydivs == 1 ? grid_cntr.y : grid.Max.y);
            }
        }
        node.pos = toNm(posPx);
        if (snap && !node.held)
            snapNode(node);
    }
    // render nodes
    for (auto it = nodes.begin(); it != nodes.end(); ++it)
    {
        SpatializerNode &node = it->second;
        float dtarget = std::sqrt(ImLengthSqr(node.pos - target.pos));
        // float ttarget = ImClamp(dtarget / target.radius, 0.0f, 1.0f);
        // ttarget = 1.0f - ttarget;
        // ttarget = (float)rolloff(ttarget);
        float ttarget = node.level;
        ImVec4 heldColor = ImLerp(style.Colors[ImGuiCol_ButtonActive], color, ttarget);
        ImVec4 normColor = ImLerp(style.Colors[ImGuiCol_ButtonHovered], color, ttarget);
        auto posPx = toPx(node.pos);
        DrawList->AddCircleFilled(posPx, nodeRadius, node.held ? ColorConvertFloat4ToU32(heldColor) : ColorConvertFloat4ToU32(normColor), 25);
        std::string id_label = std::to_string(node.index);
        const ImVec2 label_size = CalcTextSize(id_label.c_str(), NULL, true);
        DrawList->AddText(posPx - label_size / 2, GetColorU32(ImGuiCol_Text), id_label.c_str());
    }

    // delete
    if (toDelete != -1)
    {
        nodes.erase(toDelete);
        changed = true;
    }
    // render target(s) ... lol, this is so lazy
    ImVec2 targetPosPx[9] = {toPx(target.pos), 
                             toPx(target.pos + ImVec2(-1, 0)), 
                             toPx(target.pos + ImVec2( 1, 0)),
                             toPx(target.pos + ImVec2(-1,-1)), 
                             toPx(target.pos + ImVec2( 0,-1)), 
                             toPx(target.pos + ImVec2( 1,-1)),              
                             toPx(target.pos + ImVec2(-1, 1)),
                             toPx(target.pos + ImVec2( 0, 1)),
                             toPx(target.pos + ImVec2( 1, 1))};

    if (IO.MouseDown[1] && hovered)    
        targetPosPx[0] = mousePos;    
    if (xdivs == 1)
        targetPosPx[0].x = grid_cntr.x;
    if (ydivs == 1)
        targetPosPx[0].y = grid_cntr.y;

    float r = target.radius * grid.GetWidth();
    if (hovered) {
        r += IO.MouseWheel;
        r = ImClamp(r, 1.0f, FLT_MAX);
    }
    DrawList->PushClipRect(grid.Min, grid.Max, true);
    color.w = 0.2f;

    if (xdivs > 1 && ydivs > 1) {
        int target_count = wrap ? 9 : 1;
        for (int tg = 0; tg < target_count; ++tg) {
            color.w = 0.2f;
            int rings = 25;
            DrawList->AddCircleFilled(targetPosPx[tg], 0.5f * r / rings, ColorConvertFloat4ToU32(color), 20);
            for (int i = 0; i < rings+1; ++i) {
                float t = (i + 1.0f) / rings;
                color.w = rolloff(1 - t) * 0.1f;
                DrawList->AddCircleFilled(targetPosPx[tg], t * r, ColorConvertFloat4ToU32(color), 20);
            }
        }
    }
    else
    {
        int target_count = wrap ? 3 : 1;
        color.w = 1;
        if (xdivs == 1 && ydivs == 1) {
            DrawList->AddRectFilled(grid.Min, grid.Max, ColorConvertFloat4ToU32(color));
        }
        else if (xdivs == 1)
        {
            int indices[3] = {0,4,7};
            for (int i = 0; i < target_count; ++i) {
                ImRect box;
                box.Min = targetPosPx[indices[i]] - ImVec2(r, r);
                box.Max = targetPosPx[indices[i]] + ImVec2(r, r);
                int n = 50;
                box.Min.x = grid.Min.x;
                box.Max.x = grid.Max.x;
                float H = box.GetHeight();
                float h = H / n;
                ImRect b = ImRect(box.Min, ImVec2(box.Max.x, box.Min.y + h));
                float C = box.GetCenter().y;
                for (int i = 0; i < n; ++i) {
                    float c = b.GetCenter().y;
                    float t = 1 - std::abs(C-c) / (0.5 * H);
                    color.w = rolloff(t) * 0.5f;
                    DrawList->AddRectFilled(b.Min, b.Max, ColorConvertFloat4ToU32(color));
                    b.Min.y += h; b.Max.y += h;
                }
            }
        }
        else if (ydivs == 1)
        {
            int indices[3] = {0,1,2};
            for (int i = 0; i < target_count; ++i) {
                ImRect box;
                box.Min = targetPosPx[indices[i]] - ImVec2(r, r);
                box.Max = targetPosPx[indices[i]] + ImVec2(r, r);
                int n = 50;
                box.Min.y = grid.Min.y;
                box.Max.y = grid.Max.y;
                float W = box.GetWidth();
                float w = W / n;
                ImRect b = ImRect(box.Min, ImVec2(box.Min.x + w, box.Max.y));
                float C = box.GetCenter().x;
                for (int i = 0; i < n; ++i) {
                    float c = b.GetCenter().x;
                    float t = 1 - std::abs(C-c) / (0.5 * W);
                    color.w = rolloff(t) * 0.5f;
                    DrawList->AddRectFilled(b.Min, b.Max, ColorConvertFloat4ToU32(color));
                    b.Min.x += w; b.Max.x += w;
                }
            }
        }
        
    }
    DrawList->PopClipRect();

    target.pos = toNm(targetPosPx[0]);
    target.radius = r / grid.GetWidth();
    // render position label
    if (grid.Contains(mousePos))
    {
        char posText[16];
        ImVec2 mouseNm = toNm(mousePos);
        sprintf(posText, "%.2f,%.2f", mouseNm.x, mouseNm.y);
        ImVec2 posTextSize = CalcTextSize(posText);
        RenderText(grid.Max - posTextSize - style.FramePadding, posText);
    }
    // drag and drop
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload(dnd))
        {
            auto dropped = *(int *)payload->Data;
            SpatializerNode node;
            node.index = dropped;
            node.pos = ImClamp(toNm(mousePos), ImVec2(0, 0), ImVec2(1, 1));
            node.held = false;
            node.level = 0;
            if (snap)
                snapNode(node);
            nodes[node.index] = node;
            changed = true;
        }
        ImGui::EndDragDropTarget();
    }
    return changed;
}


}