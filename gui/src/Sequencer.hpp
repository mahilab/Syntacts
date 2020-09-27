#pragma once
#include "Widget.hpp"

///////////////////////////////////////////////////////////////////////////////
// CUSTOM IMGUI

namespace ImGui {

    struct SeqInterface {

        struct Track {
            tact::Signal signal;
            std::string label;
            double t = 0;
            bool visible = true;
            bool held = false;
            bool populated = false;
            bool expanded = false;
        };

        std::vector<Track> tracks;
        int selected = 0;
        float tMin = 0, tMax = 5, tEnd = 10;
        ImVec4 activeColor = {1,1,1,1};
        ImVec4 inactiveColor = {0,0,0,1};
        bool milliseconds = false;
        bool snap = true;
        bool lg = false, rg = false, cg = false;
        bool fitThisFrame = false;

        std::function<void(const char* hint)> onTooltip = [&](const char* msg){ };
    };

    bool Sequencer(const char* label, SeqInterface& I, bool fullFrame = true);

}

///////////////////////////////////////////////////////////////////////////////

class Sequencer : public Widget {
public:
    Sequencer(Gui& gui);
    void update() override;
    tact::Signal buildSignal();
private:
    tact::Sequence m_seq;
    ImGui::SeqInterface m_interface;
};
