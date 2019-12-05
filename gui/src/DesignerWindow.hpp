#pragma once

#include <carnot>
#include <syntacts>
#include <deque>
#include "SignalNodes.hpp"
#include "VisualizerWindow.hpp"

using namespace carnot;


class DesignerWindow : public GameObject {
public:

    using GameObject::GameObject;    

    /// Builds a Syntacts Cue from the user's GUI configuration
    tact::Signal buildSignal() {
        return m_root.signal();
    }

private:
    ProductNode m_root;    
    Handle<VisualizerWindow> m_vis;

    void start() override {
        m_vis = findSibling<VisualizerWindow>();
    }

    void test() {

        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
        ImGui::Button("##TestingHover", ImVec2(-1,0));
        
        ImGui::PopItemFlag();  
        if (ImGui::IsItemHovered()) 
            ImGui::PushStyleColor(ImGuiCol_Button,       ImGui::GetStyle().Colors[ImGuiCol_TabHovered]);
        else 
            ImGui::PushStyleColor(ImGuiCol_Button,       ImGui::GetStyle().Colors[ImGuiCol_TabActive]);
        ImGui::PushStyleColor(ImGuiCol_FrameBg, Color::Transparent);
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, Color::Transparent);
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive,  Color::Transparent);   

        ImGui::SameLine(1);
        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
        ImGui::Button("##Testing", ImVec2(-1,0));
        ImGui::PopItemFlag();  
        
        ImGui::SameLine(1);

        ImGui::ArrowButton("##Arrow", ImGuiDir_Right);
        ImGui::SameLine();
        ImGui::Text("Product");
        ImGui::SameLine(420);
        static float x = 1.0f;
        static float o = 0;
        ImGui::PushItemWidth(50);
        ImGui::DragFloat("##GAIN", &x, 0.01, -1, 1, "x%.2f");
        ImGui::SameLine();
        ImGui::DragFloat("##OFF", &o, 0.01, -10, 10, o >= 0 ? "+%.2f" : "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine(540);
        ImGui::Button(ICON_FA_MINUS_CIRCLE);
                ImGui::PopStyleColor(4);
    }

    void update() override {
        helpers::setWindowRect(rect);
        ImGui::Begin(getName().c_str(), nullptr,  ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
        /// render designer
        if (ImGui::BeginTabBar("DesignerWindowTabs")) {
            if (ImGui::BeginTabItem("Designer##Tab")) {
                ImGui::PushStyleColor(ImGuiCol_ChildBg, Color::Transparent);
                ImGui::BeginChild("Designer##TabScroll");
                // test();
                m_root.gui();
                ImGui::EndChild();
                ImGui::PopStyleColor();
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }     
        m_vis->setRenderedSignal(buildSignal(), hexCode("cf94c2"));
        ImGui::End();
    }

public:
    FloatRect rect;
private:

};