#pragma once
#include "Widget.hpp"
#include "ImGui/Sequencer.hpp"
#include "ImGui/Custom.hpp"

///////////////////////////////////////////////////////////////////////////////

class Sequencer : public Widget {
public:
    Sequencer(Gui* gui);
    void render() override;
    tact::Signal buildSignal();
private:
    tact::Sequence m_seq;
    std::vector<ImGui::Track> m_tracks;
    float m_tMin = 0, m_tMax = 10;
    int m_selected = 0;
    int m_currentFrame = 0;
    bool m_expanded = true;
    int m_selectedEntry = 0;
    int m_firstFrame = 0;
};
