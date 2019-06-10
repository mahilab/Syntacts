#define CARNOT_NO_CONSOLE
// #define CARNOT_USE_DISCRETE_GPU

#include <carnot>
#include <Syntacts/Syntacts.hpp>
#include <deque>
#include <functional>
#include <chrono>
#include <ctime>  
#include <string>

using namespace carnot;

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
        auto devices = *(std::vector<syntacts::DeviceInfo>*)data;
        std::vector<const char*> strs(devices.size());
        for (std::size_t i = 0; i < devices.size(); ++i)
            strs[i] = devices[i].name.c_str();
        out_str = &strs[idx];
    } 
};


//==============================================================================
// DEMO GAMEOBJECT
//==============================================================================

class SyntactsGui : public GameObject {
public:

    /// Constructor
    SyntactsGui() : plot(), checkBoxes(2,false) { 
        plot.reserve(44100*10);
        rechannel();
    }

    /// Gets number of channels from current Syntacts device and resizes accordingly
    void rechannel() {
        curDev = syntacts::getCurrentDevice();
        checkBoxes = std::deque<bool>(curDev.maxChannels, false);
        if (checkBoxes.size()>0)
            checkBoxes[0] = true;
        if (checkBoxes.size()>1)
            checkBoxes[1] = true;
    }

    /// Restarts Syntacts (useful if device connected mid usage)
    void reconnect(syntacts::DeviceInfo dev) {
        curDev = dev;
        syntacts::finalize();
        syntacts::initialize(dev.index, dev.maxChannels);
        rechannel();
    }

    /// Builds a Syntacts Cue from the user's UI configuration
    Ptr<syntacts::Cue> buildCue() {
        Ptr<syntacts::Envelope> env;
        if (envMode == 0)
            env = make<syntacts::Envelope>(dur/1000.0f, amp[0]);    
        else if (envMode == 1)
            env = make<syntacts::ASR>(asr[0]/1000.0f, asr[1]/1000.0f, asr[2]/1000.f, amp[0]);
        else if (envMode == 2) 
            env = make<syntacts::ADSR>(adsr[0]/1000.0f, adsr[1]/1000.0f, adsr[2]/1000.0f, adsr[3]/1000.f, amp[0], amp[1]);
        Ptr<syntacts::Oscillator> osc;
        if (modMode != 2) {
            if (freq_wave == 0)
                osc = make<syntacts::SineWave>((float)freq);
            else if (freq_wave == 1)
                osc = make<syntacts::SquareWave>((float)freq);
            else if (freq_wave == 2)
                osc = make<syntacts::SawWave>((float)freq);
            else if (freq_wave == 3)
                osc = make<syntacts::TriWave>((float)freq);   
        }       
        else {
            osc = make<syntacts::SineWaveFM>((float)freq, (float)mod, fmIdx);
        }
        Ptr<syntacts::Cue> cue;
        if (modMode == 1 && mod > 0) {
            Ptr<syntacts::Oscillator> modOsc;
            if (mod_wave == 0)
                modOsc = make<syntacts::SineWave>((float)mod);
            else if (mod_wave == 1)
                modOsc = make<syntacts::SquareWave>((float)mod);
            else if (mod_wave == 2)
                modOsc = make<syntacts::SawWave>((float)mod);
            else if (mod_wave == 3)
                modOsc = make<syntacts::TriWave>((float)mod);  
            cue = make<syntacts::Cue>(osc, modOsc, env);
        }
        else
            cue = make<syntacts::Cue>(osc, env);
        return cue;
    }

