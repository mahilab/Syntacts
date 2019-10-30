#define CARNOT_NO_CONSOLE
// #define CARNOT_USE_DISCRETE_GPU

#include "gui-detail.hpp"

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

const std::vector<std::shared_ptr<tact::Tween::Functor>> g_tweenFuncs = {
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

//==============================================================================
// GUI GAMEOBJECT
//==============================================================================

class SyntactsGui : public GameObject {
public:

    //--------------------------------------------------------------------------
    // Enums
    //--------------------------------------------------------------------------

    enum OscType : int { Sine = 0, Square = 1, Saw = 2, Triangle = 3, Chirp = 4 };
    enum ModMode : int { Off = 0, AM = 1, FM = 2 };
    enum EnvMode : int { Basic = 0, ASR = 1, ADSR = 2, PulseTrain = 3, Custom = 4 };

    //--------------------------------------------------------------------------
    // Generic Functions
    //--------------------------------------------------------------------------

    /// Constructor
    SyntactsGui() : m_checkBoxes(2,false) { 
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

    /// Selects/deselects all available channels
    void selectAllChannels(bool select) {
        for (int i = 0; i < m_checkBoxes.size(); i++)
            m_checkBoxes[i] = select;
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

        if (m_envMode == EnvMode::Basic)
            code += "auto env = std::make_shared<tact::Envelope>(" + str(m_duration/1000.0f) + ", " + str(m_amps[0]) + ");\n";
        else if (m_envMode == EnvMode::ASR)
            code += "auto env = std::make_shared<tact::ASR>(" + str(m_asr[0]/1000.0f) + ", " + str(m_asr[1]/1000.0f) + ", " + str(m_asr[2]/1000.0f) + ", " + str(m_amps[0]) + ", tact::Tween::" + g_tweenStrings[m_tweenModes[0]] + ", tact::Tween::" + g_tweenStrings[m_tweenModes[2]] + ");\n";
        else if (m_envMode == EnvMode::ADSR)
            code += "auto env = std::make_shared<tact::ADSR>(" + str(m_adsr[0]/1000.0f) + ", " + str(m_adsr[1]/1000.0f) + ", " + str(m_adsr[2]/1000.0f) + ", " + str(m_adsr[3]/1000.0f) + ", " + str(m_amps[0]) + ", " + str(m_amps[1]) + ", tact::Tween::" + g_tweenStrings[m_tweenModes[0]] + ", tact::Tween::" + g_tweenStrings[m_tweenModes[1]] + ", tact::Tween::" + g_tweenStrings[m_tweenModes[3]] + ");\n";
        else if (m_envMode == EnvMode::PulseTrain) {
            code += "auto pulse = std::make_shared<tact::PulseTrain>(" + str(m_pwmValues[0]) + ", "  + str(m_pwmValues[1] / 100.0f) + ");\n";
            code += "auto env = std::make_shared<tact::Envelope>(" + str(m_duration/1000.0f) + ", " + str(m_amps[0]) + ", pulse);\n";
        }
        
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
        std::vector<signed short> samples(cue->sampleCount(tact::DEFAULT_SAMPLE_RATE)); 
        float t = 0;      
        for (auto& sample : samples) {
            sample = (signed short)(cue->sample(t) * 32768);    
            t += tact::DEFAULT_SAMPLE_LENGTH;
        }    
        m_speakerBuffer.loadFromSamples(&samples[0], samples.size(), 1, tact::DEFAULT_SAMPLE_RATE);
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
            std::string filePath = "syntacts_" + str(Random::range(0,std::numeric_limits<int>::max())) + ".wav";
            tact::exportToWave(buildCue(), filePath);
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
        // detail dev info
        ImGui::SameLine();
        if (ImGui::Button("...")) {
            ImGui::OpenPopup("Device Details");
        }
        bool modalOpen = true;
        if (ImGui::BeginPopupModal("Device Details", &modalOpen, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove))
        {
            ImGui::Text("ID"); ImGui::SameLine(40);
            ImGui::Text("Name");  ImGui::SameLine(500);
            ImGui::Text("API");  
            

            for (auto& d : devs) {
                auto api = d.api;
                if (api.find("Windows ") == 0)
                    api.erase(0, 8);
                if (d.defaultApi)
                    api += "*";
                std::string id = str(d.index);
                if (d.default)
                    id += "*";
                ImGui::Separator();

                ImGui::Text(id.c_str());   ImGui::SameLine(40);
                ImGui::Text(d.name.c_str()); ImGui::SameLine(500);
                ImGui::Text(api.c_str()); 
            }

            // if (ImGui::Button("Close")) {
            //     ImGui::CloseCurrentPopup();
            // } 
            ImGui::EndPopup();
        }
    }

    // Updates the help/info icons and contents
    void updateHelpInfo() {
        bool modalOpen = true;
        ImGui::SameLine(ImGui::GetWindowWidth()-60);
        if (ImGui::Button(ICON_FA_QUESTION)) 
            ImGui::OpenPopup("Help");
        if (ImGui::BeginPopupModal("Help", &modalOpen, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove))
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
            ImGui::EndPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button(ICON_FA_INFO)) 
            ImGui::OpenPopup("Syntacts v1.0.0");
        if (ImGui::BeginPopupModal("Syntacts v1.0.0", &modalOpen, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove))
        {
            ImGui::Text("Evan Pezent"); ImGui::SameLine(150);
            if (ImGui::Button(ICON_FA_ENVELOPE))
                openEmail("epezent@rice.edu","Syntacts");
            ImGui::SameLine();
            if (ImGui::Button(ICON_FA_HOME))
                openUrl("http://www.evanpezent.com");
            ImGui::Text("Brandon Cambio"); ImGui::SameLine(150);
            if (ImGui::Button(ICON_FA_ENVELOPE))
                openEmail("Brandon.T.Cambio@rice.edu","Syntacts");
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
        if (ImGui::Button("Select All"))
            selectAllChannels(true);
        ImGui::SameLine();
        if(ImGui::Button("Deselect All"))
            selectAllChannels(false);
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
            ImGui::RadioButton("Triangle##C", &m_carType, OscType::Triangle); ImGui::SameLine();
            ImGui::RadioButton("Chirp##C",    &m_carType, OscType::Chirp);
            if (m_carType == OscType::Chirp) {
                ImGui::DragFloat("Chirpyness", &m_chirp, 1.0f, 0.0f, 500.0f);
            }
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
    }

    /// Updates the waveform plot
    void updatePlot() {
        m_cuePlot.clear();
        auto cue = buildCue();
        auto nPoints = cue->sampleCount(10000);
        m_cuePlot.resize(nPoints);    
        float t = 0;
        for (auto& point : m_cuePlot) {
            point = cue->sample(t);
            t += 0.0001f;
        }
        auto env = buildEnv();
        m_envPlot.resize(nPoints);
        t = 0;
        for (auto& point : m_envPlot) {
            point = env->sample(t);
            t += 0.0001f;
        }
        ImGui::PushItemWidth(m_windowSize.x - 25.0f - 200);
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
        float cueListWidth = 200;
        // Update the GUI
        ImGui::SetNextWindowPos(Vector2f(5+5+cueListWidth,5), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(m_windowSize.x - 10.0f - cueListWidth, m_windowSize.y - 10.0f), ImGuiCond_Always);
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
        // ImGui::ShowBezierDemo();
        updatePlot();
        ImGui::End();
        ImGui::SetNextWindowPos(Vector2f(5,5), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(cueListWidth, m_windowSize.y - 10.0f), ImGuiCond_Always);
        ImGui::Begin("Cues", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
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

    float m_chirp = 200;

    bool m_deviceDetailsOpen = false;

    int   m_envMode  = EnvMode::ASR;
    std::vector<float> m_amps = {1.0f, 0.5f};
    int   m_duration = 150;
    std::vector<int> m_pwmValues = {10, 25};
    std::vector<int> m_asr   = {50, 75, 25};    
    std::vector<int> m_adsr  = {50,50,75,25};
    std::vector<int> m_tweenModes = {0,0,0,0};

    Vector2u           m_windowSize;
    std::deque<bool>   m_checkBoxes;
    std::vector<float> m_cuePlot;
    std::vector<float> m_envPlot;

    SoundBuffer m_speakerBuffer;
    Sound       m_speakerSound;
};

//==============================================================================
// MAIN
//==============================================================================

int main(int argc, char const *argv[])
{
    tact::initialize();
    Engine::init(960, 540, WindowStyle::Default);
    Engine::window->setTitle("Syntacts");
    Engine::makeRoot<SyntactsGui>();
    Engine::run();  
    tact::finalize();
    return 0;
}
