#pragma once
#include <memory>
#include <ImGui/imgui.h>
#include <syntacts>
#include <functional>

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

        std::function<void(const char* hint)> onTooltip = [&](const char* msg){ };
    };

    bool Sequencer(const char* label, SeqInterface& I);

}