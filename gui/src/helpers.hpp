#pragma once

#include <carnot>
#include <Engine/ImGui/imgui_internal.h>
#include <string>

using namespace carnot;

namespace helpers {

/// Renders an ImGui tooltip. Call after item to be tooltipped
inline void tooltip(const char* tip) {
    if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 1.0f)  
        ImGui::SetTooltip(tip);
}

/// Begins a disabled section
inline void beginDisabled(bool disabled = true) {
    if (disabled) {
        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
    }
}

/// Ends a disabled section
inline void endDisabled(bool disabled = true) {
    if (disabled) {
        ImGui::PopItemFlag();
        ImGui::PopStyleVar();
    }
}

/// Sets a window rectangle
inline void setWindowRect(const FloatRect& rect) {
    ImGui::SetNextWindowPos(rect.getPosition(), ImGuiCond_Always);
    ImGui::SetNextWindowSize(rect.getSize(),    ImGuiCond_Always);
}

} // helpers