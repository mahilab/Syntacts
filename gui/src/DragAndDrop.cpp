#define IMGUI_DEFINE_MATH_OPERATORS

#include "DragAndDrop.hpp"
#include <ImGui/imgui_internal.h>

namespace
{
bool g_nodeHeld = false;
PItem g_pPayload;
std::pair<std::string, tact::Signal> g_lPayload;
float g_nodeTime;

static inline ImVec4 Lerp(const ImVec4& a, const ImVec4& b, float t)          { return ImVec4(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t, a.w + (b.w - a.w) * t); }

} // namespace

void SetNodeHeld(bool held)
{
    g_nodeHeld = held;
    if (held) {
        g_nodeTime = 0;
    }
    else {
    }
}

void DragAndDrop::update()
{
    SetNodeHeld(false);
    g_nodeTime = carnot::Engine::deltaTime();
}

bool NodeHeld()
{
    return g_nodeHeld;
}

void BeginNodeTarget(ImGuiCol col)
{
    ImVec4 color = ImGui::GetStyle().Colors[col];
    if (g_nodeHeld)
    {
        static float f = 1.0f;
        float t = 0.5f + 0.5f * std::sin(2.0f * IM_PI * f * (float)ImGui::GetTime());
        color = Lerp(color, ImVec4(0, 0, 0, 0), t * 0.25f);        
    }
    ImGui::PushStyleColor(ImGuiCol_Button, color);
    ImGui::PushStyleColor(ImGuiCol_FrameBg, color);
}

void EndNodeTarget()
{
    ImGui::PopStyleColor(2);
}


void PaletteSource(PItem pItem)
{
    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
    {
        g_pPayload = pItem;
        SetNodeHeld(true);
        ImGui::SetDragDropPayload("DND_PITEM", &g_pPayload, sizeof(PItem));
        ImGui::Text(palletteString(pItem).c_str());
        ImGui::EndDragDropSource();
    }
}

void SignalSource(const std::string &name, tact::Signal signal)
{
    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
    {
        g_lPayload.first = name;
        g_lPayload.second = std::move(signal);
        SetNodeHeld(true);
        ImGui::SetDragDropPayload("DND_LITEM", &g_lPayload, sizeof(std::string));
        ImGui::Text(name.c_str());
        ImGui::EndDragDropSource();
    }
}

bool PaletteTarget()
{
    bool ret = false;
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("DND_PITEM"))
        {
            ret = true;
        }
        ImGui::EndDragDropTarget();
    }
    return ret;
}

bool SignalTarget()
{
    bool ret = false;
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("DND_LITEM"))
        {
            ret = true;
        }
        ImGui::EndDragDropTarget();
    }
    return ret;
}

PItem PalettePayload()
{
    return g_pPayload;
}

const std::pair<std::string, tact::Signal>& SignalPayload()
{
    return g_lPayload;
}

