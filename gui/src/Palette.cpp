#include "Palette.hpp"
#include "Nodes.hpp"
#include "Gui.hpp"
#include <unordered_map>

using namespace mahi::gui;

/// Returns the name of a Syntacts signal
const std::string &paletteName(PItem id)
{
    static std::unordered_map<PItem, std::string> names = {
        {PItem::Unknown, "Unknown"},
        {PItem::Time, "Time"},
        {PItem::Scalar, "Scalar"},
        {PItem::Ramp, "Ramp"},
        {PItem::Noise, "Noise"},
        {PItem::Expression, "Expression"},
        {PItem::Sum, "Sum"},
        {PItem::Product, "Product"},
        {PItem::Repeater, "Repeater"},
        {PItem::Stretcher, "Stretcher"},
        {PItem::Reverser, "Reverser"},
        {PItem::Sequencer, "Sequencer"},
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
        {PItem::PolyBezier, "PolyBezier"}};
    if (names.count(id))
        return names[id];
    else
        return names[PItem::Unknown];
}

const std::string& paletteDescription(PItem id) {
    static std::unordered_map<PItem, std::string> desc = {
        {PItem::Unknown, "Hmm, we don't know what this does ..."},
        {PItem::Time, "Creates a sample equal to the sample time"},
        {PItem::Scalar, "Creates a constant sample value"},
        {PItem::Ramp, "Creates a sample that linearly increases or decreases with time"},
        {PItem::Noise, "Creates white noise"},
        {PItem::Expression, "Creates a sample by evaluating a mathematical expression"},
        {PItem::Sum, "Sums two or more Signals"},
        {PItem::Product, "Multiplies two or more Signals"},
        {PItem::Repeater, "Repeats a Signal a specified number of times"},
        {PItem::Stretcher, "Stretches or compresses a Signal in time"},
        {PItem::Reverser, "Reverses a Signal"},
        {PItem::Sequencer, "Sequences multiple Signals in time"},
        {PItem::Sine, "Creates a basic sine wave oscillator"},
        {PItem::Square, "Creates a basic square wave oscillator"},
        {PItem::Saw, "Creates a basic saw wave oscillator"},
        {PItem::Triangle, "Creates a basic triangle wave oscillator"},
        {PItem::FM, "Creates a frequency modulated oscillator"},
        {PItem::Chirp, "Creates an oscillator with an linearly increasing or decreasing frequency"},
        {PItem::Pwm, "Creates a pulse width modulated oscillator"},
        {PItem::Envelope, "Creates a basic envelope"},
        {PItem::ASR, "Creates an Attack, Sustain, Release envelope"},
        {PItem::ADSR, "Creates an Attack, Decay, Sustain, Release envelope"},
        {PItem::ExponentialDecay, "Creates an exponentially decaying envelope"},
        {PItem::KeyedEnvelope, "Creates a envelope from a series of key frame times, amplitudes, and Curves"},
        {PItem::SignalEnvelope, "Creates an envelope from a Signal"},
        {PItem::PolyBezier, "Creates an envelope from a cubic bezier curve"}};
    if (desc.count(id))
        return desc[id];
    else
        return desc[PItem::Unknown];
}

void Palette::update()
{
    auto avail = ImGui::GetContentRegionAvail();
    ImGui::PushStyleColor(ImGuiCol_ChildBg, {0,0,0,0});
    ImGui::BeginChild("PalleteList", ImVec2(0, avail.y));
    static std::vector<std::pair<std::string, std::vector<PItem>>> signals = {
        {"Oscillators", {PItem::Sine, PItem::Square, PItem::Saw, PItem::Triangle, PItem::Chirp, PItem::FM, PItem::Pwm, PItem::Noise}},
        {"Envelopes", {PItem::Envelope, PItem::KeyedEnvelope, PItem::ASR, PItem::ADSR, PItem::ExponentialDecay, PItem::PolyBezier, PItem::SignalEnvelope}},
        {"Processes", {PItem::Sum, PItem::Product, PItem::Repeater, PItem::Stretcher, PItem::Reverser, PItem::Sequencer}},
        {"General", {PItem::Expression, PItem::Ramp, PItem::Scalar}}};
    for (auto &section : signals)
    {
        if (!ImGui::CollapsingHeader(section.first.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
            continue;
        for (auto &pItem : section.second)
        {
            ImGui::Selectable(paletteName(pItem).c_str(), false);
            PaletteSource(pItem);
            gui.status.showTooltip(paletteDescription(pItem));
        }
    }
    ImGui::EndChild();
    ImGui::PopStyleColor();
}