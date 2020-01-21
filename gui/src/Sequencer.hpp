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
    ImGui::SeqInterface m_interface;
};
