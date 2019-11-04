#pragma once

#include <carnot>
#include <Syntacts/Syntacts.hpp>
#include "helpers.hpp"
#include "ImGui/ImSequencer.hpp"

using namespace carnot;

const std::vector<const char*> g_tweenStrings = {
    "Linear", 
    "Smoothstep",
    "Smootherstep",
    "Smootheststep",
    "Sinusoidal::In",
    "Sinusoidal::Out",
    "Sinusoidal::InOut",
    "Exponential::In",
    "Exponential::Out",
    "Exponential::InOut"
};

const std::vector<std::shared_ptr<tact::Tween::Function>> g_tweenFuncs = {
    std::make_shared<tact::Tween::Linear>(),
    std::make_shared<tact::Tween::Smoothstep>(), 
    std::make_shared<tact::Tween::Smootherstep>(), 
    std::make_shared<tact::Tween::Smootheststep>(), 
    std::make_shared<tact::Tween::Sinusoidal::In>(), 
    std::make_shared<tact::Tween::Sinusoidal::Out>(), 
    std::make_shared<tact::Tween::Sinusoidal::InOut>(), 
    std::make_shared<tact::Tween::Exponential::In>(), 
    std::make_shared<tact::Tween::Exponential::Out>(), 
    std::make_shared<tact::Tween::Exponential::InOut>()
};

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
        	*color = ImGui::GetColorU32(ImGuiCol_TextDisabled,0.5f); // same color for everyone, return color based on type
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

    enum OscType : int { Sine = 0, Square = 1, Saw = 2, Triangle = 3, Chirp = 4 };
    enum ModMode : int { Off = 0, AM = 1, FM = 2 };
    enum EnvMode : int { Basic = 0, ASR = 1, ADSR = 2, PulseTrain = 3, Custom = 4 };

public:

    using GameObject::GameObject;

    
    /// Builds the Syntacts Envelope
    Ptr<tact::Envelope> buildEnv() {
        if (m_envMode == EnvMode::Basic)
            return make<tact::AmplitudeEnvelope>(m_duration/1000.0f, m_amps[0]);    
        else if (m_envMode == EnvMode::ASR)
            return make<tact::ASR>(m_asr[0]/1000.0f, m_asr[1]/1000.0f, m_asr[2]/1000.f, m_amps[0], g_tweenFuncs[m_tweenModes[0]], g_tweenFuncs[m_tweenModes[2]]);
        else if (m_envMode == EnvMode::ADSR)
            return make<tact::ADSR>(m_adsr[0]/1000.0f, m_adsr[1]/1000.0f, m_adsr[2]/1000.0f, m_adsr[3]/1000.f, m_amps[0], m_amps[1], g_tweenFuncs[m_tweenModes[0]], g_tweenFuncs[m_tweenModes[1]], g_tweenFuncs[m_tweenModes[3]]);
        else {
            auto pulse = make<tact::PulseTrain>((float)m_pwmValues[0], (float)m_pwmValues[1] / 100.0f);
            return make<tact::OscillatingEnvelope>(m_duration/1000.0f, m_amps[0], pulse);
        }
    }

    /// Builds a Syntacts Cue from the user's GUI configuration
    Ptr<tact::Cue> buildCue() {
        auto env = buildEnv();
        auto osc = buildCarOsc();
        if (m_modMode == 1 && m_modFreq > 0) {
            auto modOsc = buildModOsc();
            return make<tact::Cue>(osc, modOsc, env);
        }
        else
            return make<tact::Cue>(osc, env);
            
    }

