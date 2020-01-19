#include "Sequencer.hpp"

Sequencer::Sequencer(Gui* gui)
    : Widget(gui)
{
    m_tracks.emplace_back();
    m_tracks.back().label = "New Track";
}

void Sequencer::render()
{
    ImGui::Sequencer("Sequencer", m_tracks, m_selected, m_tMin, m_tMax, carnot::Oranges::DarkOrange);
    
}

tact::Signal Sequencer::buildSignal() {
    m_seq.clear();
    for (auto& track : m_tracks) {
        if (track.populated && track.visible)
            m_seq.insert(track.signal, track.t);
    }
    return m_seq;
}