    /// Exports current UI configuration to Syntacts C++
    void exportCpp() {
        std::string code;
        code += "auto osc = std::make_shared<syntacts::";
        if (modMode != 2) {
            code += freq_wave == 0 ? "SineWave" : freq_wave == 1 ? "SquareWave" : freq_wave == 2 ? "SawWave" : freq_wave == 3 ? "TriWave" : "";
            code += ">(" + str((float)freq) + ");\n";
        }
        else
            code += "SineWaveFM>(" + str((float)freq) + ", " + str((float)mod) + ", " + str(fmIdx) + ");\n";     

        if (envMode == 0)
            code += "auto env = std::make_shared<syntacts::Envelope>(" + str(dur/1000.0f) + ", " + str(amp[0]) + ");\n";
        else if (envMode == 1)
            code += "auto env = std::make_shared<syntacts::ASR>(" + str(asr[0]/1000.0f) + ", " + str(asr[1]/1000.0f) + ", " + str(asr[2]/1000.0f) + ", " + str(amp[0]) + ");\n";
        else if (envMode == 2)
            code += "auto env = std::make_shared<syntacts::ADSR>(" + str(adsr[0]/1000.0f) + ", " + str(adsr[1]/1000.0f) + ", " + str(adsr[2]/1000.0f) + ", " + str(adsr[3]/1000.0f) + ", " + str(amp[0]) + ", " + str(amp[1]) + ");\n";

        
        if (modMode == 1 && mod > 0) {
            code += "auto mod = std::make_shared<syntacts::";
            code += mod_wave == 0 ? "SineWave" : mod_wave == 1 ? "SquareWave" : mod_wave == 2 ? "SawWave" : mod_wave == 3 ? "TriWave" : "";
            code += ">(" + str((float)mod) + ");\n";
            code += "auto cue = std::make_shared<syntacts::Cue>(osc, mod, env);\n";
        }
        else {
            code += "auto cue = std::make_shared<syntacts::Cue>(osc, env);\n";
        }
        Clipboard::setString(code);
    }


    /// Creats and plays the user's cue
    void playSyntacts(std::size_t ch) {
        syntacts::play((int)ch, buildCue());
    }

    void playSelected() {
        for (std::size_t i = 0; i < curDev.maxChannels; ++i) {
            if (checkBoxes[i])
                playSyntacts(i);
        }
    }



    /// Plays the Cue over the user's speakers
    void playSpeaker() {
        auto cue = buildCue();
        std::vector<signed short> samples(cue->sampleCount());       
        for (auto& sample : samples)
            sample = (signed short)(cue->nextSample() * 32768);        
        speakerBuffer.loadFromSamples(&samples[0], samples.size(), 1, 44100);
        speakerSound.setBuffer(speakerBuffer);
        speakerSound.setVolume(100);
        speakerSound.play();
    }