private:

    CueSequence m_sequence;
    int m_selectedEntry = -1;
    int m_firstFrame = 0;
    bool m_expanded = true;
    int m_currentFrame = 0;

    void start() override {
        m_sequence.frameMin = 0;
        m_sequence.frameMax = 200;
        m_sequence.items.push_back(CueSequence::Item{0, 0, 100, false});
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
        ImGui::RadioButton("Sine##C", &m_carType, OscType::Sine);   
        if (m_modMode != ModMode::FM) {
            ImGui::SameLine();
            ImGui::RadioButton("Square##C",   &m_carType, OscType::Square);   ImGui::SameLine();
            ImGui::RadioButton("Saw##C",      &m_carType, OscType::Saw);      ImGui::SameLine();
            ImGui::RadioButton("Triangle##C", &m_carType, OscType::Triangle); ImGui::SameLine();
            ImGui::RadioButton("Chirp##C",    &m_carType, OscType::Chirp);
            if (m_carType == OscType::Chirp) {
                ImGui::DragFloat("Chirpyness", &m_chirp, 1.0f, 0.0f, 500.0f);
            }
        }
        else {
            m_carType = OscType::Sine;
        }
        ImGui::DragInt("Frequency##C", &m_carFreq, 0.5f, 0, 500, "%i Hz");
        ImGui::Unindent();
    }

    /// Updates the modulation section
    void updateModulation() {
        if (!ImGui::CollapsingHeader("Modulation"))
            return;
        ImGui::Indent();
        ImGui::RadioButton("Off", &m_modMode, ModMode::Off); ImGui::SameLine();
        ImGui::RadioButton("AM",  &m_modMode, ModMode::AM);  ImGui::SameLine();
        ImGui::RadioButton("FM",  &m_modMode, ModMode::FM);
        if (m_modMode == ModMode::Off) {

        }
        else if (m_modMode == ModMode::AM) {
            ImGui::RadioButton("Sine##M",     &m_modType, OscType::Sine);      ImGui::SameLine();
            ImGui::RadioButton("Square##M",   &m_modType, OscType::Square);    ImGui::SameLine();
            ImGui::RadioButton("Saw##M",      &m_modType, OscType::Saw);       ImGui::SameLine();
            ImGui::RadioButton("Triangle##M", &m_modType, OscType::Triangle);  
            ImGui::DragInt("Frequency##M", &m_modFreq, 0.5f,  0, 500, "%i Hz");
        }
        else if (m_modMode == ModMode::FM) {            
            ImGui::DragInt("Frequency##M", &m_modFreq, 0.5f,  0, 250, "%i Hz");
            ImGui::DragFloat("Modulation Index", &m_modIdx, 0.01f, 0.0f, 12.0f);
        }
        ImGui::Unindent();
    }

    /// Updates the Envelope section
    void updateEnvelope() {
        if (!ImGui::CollapsingHeader("Envelope"))
            return;
        ImGui::Indent();
        ImGui::RadioButton("Basic",&m_envMode, EnvMode::Basic); ImGui::SameLine();
        ImGui::RadioButton("ASR",  &m_envMode, EnvMode::ASR);   ImGui::SameLine();
        ImGui::RadioButton("ADSR", &m_envMode, EnvMode::ADSR);  ImGui::SameLine();
        ImGui::RadioButton("Pulse Train", &m_envMode, EnvMode::PulseTrain);
        int numTweens = 0;
        int skip = 0;
        if (m_envMode == EnvMode::Basic) {
            numTweens = 1;
            skip = 0;
            ImGui::DragFloat("Amplitude", &m_amps[0], 0.005f, 0.0f, 1.0f);
            ImGui::DragInt("Duration", &m_duration, 0.5f, 0, 5000, "%i ms");
        }
        if (m_envMode == EnvMode::ASR) {
            numTweens = 3;
            skip = 1;
            ImGui::DragFloat("Amplitude", &m_amps[0], 0.005f, 0.0f, 1.0f);
            ImGui::DragInt3("ASR##",&m_asr[0], 0.5f, 0, 1000, "%i ms");
            m_adsr[0] = m_asr[0];
            m_adsr[2] = m_asr[1];
            m_adsr[3] = m_asr[2];
            m_duration = m_asr[0] + m_asr[1] + m_asr[2];
        }
        else if (m_envMode == EnvMode::ADSR) {
            numTweens = 4;
            skip = 2;
            ImGui::DragFloat2("Amplitudes", &m_amps[0], 0.005f, 0.0f, 1.0f);
            ImGui::DragInt4("ADSR##",&m_adsr[0], 0.5f, 0, 1000, "%i ms");
            m_asr[0] = m_adsr[0];
            m_asr[1] = m_adsr[2];
            m_asr[2] = m_adsr[3];
            m_duration = m_adsr[0] + m_adsr[1] + m_adsr[2] + m_adsr[3];
        }
        else if (m_envMode == EnvMode::PulseTrain) {
            numTweens = -1;
            skip = 0;
            ImGui::DragFloat("Amplitude", &m_amps[0], 0.005f, 0.0f, 1.0f);
            ImGui::DragInt("Duration", &m_duration, 0.5f, 0, 5000, "%i ms");
            ImGui::DragInt2("Frequency (Hz) / Duty Cycle (%)", &m_pwmValues[0], 0.5f, 0, 100);
        }
        else if (m_envMode == EnvMode::Custom) {
            numTweens = 4;
            skip = 0;
            m_amps.resize(5);
            // float amin = 0.0f;
            // float amax = 1.0f;
            // float tmin = 0.0f;
            // float tmax = 10.0f;
            ImGui::DragFloat4("Amplitudes", &m_amps[0], 0.005f, 0.0f, 1.0f);
            ImGui::DragInt4("Times##",&m_adsr[0], 0.5f, 0, 1000, "%i ms");

        }
        if (numTweens != -1){
            ImGui::BeginGroup();
            ImGui::PushID("Tweens");
            ImGui::PushMultiItemsWidths(numTweens);
            ImGuiContext& g = *GImGui;
            for (int i = 0; i < numTweens; ++i) {
                ImGui::PushID(i);
                
                if (i == skip)
                    ImGui::Dummy(ImGui::GetItemRectSize());
                else {
                    if (i == numTweens - 1)
                        ImGui::Combo("Tween Modes", &m_tweenModes[i], &g_tweenStrings[0], (int)g_tweenStrings.size());
                    else
                        ImGui::Combo("##i", &m_tweenModes[i], &g_tweenStrings[0], (int)g_tweenStrings.size());
                }
                ImGui::SameLine(0, g.Style.ItemInnerSpacing.x);
                ImGui::PopID();
                ImGui::PopItemWidth();
            }
            ImGui::PopID();
            ImGui::EndGroup();
        }
        ImGui::Unindent();
    }

        /// Builds the carrier oscillator
    Ptr<tact::Oscillator> buildCarOsc() {
        if (m_modMode != ModMode::FM) {
            if (m_carType == OscType::Sine)
                return make<tact::SineWave>((float)m_carFreq);
            else if (m_carType == OscType::Square)
                return make<tact::SquareWave>((float)m_carFreq);
            else if (m_carType == OscType::Saw)
                return make<tact::SawWave>((float)m_carFreq);
            else if (m_carType == OscType::Triangle)
                return make<tact::TriWave>((float)m_carFreq);  
            else 
                return make<tact::Chirp>((float)m_carFreq, m_chirp);
        }       
        else {
            return make<tact::SineWaveFM>((float)m_carFreq, (float)m_modFreq, m_modIdx);
        }
    }

    /// Builds the modulation oscillator
    Ptr<tact::Oscillator> buildModOsc() {
        Ptr<tact::Oscillator> modOsc;
        if (m_modType == OscType::Sine)
            modOsc = make<tact::SineWave>((float)m_modFreq);
        else if (m_modType == OscType::Square)
            modOsc = make<tact::SquareWave>((float)m_modFreq);
        else if (m_modType == OscType::Saw)
            modOsc = make<tact::SawWave>((float)m_modFreq);
        else if (m_modType == OscType::Triangle)
            modOsc = make<tact::TriWave>((float)m_modFreq);  
        return modOsc;
    }


public:

    FloatRect rect;

private:

    int   m_carFreq = 175;
    int   m_carType = 0;

    int   m_modMode = ModMode::AM;
    int   m_modFreq = 25;
    int   m_modType = OscType::Sine;
    float m_modIdx  = 2.0f;

    float m_chirp = 200;

    int   m_envMode  = EnvMode::ASR;
    std::vector<float> m_amps = {1.0f, 0.5f};
    int   m_duration = 150;
    std::vector<int> m_pwmValues = {10, 25};
    std::vector<int> m_asr   = {50, 75, 25};    
    std::vector<int> m_adsr  = {50,50,75,25};
    std::vector<int> m_tweenModes = {0,0,0,0};

};