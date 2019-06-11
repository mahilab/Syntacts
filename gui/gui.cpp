#define CARNOT_NO_CONSOLE
// #define CARNOT_USE_DISCRETE_GPU
#ifndef IMGUI_DEFINE_MATH_OPERATORS
    #define IMGUI_DEFINE_MATH_OPERATORS
#endif
#ifndef SAMPLE_RATE
    #define SAMPLE_RATE 44100
#endif

#include <carnot>
#include <Syntacts/Syntacts.hpp>
#include <deque>
#include <functional>
#include <chrono>
#include <ctime>  
#include <string>
#include <windows.h>
#include <Engine/ImGui/imgui_internal.h>
#include <Common/Tween.hpp>

using namespace carnot;

const std::vector<const char*>     g_tweenStrings = {"Linear",            "Smoothstep",            "Smootherstep",            "Smootheststep",            "Sinusoidal::In",            "Sinusoidal::Out",            "Sinusoidal::InOut"           , "Exponential::In",            "Exponential::Out",            "Exponential::InOut"           };
const std::vector<tact::TweenFunc> g_tweenFuncs   = {tact::Tween::Linear, tact::Tween::Smoothstep, tact::Tween::Smootherstep, tact::Tween::Smootheststep, tact::Tween::Sinusoidal::In, tact::Tween::Sinusoidal::Out, tact::Tween::Sinusoidal::InOut, tact::Tween::Exponential::In, tact::Tween::Exponential::Out, tact::Tween::Exponential::InOut};

//==============================================================================
// IMGUI CUSTOM PLOTTING
//==============================================================================

namespace ImGui {

void PlotEx2(ImGuiPlotType plot_type, const char* label, float (*values_getter)(void* data, int idx), void* data1, void* data2, int values_count, int values_offset, const char* overlay_text, float scale_min, float scale_max, ImVec2 graph_size)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;

    const ImVec2 label_size = CalcTextSize(label, NULL, true);
    if (graph_size.x == 0.0f)
        graph_size.x = CalcItemWidth();
    if (graph_size.y == 0.0f)
        graph_size.y = label_size.y + (style.FramePadding.y * 2);

