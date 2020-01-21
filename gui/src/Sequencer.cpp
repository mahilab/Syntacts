#include "Sequencer.hpp"
#include "Gui.hpp"

Sequencer::Sequencer(Gui* gui)
    : Widget(gui)
{
    m_interface.tracks.emplace_back();
    m_interface.tracks.back().label = "Track 1";
    m_interface.activeColor = carnot::Oranges::DarkOrange;
    m_interface.inactiveColor = carnot::Grays::Gray50;
    m_interface.onTooltip = [gui](const char* msg) { gui->status->showTooltip(msg); };
}

void Sequencer::render()
{
    ImGui::Sequencer("Sequencer", m_interface);    
}

tact::Signal Sequencer::buildSignal() {
    m_seq.clear();
    for (auto& track : m_interface.tracks) {
        if (track.populated && track.visible)
            m_seq.insert(track.signal, track.t);
    }
    return m_seq;
}