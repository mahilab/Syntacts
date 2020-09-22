#include "Palette.hpp"
#include "Nodes.hpp"
#include <unordered_map>

using namespace mahi::gui;

/// Returns the name of a Syntacts signal
const std::string &palletteString(PItem id)
{
    static std::string unkown = "Unkown";
    static std::unordered_map<PItem, std::string> names = {
        {PItem::Unknown, "Unknown"},
        {PItem::Time, "Time"},
        {PItem::Scalar, "Scalar"},
        {PItem::Ramp, "Ramp"},
        {PItem::Noise, "Noise"},
        {PItem::Expression, "Expression"},
        {PItem::Sum, "Sum"},
        {PItem::Product, "Product"},
        {PItem::Filter, "Filter"},
        {PItem::Repeater, "Repeater"},
        {PItem::Stretcher, "Stretcher"},
        {PItem::Reverser, "Reverser"},
        {PItem::Sine, "Sine"},
        {PItem::Square, "Square"},
        {PItem::Saw, "Saw"},
        {PItem::Triangle, "Triangle"},
        {PItem::FM, "FM"},
        {PItem::Chirp, "Chirp"},
        {PItem::Pwm, "PWM"},
        {PItem::Envelope, "Envelope"},
        {PItem::ASR, "ASR"},
        {PItem::ADSR, "ADSR"},
        {PItem::ExponentialDecay, "Exponential Decay"},
        {PItem::KeyedEnvelope, "Keyed Envelope"},
        {PItem::SignalEnvelope, "Signal Envelope"},
        {PItem::PolyLine, "PolyLine"},
        {PItem::PolyBezier, "PolyBezier"}};
    if (names.count(id))
        return names[id];
    else
        return names[PItem::Unknown];
}

void Palette::update()
{
    auto avail = ImGui::GetContentRegionAvail();
    ImGui::PushStyleColor(ImGuiCol_ChildBg, {0,0,0,0});
    ImGui::BeginChild("PalleteList", ImVec2(0, avail.y));
    static std::vector<std::pair<std::string, std::vector<PItem>>> signals = {
        {"Oscillators", {PItem::Sine, PItem::Square, PItem::Saw, PItem::Triangle, PItem::Chirp, PItem::FM, PItem::Pwm, PItem::Noise}},
        {"Envelopes", {PItem::Envelope, PItem::ASR, PItem::ADSR, PItem::ExponentialDecay, PItem::PolyBezier}},
        {"Processes", {PItem::Sum, PItem::Product, PItem::Repeater, PItem::Stretcher, PItem::Reverser}},
        {"General", {PItem::Expression, PItem::Ramp, PItem::Scalar}}};
    for (auto &section : signals)
    {
        if (!ImGui::CollapsingHeader(section.first.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
            continue;
        for (auto &pItem : section.second)
        {
            ImGui::Selectable(palletteString(pItem).c_str(), false);
            PaletteSource(pItem);
        }
    }
    ImGui::EndChild();
    ImGui::PopStyleColor();
}