    const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(graph_size.x, graph_size.y));
    const ImRect inner_bb(frame_bb.Min + style.FramePadding, frame_bb.Max - style.FramePadding);
    const ImRect total_bb(frame_bb.Min, frame_bb.Max + ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0));
    ItemSize(total_bb, style.FramePadding.y);
    if (!ItemAdd(total_bb, 0, &frame_bb))
        return;

    // Determine scale from values if not specified
    if (scale_min == FLT_MAX || scale_max == FLT_MAX)
    {
        float v_min = FLT_MAX;
        float v_max = -FLT_MAX;
        for (int i = 0; i < values_count; i++)
        {
            const float v = values_getter(data1, i);
            v_min = ImMin(v_min, v);
            v_max = ImMax(v_max, v);
        }
        if (scale_min == FLT_MAX)
            scale_min = v_min;
        if (scale_max == FLT_MAX)
            scale_max = v_max;
    }

    RenderFrame(frame_bb.Min, frame_bb.Max, GetColorU32(ImGuiCol_FrameBg), true, style.FrameRounding);

    if (values_count > 0)
    {
        int res_w = ImMin((int)graph_size.x, values_count) + ((plot_type == ImGuiPlotType_Lines) ? -1 : 0);
        int item_count = values_count + ((plot_type == ImGuiPlotType_Lines) ? -1 : 0);

        const float t_step = 1.0f / (float)res_w;
        const float inv_scale = (scale_min == scale_max) ? 0.0f : (1.0f / (scale_max - scale_min));

        float v0 = values_getter(data1, (0 + values_offset) % values_count);
        float t0 = 0.0f;
        ImVec2 tp0 = ImVec2( t0, 1.0f - ImSaturate((v0 - scale_min) * inv_scale) );                       // Point in the normalized space of our target rectangle
        float histogram_zero_line_t = (scale_min * scale_max < 0.0f) ? (-scale_min * inv_scale) : (scale_min < 0.0f ? 0.0f : 1.0f);   // Where does the zero line stands

        const ImU32 col_base = GetColorU32(ImGuiCol_PlotLines);
        for (int n = 0; n < res_w; n++)
        {
            const float t1 = t0 + t_step;
            const int v1_idx = (int)(t0 * item_count + 0.5f);
            IM_ASSERT(v1_idx >= 0 && v1_idx < values_count);
            const float v1 = values_getter(data1, (v1_idx + values_offset + 1) % values_count);
            const ImVec2 tp1 = ImVec2( t1, 1.0f - ImSaturate((v1 - scale_min) * inv_scale) );

            // NB: Draw calls are merged together by the DrawList system. Still, we should render our batch are lower level to save a bit of CPU.
            ImVec2 pos0 = ImLerp(inner_bb.Min, inner_bb.Max, tp0);
            ImVec2 pos1 = ImLerp(inner_bb.Min, inner_bb.Max, tp1);
            window->DrawList->AddLine(pos0, pos1, col_base); 

            t0 = t1;
            tp0 = tp1;
        }
    }

    if (values_count > 0)
    {
        int res_w = ImMin((int)graph_size.x, values_count) + ((plot_type == ImGuiPlotType_Lines) ? -1 : 0);
        int item_count = values_count + ((plot_type == ImGuiPlotType_Lines) ? -1 : 0);

        const float t_step = 1.0f / (float)res_w;
        const float inv_scale = (scale_min == scale_max) ? 0.0f : (1.0f / (scale_max - scale_min));

        float v0 = values_getter(data2, (0 + values_offset) % values_count);
        float t0 = 0.0f;
        ImVec2 tp0 = ImVec2( t0, 1.0f - ImSaturate((v0 - scale_min) * inv_scale) );                       // Point in the normalized space of our target rectangle
        float histogram_zero_line_t = (scale_min * scale_max < 0.0f) ? (-scale_min * inv_scale) : (scale_min < 0.0f ? 0.0f : 1.0f);   // Where does the zero line stands

        const ImU32 col_base = GetColorU32(withAlpha(Whites::White,0.1f));
        for (int n = 0; n < res_w; n++)
        {
            const float t1 = t0 + t_step;
            const int v1_idx = (int)(t0 * item_count + 0.5f);
            IM_ASSERT(v1_idx >= 0 && v1_idx < values_count);
            const float v1 = values_getter(data2, (v1_idx + values_offset + 1) % values_count);
            const ImVec2 tp1 = ImVec2( t1, 1.0f - ImSaturate((v1 - scale_min) * inv_scale) );

            // NB: Draw calls are merged together by the DrawList system. Still, we should render our batch are lower level to save a bit of CPU.
            ImVec2 pos0 = ImLerp(inner_bb.Min, inner_bb.Max, tp0);
            ImVec2 pos1 = ImLerp(inner_bb.Min, inner_bb.Max, tp1);
            window->DrawList->AddLine(pos0, pos1, col_base); 

            t0 = t1;
            tp0 = tp1;
        }
    }

    // Text overlay
    if (overlay_text)
        RenderTextClipped(ImVec2(frame_bb.Min.x, frame_bb.Min.y + style.FramePadding.y), frame_bb.Max, overlay_text, NULL, NULL, ImVec2(0.5f,0.0f));

    if (label_size.x > 0.0f)
        RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, inner_bb.Min.y), label);
}

struct ImGuiPlotArrayGetterData2
{
    const float* Values;
    int Stride;
    ImGuiPlotArrayGetterData2(const float* values, int stride) { Values = values; Stride = stride; }
};

static float Plot_ArrayGetter2(void* data, int idx)
{
    ImGuiPlotArrayGetterData2* plot_data = (ImGuiPlotArrayGetterData2*)data;
    const float v = *(const float*)(const void*)((const unsigned char*)plot_data->Values + (size_t)idx * plot_data->Stride);
    return v;
}