    /// Overrides GameObject::update to draw GUI and get user inputs
    void update() override {    
        ImGui::SetNextWindowPos(Vector2f(5,5), ImGuiCond_Always);
        auto currSize = Engine::getWindowSize();
        ImGui::SetNextWindowSize((Vector2f)Vector2u(currSize.x - 10, currSize.y - 10), ImGuiCond_Always);
        ImGui::Begin("Syntacts", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);   
        //====================================================================
        if (ImGui::Button(ICON_FA_PLAY) || Input::getKeyDown(Key::Space)) 
            playSelected();
        ImGui::SameLine();
        if (ImGui::Button(ICON_FA_VOLUME_UP)) {
            playSpeaker();
            playSelected();
        }
        ImGui::SameLine();
        if (ImGui::Button(ICON_FA_STOP)) {
            syntacts::stopAll();
            speakerSound.stop();
        }
        ImGui::SameLine();
        if (ImGui::Button(ICON_FA_PASTE))
            exportCpp();
        ImGui::SameLine();
        if (ImGui::Button(ICON_FA_FILE_AUDIO)) {
            std::string filePath = "syntacts_" + str(rand() % 900000 + 100000) + ".wav";
            syntacts::save(buildCue(), filePath);
        }
        //====================================================================
        ImGui::SameLine();
        // if (ImGui::Button(ICON_FA_SYNC_ALT)) 
        //     refresh();
        // ImGui::SameLine();
        // auto devName = syntacts::getCurrentDevice().name;
        // if (devName != "none")
        //     ImGui::Text(devName.c_str());
        // else {
        //     ImGui::TextColored(Yellows::Gold, "ASIO Device Not Found");
        //     tooltip("Connect an ASIO sound device and then press " ICON_FA_SYNC_ALT ".\nAlternately, install the ASIO4ALL driver to \nenable your non-ASIO device.");
        // }


        auto devs = syntacts::getAvailableDevices();
        ImGui::SameLine();
        ImGui::PushItemWidth(200);
        if (ImGui::BeginCombo("##Devices", curDev.name.c_str())) // The second parameter is the label previewed before opening the combo.
        {
            for (int i = 0; i < devs.size(); i++)
            {
                bool is_selected = (curDev.index == devs[i].index);
                if (ImGui::Selectable(devs[i].name.c_str(), is_selected))
                    reconnect(devs[i]);
                if (is_selected)
                    ImGui::SetItemDefaultFocus();   // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
            }
            ImGui::EndCombo();
        }
        ImGui::PopItemWidth();


        ImGui::SameLine(ImGui::GetWindowWidth()-30);
        //====================================================================
        if (ImGui::Button(ICON_FA_INFO)) {
            ImGui::OpenPopup("Syntacts v1.0");
        }
        if (ImGui::BeginPopupModal("Syntacts v1.0", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove))
        {
            if (ImGui::Button("Got It!"))
                ImGui::CloseCurrentPopup(); 
            ImGui::EndPopup();
        }

        //====================================================================
        ImGui::Separator();
        ImGui::PushStyleColor(ImGuiCol_Border, Color::Transparent);
        ImGui::BeginChild("Channels", ImVec2(0, ImGui::GetFrameHeightWithSpacing()+27), true, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoBackground);
        for (int i = 0; i < curDev.maxChannels; ++i)
        {
            auto label = str("Ch",i+1);
            if (ImGui::Button(label.c_str()) || Input::getKeyDown((Key)((int)Key::Num1 + i)))
                playSyntacts(i);
            ImGui::SameLine();
            ImGui::Checkbox(str("##",i).c_str(), &checkBoxes[i]);

            if (((i+1) % curDev.maxChannels) != 0)
                ImGui::SameLine();
        }
        ImGui::EndChild();
        ImGui::PopStyleColor();        
        ImGui::Separator();
        //====================================================================
        ImGui::Text("Carrier");
        ImGui::DragInt("Frequency##C", &freq, 0.5f, 0, 250, "%i Hz");
        ImGui::RadioButton("Sine##C", &freq_wave, 0);   
        if (modMode != 2) {
            ImGui::SameLine();
            ImGui::RadioButton("Square##C", &freq_wave, 1); ImGui::SameLine();
            ImGui::RadioButton("Saw##C", &freq_wave, 2);    ImGui::SameLine();
            ImGui::RadioButton("Triangle##C", &freq_wave, 3);
        }
        else {
            freq_wave = 0;
        }
        ImGui::Separator();
        //====================================================================
        ImGui::Text("Modulation"); ImGui::SameLine();
        ImGui::RadioButton("Off",&modMode, 0); ImGui::SameLine();
        ImGui::RadioButton("AM",&modMode, 1); ImGui::SameLine();
        ImGui::RadioButton("FM",&modMode, 2);
        if (modMode == 0) {
            ImGui::Dummy(ImVec2(0.0f, 21));
            ImGui::Dummy(ImVec2(0.0f, 21));
        }
        else if (modMode == 1) {
            ImGui::DragInt("Frequency##M", &mod, 0.5f,  0, 250, "%i Hz");
            ImGui::RadioButton("Sine##M", &mod_wave, 0);   ImGui::SameLine();
            ImGui::RadioButton("Square##M", &mod_wave, 1); ImGui::SameLine();
            ImGui::RadioButton("Saw##M", &mod_wave, 2);    ImGui::SameLine();
            ImGui::RadioButton("Triangle##M", &mod_wave, 3);  ImGui::SameLine();
            ImGui::Dummy(ImVec2(0.0f, 21));
        }
        else if (modMode == 2) {            
            ImGui::DragInt("Frequency##M", &mod, 0.5f,  0, 250, "%i Hz");
            ImGui::DragFloat("Modulation Index", &fmIdx, 0.01f, 0.0f, 12.0f);
        }
        ImGui::Separator();
        //====================================================================
        ImGui::Text("Envelope");
        ImGui::SameLine();
        ImGui::RadioButton("Basic",&envMode, 0); ImGui::SameLine();
        ImGui::RadioButton("ASR",&envMode,  1); ImGui::SameLine();
        ImGui::RadioButton("ADSR",&envMode, 2);
        if (envMode == 0) {
            ImGui::DragFloat("Amplitude", amp, 0.005f, 0.0f, 1.0f);
            ImGui::DragInt("Duration", &dur, 0.5f, 0, 5000, "%i ms");
        }
        if (envMode == 1) {
            ImGui::DragFloat("Amplitude", amp, 0.005f, 0.0f, 1.0f);
            ImGui::DragInt3("ASR##",asr, 0.5f, 0, 1000, "%i ms");
            adsr[0] = asr[0];
            adsr[2] = asr[1];
            adsr[3] = asr[2];
            dur = asr[0] + asr[1] + asr[2];
        }
        else if (envMode == 2) {
            ImGui::DragFloat2("Amplitudes", amp, 0.005f, 0.0f, 1.0f);
            ImGui::DragInt4("ADSR##",adsr, 0.5f, 0, 1000, "%i ms");
            asr[0] = adsr[0];
            asr[1] = adsr[2];
            asr[2] = adsr[3];
            dur = adsr[0] + adsr[1] + adsr[2] + adsr[3];
        }
        ImGui::Separator();
        //====================================================================
        plot.clear();

        auto cue = buildCue();
        plot.resize(cue->sampleCount());    
        float durMs = cue->sampleCount() * 1000.0f/44100.0f;  
        for (auto& point : plot)
            point = cue->nextSample();  

        ImGui::PushItemWidth(currSize.x - 25);
        ImGui::PushStyleColor(ImGuiCol_PlotLines, hexCode("cf94c2"));
        ImGui::PushStyleColor(ImGuiCol_PlotLinesHovered, hexCode("cf94c2"));
        auto title = str(durMs, "ms /", plot.size(), " samples");
        ImGui::PlotLines("", &plot[0], (int)plot.size(),  0, title.c_str(), -1.0f, 1.0f, ImVec2(0, ImGui::GetContentRegionAvail().y));
        ImGui::PopStyleColor();
        ImGui::PopStyleColor();
        ImGui::PopItemWidth();
        ImGui::End();
    }

private:
     
    float amp[2]      = {0.75f, 0.5f};
    int   freq      = 175;
    int   freq_wave = 0;
    int   modMode     = 1;
    int   mod       = 25;
    int   mod_wave  = 0;
    float fmIdx     = 2;
    int envMode     = 1;

    int curIdx = 0;
    syntacts::DeviceInfo curDev;

    int dur     = 150;
    int asr[3]  = {50, 75, 25};
    int adsr[4] = {50,50,75,25};

    std::deque<bool> checkBoxes;
    std::vector<float> plot;

    SoundBuffer speakerBuffer;
    Sound speakerSound;
};


int main(int argc, char const *argv[])
{
    syntacts::initialize();
    Engine::init(720, 540, WindowStyle::Default);
    Engine::window->setTitle("Syntacts");
    Engine::makeRoot<SyntactsGui>();
    Engine::run();  
    syntacts::finalize();
    return 0;
}
