#pragma once

#include <carnot>
#include <syntacts>
#include <deque>
#include "SignalNodes.hpp"
#include "VisualizerWindow.hpp"
#include "ImGui/ImSequencer.hpp"

using namespace carnot;

///////////////////////////////////////////////////////////////////////////////

struct CueSequence : public ImSequencer::SequenceInterface {

    struct Item
    {
        int type;
        int frameStart, frameEnd;
        bool expanded;
    };

    int frameMin, frameMax;
    std::vector<Item> items;

    CueSequence() : frameMin(0), frameMax(0) {}

    virtual int GetFrameMin() const override { return frameMin; }
    virtual int GetFrameMax() const override { return frameMax; }
    virtual int GetItemCount() const override { return (int)items.size(); }
    virtual int GetItemTypeCount() const override { return 1; }
    virtual const char *GetItemTypeName(int typeIndex) const override {
        static char typeName[] = "TypeName";
        return typeName;
    }
    virtual const char *GetItemLabel(int index) const override {
        static char tmp[512];
        sprintf_s(tmp, "item_%02d", index);
        return tmp;
    }
    virtual void Add(int type) { items.push_back(Item{type, 0, 100, false}); };
    virtual void Del(int index) { items.erase(items.begin() + index); }
    virtual void Duplicate(int index) { items.push_back(items[index]); }
    
	virtual void Get(int index, int** start, int** end, int *type, unsigned int *color) {
        Item& item = items[index];
        if (color)
        	*color = 0x801080FF; // same color for everyone, return color based on type
		if (start)
			*start = &item.frameStart;
		if (end)
			*end   = &item.frameEnd;
		if (type)
			*type  = item.type;    
    }
};
///////////////////////////////////////////////////////////////////////////////

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
    CueSequence m_sequence;
    int m_currentFrame = 0;
    bool m_expanded = true;
    int m_selectedEntry = 0;
    int m_firstFrame = 0;

    void start() override {
        m_vis = findSibling<VisualizerWindow>();

        m_sequence.frameMin = 0;
        m_sequence.frameMax = 100;
        m_sequence.items.push_back(CueSequence::Item{0, 0, 20, false});
        
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
            if (ImGui::BeginTabItem("Sequencer##Tab")) {

            ImSequencer::Sequencer(&m_sequence, &m_currentFrame, &m_expanded, &m_selectedEntry, &m_firstFrame, 
                    ImSequencer::SEQUENCER_EDIT_STARTEND | ImSequencer::SEQUENCER_ADD | ImSequencer::SEQUENCER_DEL | ImSequencer::SEQUENCER_CHANGE_FRAME);
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