#define IMGUI_DEFINE_MATH_OPERATORS

#include "DragAndDrop.hpp"
#include <imgui_internal.h>

namespace
{
bool g_signalHeld = false;
bool g_paletteHeld = false;
PItem g_palettePayload;
std::pair<std::string, tact::Signal> g_signalPayload;
bool g_pulsableSet = false;
float g_pulseTime;

static inline ImVec4 Lerp(const ImVec4& a, const ImVec4& b, float t)          
{ 
    return ImVec4(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t, a.w + (b.w - a.w) * t); 
}

} // namespace

void UpdateDragAndDrop()
{
    g_pulseTime = ImGui::GetIO().DeltaTime;
    if (g_signalHeld && (ImGui::GetIO().MouseReleased[0] || !ImGui::GetIO().MouseDown[0]))
        g_signalHeld = false;
    if (g_paletteHeld && (ImGui::GetIO().MouseReleased[0] || !ImGui::GetIO().MouseDown[0]))
        g_paletteHeld = false;
}

void BeginPulsable(bool acceptPalette, bool acceptSignal, ImGuiCol col)
{
    ImVec4 color = ImGui::GetStyle().Colors[col];
    if ((g_signalHeld && acceptSignal) || (g_paletteHeld && acceptPalette))
    {
        static float f = 1.0f;
        float t = 0.5f + 0.5f * std::sin(2.0f * IM_PI * f * (float)ImGui::GetTime());
        color = Lerp(color, ImVec4(0, 0, 0, 0), t * 0.25f);     
        ImGui::PushStyleColor(col, color);
        g_pulsableSet = true;
    }
}

void EndPulsable()
{
    if (g_pulsableSet) {
        ImGui::PopStyleColor();
        g_pulsableSet = false;
    }
}

void SetPaletteHeld(bool held)
{
    g_paletteHeld = held;
    if (held) {
        g_pulseTime = 0;
    }
}

void PaletteSource(PItem pItem)
{
    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
    {
        g_palettePayload = pItem;
        SetPaletteHeld(true);
        ImGui::SetDragDropPayload("DND_PITEM", &g_palettePayload, sizeof(PItem));
        ImGui::Text(paletteName(pItem).c_str());
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

PItem PalettePayload()
{
    return g_palettePayload;
}

bool PaletteHeld() {
    return g_paletteHeld;
}

void SetSignalHeld(bool held)
{
    g_signalHeld = held;
    if (held) {
        g_pulseTime = 0;
    }
}

void SignalSource(const std::string &name, tact::Signal signal)
{
    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
    {
        g_signalPayload.first = name;
        g_signalPayload.second = std::move(signal);
        SetSignalHeld(true);
        ImGui::SetDragDropPayload("DND_LITEM", &g_signalPayload, sizeof(std::string));
        ImGui::Text(name.c_str());
        ImGui::EndDragDropSource();
    }
}

bool SignalTarget()
{
    bool ret = false;
    if (ImGui::BeginDragDropTarget())
    {
        const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("DND_LITEM");
        if (payload != NULL && g_signalHeld)
        {
            g_signalHeld = false;
            ret = true;
        }
        ImGui::EndDragDropTarget();
    }
    return ret;
}

const std::pair<std::string, tact::Signal>& SignalPayload()
{
    return g_signalPayload;
}

bool SignalHeld()
{
    return g_signalHeld;
}

void HelpSource() {
    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
    {
        static int helpPayload = 1;
        ImGui::SetDragDropPayload("DND_HELP", &helpPayload, sizeof(int));
        ImGui::Text(ICON_FA_QUESTION);
        ImGui::EndDragDropSource();
    }
}

bool HelpTarget()
{
    bool ret = false;
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("DND_HELP"))
        {
            ret = true;
        }
        ImGui::EndDragDropTarget();
    }
    return ret;
}