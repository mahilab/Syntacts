#pragma once
#include <memory>
#include <ImGui/imgui.h>
#include <syntacts>

namespace ImGui {

    struct Track {
        tact::Signal signal;
        std::string label;
        float t = 0;
        bool visible = true;
        bool held = false;
        bool populated = false;
    };

    bool Sequencer(const char* label, std::vector<Track>& tracks, int& selected, float tMin, float tMax, ImVec4 color);

}