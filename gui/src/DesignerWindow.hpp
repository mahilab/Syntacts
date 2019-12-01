#pragma once

#include <carnot>
#include <syntacts>
#include <deque>
#include "ImGui/ImSequencer.hpp"
#include "SignalNode.hpp"

using namespace carnot;

class DesignerWindow : public GameObject {
public:

    using GameObject::GameObject;    

    /// Builds a Syntacts Cue from the user's GUI configuration
    tact::Signal buildSignal() {
        return m_root.getOutput();
    }

private:

    ProductNode m_root;

    void start() override {
    }

    void update() override {
        helpers::setWindowRect(rect);
        ImGui::Begin(getName().c_str(), nullptr,  ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
        if (ImGui::BeginTabBar("DesignerWindowTabs")) {
            if (ImGui::BeginTabItem("Designer##Tab")) {
                ImGui::PushStyleColor(ImGuiCol_ChildBg, Color::Transparent);
                ImGui::BeginChild("Designer##TabScroll");
                m_root.renderGui();
                ImGui::EndChild();
                ImGui::PopStyleColor();
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }        
        ImGui::End();
    }

public:

    FloatRect rect;

private:

};