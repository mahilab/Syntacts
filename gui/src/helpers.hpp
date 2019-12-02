#pragma once

#include <carnot>
#include <syntacts>
#include <Engine/ImGui/imgui_internal.h>
#include <string>
#include <unordered_map>

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

const std::unordered_map<std::type_index, std::string> nameMap() {
    static std::unordered_map<std::type_index, std::string> names = {
        // {typeid(tact::Zero),           "Zero"},
        {typeid(tact::Time),           "Time"},
        {typeid(tact::Scalar),         "Scalar"},
        {typeid(tact::Ramp),           "Ramp"},
        {typeid(tact::Noise),          "Noise"},
        {typeid(tact::Expression),     "Expression"},
        {typeid(tact::Sum),            "Sum"},
        {typeid(tact::Product),        "Product"},
        {typeid(tact::Sine),           "Sine"},
        {typeid(tact::Square),         "Square"},
        {typeid(tact::Saw),            "Saw"},
        {typeid(tact::Triangle),       "Triangle"},
        {typeid(tact::SineFM),         "Sine FM"},
        {typeid(tact::Chirp),          "Chirp"},
        {typeid(tact::Pwm),            "PWM"},
        {typeid(tact::Envelope),       "Envelope"},
        {typeid(tact::ASR),            "ASR"},
        {typeid(tact::ADSR),           "ADSR"},
        {typeid(tact::KeyedEnvelope),  "Keyed Envelope"},
        {typeid(tact::SignalEnvelope), "Signal Envelope"}
    };
    return names;
}

/// Returns the name of a Syntacts signal
const std::string& signalName(std::type_index id) {
    static std::string unkown = "Unkown";
    static std::unordered_map<std::type_index, std::string> names = {
        // {typeid(tact::Zero),           "Zero"},
        {typeid(tact::Time),           "Time"},
        {typeid(tact::Scalar),         "Scalar"},
        {typeid(tact::Ramp),           "Ramp"},
        {typeid(tact::Noise),          "Noise"},
        {typeid(tact::Expression),     "Expression"},
        {typeid(tact::Sum),            "Sum"},
        {typeid(tact::Product),        "Product"},
        {typeid(tact::Sine),           "Sine"},
        {typeid(tact::Square),         "Square"},
        {typeid(tact::Saw),            "Saw"},
        {typeid(tact::Triangle),       "Triangle"},
        {typeid(tact::SineFM),         "Sine FM"},
        {typeid(tact::Chirp),          "Chirp"},
        {typeid(tact::Pwm),            "PWM"},
        {typeid(tact::Envelope),       "Envelope"},
        {typeid(tact::ASR),            "ASR"},
        {typeid(tact::ADSR),           "ADSR"},
        {typeid(tact::KeyedEnvelope),  "Keyed Envelope"},
        {typeid(tact::SignalEnvelope), "Signal Envelope"}
    };
    if (names.count(id))
        return names[id];
    else
        return unkown;    
}

const std::string& signalName(const tact::Signal& sig) {
    return signalName(sig.typeId());
}

bool signalHeld = false;

void nodeSlot(bool renderAlways = false) {
    if (signalHeld || renderAlways) {
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImGui::GetStyle().Colors[ImGuiCol_TabActive]);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered,ImGui::GetStyle().Colors[ImGuiCol_TabActive]);
        ImGui::PushStyleColor(ImGuiCol_Button,       ImGui::GetStyle().Colors[ImGuiCol_TabActive]);
        ImGui::Button("##SignalDND", ImVec2(-1, 0));
        ImGui::PopStyleColor(3);
    }
}

bool nodeDropped(std::type_index& dropped) {
    bool ret = false;    
    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_SIGNAL")) { 
            dropped = *(std::type_index*)payload->Data;
            ret = true;
        }
        ImGui::EndDragDropTarget();
    }
    return ret;
}

} // helpers