void PlotLines2(const char* label, const float* values1, const float* values2, int values_count, int values_offset = 0, const char* overlay_text = NULL, float scale_min = FLT_MAX, float scale_max = FLT_MAX, ImVec2 graph_size = ImVec2(0, 0), int stride = sizeof(float)) {
    ImGuiPlotArrayGetterData2 data1(values1, stride);
    ImGuiPlotArrayGetterData2 data2(values2, stride);
    PlotEx2(ImGuiPlotType_Lines, label, &Plot_ArrayGetter2, (void*)&data1, (void*)&data2, values_count, values_offset, overlay_text, scale_min, scale_max, graph_size);
}

}


//==============================================================================
// HELPER FUNCTIONS
//==============================================================================

// renders an ImGui tooltip
void tooltip(const std::string& tip) {
    if (ImGui::IsItemHovered()) 
        ImGui::SetTooltip(tip.c_str());
}

struct FuncHolder { 
    static bool ItemGetter(void* data, int idx, const char** out_str) { 
        auto devices = *(std::vector<tact::DeviceInfo>*)data;
        std::vector<const char*> strs(devices.size());
        for (std::size_t i = 0; i < devices.size(); ++i)
            strs[i] = devices[i].name.c_str();
        out_str = &strs[idx];
    } 
};


//==============================================================================
// GUI GAMEOBJECT
//==============================================================================

class SyntactsGui : public GameObject {
public:

    //--------------------------------------------------------------------------
    // Enums
    //--------------------------------------------------------------------------

    enum EnvMode : int { Basic = 0, ASR = 1, ADSR = 2 };
    enum ModMode : int { Off = 0, AM = 1, FM = 2 };
    enum OscType : int { Sine = 0, Square = 1, Saw = 2, Triangle = 3 };

    //--------------------------------------------------------------------------
    // Generic Functions
    //--------------------------------------------------------------------------

    /// Constructor
    SyntactsGui() : m_checkBoxes(2, false) { 
        rechannel();
    }

    /// Gets number of channels from current Syntacts device and resizes accordingly
    void rechannel() {
        m_device = tact::getCurrentDevice();
        m_checkBoxes = std::deque<bool>(m_device.maxChannels, false);
        if (m_checkBoxes.size() > 0)
            m_checkBoxes[0] = true;
    }

