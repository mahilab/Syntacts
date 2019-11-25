#pragma once

#include <carnot>
#include <syntacts>
#include "helpers.hpp"
#include "ImGui/ImSequencer.hpp"

using namespace carnot;

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

class DesignerWindow : public GameObject {
public:


public:

    using GameObject::GameObject;

    
    /// Builds the Syntacts Envelope
    tact::Signal buildEnv() {
        return tact::Signal();
    }

    /// Builds a Syntacts Cue from the user's GUI configuration
    tact::Signal buildCue() {
        return tact::Signal();
    }

private:

    CueSequence m_sequence;
    int m_selectedEntry = -1;
    int m_firstFrame = 0;
    bool m_expanded = true;
    int m_currentFrame = 0;


    void start() override {
        m_sequence.frameMin = 0;
        m_sequence.frameMax = 100;
        m_sequence.items.push_back(CueSequence::Item{0, 0, 20, false});
    }

    void update() override {
        helpers::setWindowRect(rect);
        ImGui::Begin(getName().c_str(), nullptr,  ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
        if (ImGui::BeginTabBar("DesignerWindowTabs")) {
            if (ImGui::BeginTabItem("Designer##Tab")) {
                updateEnvelope();
                updateCarrier();
                updateModulation();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Sequencer##Tab")) {

                ImSequencer::Sequencer(&m_sequence, &m_currentFrame, &m_expanded, &m_selectedEntry, &m_firstFrame, 
                      ImSequencer::SEQUENCER_EDIT_STARTEND | ImSequencer::SEQUENCER_ADD | ImSequencer::SEQUENCER_DEL | ImSequencer::SEQUENCER_CHANGE_FRAME);
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
        
        ImGui::End();
    }

  
    /// Updates the carrier section
    void updateCarrier() {
        if (!ImGui::CollapsingHeader("Carrier"))
            return;
        ImGui::Indent();
        ImGui::Unindent();
    }

    /// Updates the modulation section
    void updateModulation() {
        if (!ImGui::CollapsingHeader("Modulation"))
            return;
        ImGui::Indent();

        ImGui::Unindent();
    }

    /// Updates the Envelope section
    void updateEnvelope() {
        if (!ImGui::CollapsingHeader("Envelope"))
            return;
        ImGui::Indent();

        ImGui::Unindent();
    }

        /// Builds the carrier oscillator
    tact::Signal buildCarOsc() {
        return tact::Signal();
    }

    /// Builds the modulation oscillator
   tact::Signal buildModOsc() {
       return tact::Signal();
    }

public:

    FloatRect rect;

private:

};