    /// Restarts Syntacts and connects to a specifc device
    void connect(tact::DeviceInfo device) {
        tact::finalize();
        tact::initialize(device.index, device.maxChannels);
        m_device = device;
        rechannel();
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
            else
                return make<tact::TriWave>((float)m_carFreq);   
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

    /// Builds the Syntacts Envelope
    Ptr<tact::Envelope> buildEnv() {
        if (m_envMode == EnvMode::Basic)
            return make<tact::Envelope>(m_duration/1000.0f, m_amps[0]);    
        else if (m_envMode == EnvMode::ASR)
            return make<tact::ASR>(m_asr[0]/1000.0f, m_asr[1]/1000.0f, m_asr[2]/1000.f, m_amps[0], g_tweenFuncs[m_tweenModes[0]], g_tweenFuncs[m_tweenModes[2]]);
        else 
            return make<tact::ADSR>(m_adsr[0]/1000.0f, m_adsr[1]/1000.0f, m_adsr[2]/1000.0f, m_adsr[3]/1000.f, m_amps[0], m_amps[1], g_tweenFuncs[m_tweenModes[0]], g_tweenFuncs[m_tweenModes[1]], g_tweenFuncs[m_tweenModes[3]]);
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

    /// Exports current UI configuration to Syntacts C++
    void exportCpp() {
        std::string code;
        code += "auto osc = std::make_shared<tact::";
        if (m_modMode != 2) {
            code += m_carType == 0 ? "SineWave" : m_carType == 1 ? "SquareWave" : m_carType == 2 ? "SawWave" : m_carType == 3 ? "TriWave" : "";
            code += ">(" + str((float)m_carFreq) + ");\n";
        }
        else
            code += "SineWaveFM>(" + str((float)m_carFreq) + ", " + str((float)m_modFreq) + ", " + str(m_modIdx) + ");\n";     

        if (m_envMode == 0)
            code += "auto env = std::make_shared<tact::Envelope>(" + str(m_duration/1000.0f) + ", " + str(m_amps[0]) + ");\n";
        else if (m_envMode == 1)
            code += "auto env = std::make_shared<tact::ASR>(" + str(m_asr[0]/1000.0f) + ", " + str(m_asr[1]/1000.0f) + ", " + str(m_asr[2]/1000.0f) + ", " + str(m_amps[0]) + ");\n";
        else if (m_envMode == 2)
            code += "auto env = std::make_shared<tact::ADSR>(" + str(m_adsr[0]/1000.0f) + ", " + str(m_adsr[1]/1000.0f) + ", " + str(m_adsr[2]/1000.0f) + ", " + str(m_adsr[3]/1000.0f) + ", " + str(m_amps[0]) + ", " + str(m_amps[1]) + ");\n";

        
        if (m_modMode == 1 && m_modFreq > 0) {
            code += "auto mod = std::make_shared<tact::";
            code += m_modType == 0 ? "SineWave" : m_modType == 1 ? "SquareWave" : m_modType == 2 ? "SawWave" : m_modType == 3 ? "TriWave" : "";
            code += ">(" + str((float)m_modFreq) + ");\n";
            code += "auto cue = std::make_shared<tact::Cue>(osc, mod, env);\n";
        }
        else {
            code += "auto cue = std::make_shared<tact::Cue>(osc, env);\n";
        }
        Clipboard::setString(code);
    }

    /// Creats and plays the user's cue
    void playCh(int ch) {
        tact::play(ch, buildCue());
    }

    /// Plays all selected channels
    void playSelected() {
        for (int i = 0; i < m_device.maxChannels; ++i) {
            if (m_checkBoxes[i])
                playCh(i);
        }
    }

    /// Plays the Cue over the user's speakers
    void playSpeaker() {
        auto cue = buildCue();
        std::vector<signed short> samples(cue->sampleCount());       
        for (auto& sample : samples)
            sample = (signed short)(cue->nextSample() * 32768);        
        m_speakerBuffer.loadFromSamples(&samples[0], samples.size(), 1, 44100);
        m_speakerSound.setBuffer(m_speakerBuffer);
        m_speakerSound.setVolume(100);
        m_speakerSound.play();
    }

    //--------------------------------------------------------------------------
    // GUI Functions
    //--------------------------------------------------------------------------

    /// Updates the top bar
    void updateTopBar() {
        if (ImGui::Button(ICON_FA_PLAY) || Input::getKeyDown(Key::Space)) 
            playSelected();
        ImGui::SameLine();
        if (ImGui::Button(ICON_FA_VOLUME_UP)) {
            playSpeaker();
        }
        ImGui::SameLine();
        if (ImGui::Button(ICON_FA_STOP)) {
            tact::stopAll();
                m_speakerSound.stop();
        }
        ImGui::SameLine();
        if (ImGui::Button(ICON_FA_PASTE))
            exportCpp();
        ImGui::SameLine();
        if (ImGui::Button(ICON_FA_FILE_AUDIO)) {
            std::string filePath = "syntacts_" + str(rand() % 900000 + 100000) + ".wav";
            tact::save(buildCue(), filePath);
        }
        ImGui::SameLine();
        if (ImGui::Button(ICON_FA_SYNC_ALT)) 
            connect(m_device);
        ImGui::SameLine();
        auto devs = tact::getAvailableDevices();
        ImGui::PushItemWidth(200);
        if (ImGui::BeginCombo("##Devices", m_device.name.c_str())) // The second parameter is the label previewed before opening the combo.
        {
            for (int i = 0; i < devs.size(); i++)
            {
                bool is_selected = (m_device.index == devs[i].index);
                if (ImGui::Selectable(devs[i].name.c_str(), is_selected))
                    connect(devs[i]);
                if (is_selected)
                    ImGui::SetItemDefaultFocus();   // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
            }
            ImGui::EndCombo();
        }
        ImGui::PopItemWidth();

    }

    // Updates the help/info icons and contents
    void updateHelpInfo() {
        ImGui::SameLine(ImGui::GetWindowWidth()-60);
        if (ImGui::Button(ICON_FA_QUESTION)) 
            ImGui::OpenPopup("Help");
        if (ImGui::BeginPopupModal("Help", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove))
        {
            ImGui::BulletText("Buttons");
                ImGui::Indent();
                ImGui::Text(ICON_FA_PLAY       " Plays the current cue on all selected ASIO channels." );
                ImGui::Text(ICON_FA_VOLUME_UP  " Plays the current cue on the default audio output device." );
                ImGui::Text(ICON_FA_STOP       " Stops all cues currently playing on the ASIO and audio devices." );
                ImGui::Text(ICON_FA_PASTE      " Copies the C++ code required to generate the current cue to the clipboard.");
                ImGui::Text(ICON_FA_FILE_AUDIO " Saves the current cue to a .wav file.");
                ImGui::Text(ICON_FA_SYNC_ALT   " Reconnects the current device and refreshes the list of available ASIO devices.");
                ImGui::Unindent();
            ImGui::Spacing();
            ImGui::BulletText("Use the device dropdown list to select your ASIO device. If no devices are listed, \ntry reconnecting and pressing " ICON_FA_SYNC_ALT ". Alternately, install ASIO4ALL to enable devices\nwhich do not proivde an ASIO driver.");
            ImGui::Spacing();
            ImGui::BulletText("Use the channel buttons to play a channel individually, or toggle multiple with the\ncheckboxes to play at once with " ICON_FA_PLAY ".");
            ImGui::Spacing();
            ImGui::BulletText("Build cues by changing parameters in the Carrier, Modulation, and Envelope dialogs.\nYou can change values by dragging or double clicking the numeric entries.");
            ImGui::Spacing();
            if (ImGui::Button("Got It!"))
                ImGui::CloseCurrentPopup(); 
            ImGui::EndPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button(ICON_FA_INFO)) 
            ImGui::OpenPopup("Syntacts v1.0.0");
        if (ImGui::BeginPopupModal("Syntacts v1.0.0", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove))
        {
            ImGui::Text("Evan Pezent"); ImGui::SameLine(150);
            if (ImGui::Button(ICON_FA_ENVELOPE))
                ShellExecuteA(0, 0, "mailto:epezent@rice.edu?subject=Syntacts", 0, 0 , 5);
            ImGui::SameLine();
            if (ImGui::Button(ICON_FA_HOME))
                ShellExecuteA(0, 0, "http://www.evanpezent.com", 0, 0 , 5);
            ImGui::Text("Brandon Cambio"); ImGui::SameLine(150);
            if (ImGui::Button(ICON_FA_ENVELOPE))
                ShellExecuteA(0, 0, "mailto:Brandon.T.Cambio@rice.edu?subject=Syntacts", 0, 0 , 5);
            if (ImGui::Button("Close"))
                ImGui::CloseCurrentPopup(); 
            ImGui::EndPopup();
        }
    }

    /// Updates the channel selection checkboxes
    void updateChannels() {
        ImGui::PushStyleColor(ImGuiCol_Border, Color::Transparent);
        ImGui::BeginChild("Channels", ImVec2(0, ImGui::GetFrameHeightWithSpacing()+27), true, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoBackground);
        for (int i = 0; i < m_device.maxChannels; ++i)
        {
            auto label = str("Ch", i + 1);
            if (ImGui::Button(label.c_str()) || Input::getKeyDown((Key)((int)Key::Num1 + i)))
                playCh(i);
            ImGui::SameLine();
            ImGui::Checkbox(str("##", i).c_str(), &m_checkBoxes[i]);
            ImGui::SameLine();
        }
        ImGui::EndChild();
        ImGui::PopStyleColor(); 
    }

    /// Updates the carrier section
    void updateCarrier() {
        ImGui::Text("Carrier");
        ImGui::DragInt("Frequency##C", &m_carFreq, 0.5f, 0, 500, "%i Hz");
        ImGui::RadioButton("Sine##C", &m_carType, OscType::Sine);   
        if (m_modMode != ModMode::FM) {
            ImGui::SameLine();
            ImGui::RadioButton("Square##C",   &m_carType, OscType::Square);   ImGui::SameLine();
            ImGui::RadioButton("Saw##C",      &m_carType, OscType::Saw);      ImGui::SameLine();
            ImGui::RadioButton("Triangle##C", &m_carType, OscType::Triangle);
        }
        else {
            m_carType = OscType::Sine;
        }
    }

    /// Updates the modulation section
    void updateModulation() {
        ImGui::Text("Modulation"); ImGui::SameLine();
        ImGui::RadioButton("Off", &m_modMode, ModMode::Off); ImGui::SameLine();
        ImGui::RadioButton("AM",  &m_modMode, ModMode::AM);  ImGui::SameLine();
        ImGui::RadioButton("FM",  &m_modMode, ModMode::FM);
        if (m_modMode == ModMode::Off) {
            ImGui::Dummy(ImVec2(0.0f, 21));
            ImGui::Dummy(ImVec2(0.0f, 21));
        }
        else if (m_modMode == ModMode::AM) {
            ImGui::DragInt("Frequency##M", &m_modFreq, 0.5f,  0, 500, "%i Hz");
            ImGui::RadioButton("Sine##M",     &m_modType, OscType::Sine);      ImGui::SameLine();
            ImGui::RadioButton("Square##M",   &m_modType, OscType::Square);    ImGui::SameLine();
            ImGui::RadioButton("Saw##M",      &m_modType, OscType::Saw);       ImGui::SameLine();
            ImGui::RadioButton("Triangle##M", &m_modType, OscType::Triangle);  ImGui::SameLine();
            ImGui::Dummy(ImVec2(0.0f, 21));
        }
        else if (m_modMode == ModMode::FM) {            
            ImGui::DragInt("Frequency##M", &m_modFreq, 0.5f,  0, 250, "%i Hz");
            ImGui::DragFloat("Modulation Index", &m_modIdx, 0.01f, 0.0f, 12.0f);
        }
    }

    /// Updates the Envelope section
    void updateEnvelope() {
        ImGui::Text("Envelope");
        ImGui::SameLine();
        ImGui::RadioButton("Basic",&m_envMode, EnvMode::Basic); ImGui::SameLine();
        ImGui::RadioButton("ASR",&m_envMode,   EnvMode::ASR);   ImGui::SameLine();
        ImGui::RadioButton("ADSR",&m_envMode,  EnvMode::ADSR);
        int numTweens;
        int skip;
        if (m_envMode == EnvMode::Basic) {
            numTweens = 1;
            skip = 0;
            ImGui::DragFloat("Amplitude", m_amps, 0.005f, 0.0f, 1.0f);
            ImGui::DragInt("Duration", &m_duration, 0.5f, 0, 5000, "%i ms");
        }
        if (m_envMode == EnvMode::ASR) {
            numTweens = 3;
            skip = 1;
            ImGui::DragFloat("Amplitude", m_amps, 0.005f, 0.0f, 1.0f);
            ImGui::DragInt3("ASR##",m_asr, 0.5f, 0, 1000, "%i ms");
            m_adsr[0] = m_asr[0];
            m_adsr[2] = m_asr[1];
            m_adsr[3] = m_asr[2];
            m_duration = m_asr[0] + m_asr[1] + m_asr[2];
        }
        else if (m_envMode == EnvMode::ADSR) {
            numTweens = 4;
            skip = 2;
            ImGui::DragFloat2("Amplitudes", m_amps, 0.005f, 0.0f, 1.0f);
            ImGui::DragInt4("ADSR##",m_adsr, 0.5f, 0, 1000, "%i ms");
            m_asr[0] = m_adsr[0];
            m_asr[1] = m_adsr[2];
            m_asr[2] = m_adsr[3];
            m_duration = m_adsr[0] + m_adsr[1] + m_adsr[2] + m_adsr[3];
        }
        ImGui::BeginGroup();
        ImGui::PushID("Tweens");
        ImGui::PushMultiItemsWidths(numTweens);
        ImGuiContext& g = *GImGui;
        for (int i = 0; i < numTweens; ++i) {
            ImGui::PushID(i);
            
            if (i == skip)
                ImGui::Dummy(ImGui::GetItemRectSize());
            else
                ImGui::Combo("##i", &m_tweenModes[i], &g_tweenStrings[0], (int)g_tweenStrings.size());
            ImGui::SameLine(0, g.Style.ItemInnerSpacing.x);
            ImGui::PopID();
            ImGui::PopItemWidth();
        }
        ImGui::PopID();
        ImGui::EndGroup();

    }

    /// Updates the waveform plot
    void updatePlot() {
        m_cuePlot.clear();
        auto cue = buildCue();
        auto nPoints = cue->sampleCount();
        m_cuePlot.resize(nPoints);    
        for (auto& point : m_cuePlot)
            point = cue->nextSample();
        auto env = buildEnv();
        m_envPlot.resize(nPoints);
        for (auto& point : m_envPlot)
            point = env->nextSample();
        ImGui::PushItemWidth(m_windowSize.x - 25.0f);
        ImGui::PushStyleColor(ImGuiCol_PlotLines, hexCode("cf94c2"));
        ImGui::PushStyleColor(ImGuiCol_PlotLinesHovered, hexCode("cf94c2"));
        auto title = str(m_duration, "ms /", m_cuePlot.size(), " samples");
        ImGui::PlotLines2("", &m_cuePlot[0], &m_envPlot[0], nPoints,  0, title.c_str(), -1.0f, 1.0f, ImVec2(0, ImGui::GetContentRegionAvail().y));
        ImGui::PopStyleColor();
        ImGui::PopStyleColor();
        ImGui::PopItemWidth();
    }
    

    //--------------------------------------------------------------------------
    // Update
    //--------------------------------------------------------------------------

    /// Called once per frame
    void update() override {    
        // Update the current size of the Window
        m_windowSize = Engine::getWindowSize();
        // Update the GUI
        ImGui::SetNextWindowPos(Vector2f(5,5), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(m_windowSize.x - 10.0f, m_windowSize.y - 10.0f), ImGuiCond_Always);
        ImGui::Begin("Syntacts", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);   
        updateTopBar();
        updateHelpInfo();
        ImGui::Separator();
        updateChannels();       
        ImGui::Separator();
        updateCarrier();
        ImGui::Separator();
        updateModulation();
        ImGui::Separator();
        updateEnvelope();
        ImGui::Separator();
        updatePlot();
        ImGui::End();
    }

private:

    //--------------------------------------------------------------------------
    // Member Data
    //--------------------------------------------------------------------------
     
    tact::DeviceInfo m_device;

    int   m_carFreq = 175;
    int   m_carType = 0;

    int   m_modMode = ModMode::AM;
    int   m_modFreq = 25;
    int   m_modType = OscType::Sine;
    float m_modIdx  = 2.0f;

    int   m_envMode  = EnvMode::ASR;
    float m_amps[2] = {0.75f, 0.5f};
    int   m_duration = 150;
    int   m_asr[3]   = {50, 75, 25};    
    int   m_adsr[4]  = {50,50,75,25};
    std::vector<int> m_tweenModes = {0,0,0,0};

    Vector2u           m_windowSize;
    std::deque<bool>   m_checkBoxes;
    std::vector<float> m_cuePlot;
    std::vector<float> m_envPlot;

    SoundBuffer m_speakerBuffer;
    Sound       m_speakerSound;
};


int main(int argc, char const *argv[])
{
    tact::initialize();
    Engine::init(720, 540, WindowStyle::Default);
    Engine::window->setTitle("Syntacts");
    Engine::makeRoot<SyntactsGui>();
    Engine::run();  
    tact::finalize();
    return 0;
}
