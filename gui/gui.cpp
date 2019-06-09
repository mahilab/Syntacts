#define CARNOT_NO_CONSOLE
// #define CARNOT_USE_DISCRETE_GPU

#include <carnot>
#include <Syntacts/Syntacts.hpp>
#include <deque>
#include <functional>

using namespace carnot;

//==============================================================================
// HELPER FUNCTIONS
//==============================================================================

// renders an ImGui tooltip
void tooltip(const std::string& tip) {
    if (ImGui::IsItemHovered()) 
        ImGui::SetTooltip(tip.c_str());
}

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
        numCh = syntacts::getCurrentDevice().maxChannels;
        checkBoxes = std::deque<bool>(numCh, false);
        if (checkBoxes.size()>0)
            checkBoxes[0] = true;
        if (checkBoxes.size()>1)
            checkBoxes[1] = true;
    }

    /// Restarts Syntacts (useful if device connected mid usage)
    void refresh() {
        syntacts::finalize();
        syntacts::initialize();
        rechannel();
    }

    /// Builds a Syntacts Cue from the user's UI configuration
    Ptr<syntacts::Cue> buildCue() {
        auto env = make<syntacts::ASR>(a/1000.0f, s/1000.0f, r/1000.f, amp);
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
        if (a == 0 && r == 0)
            code += "auto env = std::make_shared<syntacts::Envelope>(" + str(s/1000.0f) + ", " + str(amp) + ");\n";
        else  
            code += "auto env = std::make_shared<syntacts::ASR>(" + str(a/1000.0f) + ", " + str(s/1000.0f) + ", " + str(r/1000.0f) + ", " + str(amp) + ");\n";
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

    /// Plays the Cue over the user's speakers
    void playSpeaker() {
        auto cue = buildCue();
        std::vector<signed short> samples(cue->sampleCount());       
        for (auto& sample : samples)
            sample = (signed short)(cue->nextSample() * 32768);        
        speakerBuffer.loadFromSamples(&samples[0], samples.size(), 1, 44100);
        speakerSound.setBuffer(speakerBuffer);
        speakerSound.play();
    }

    /// Overrides GameObject::update to draw GUI and get user inputs
    void update() override {    
        ImGui::SetNextWindowPos(Vector2f(5,5), ImGuiCond_Always);
        auto currSize = Engine::window->getSize();
        ImGui::SetNextWindowSize(Vector2f(currSize.x-10,currSize.y-10), ImGuiCond_Always);
        ImGui::Begin("Syntacts", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);   
        //====================================================================
        if (ImGui::Button(ICON_FA_PLAY) || Input::getKeyDown(Key::Space)) {
            for (std::size_t i = 0; i < numCh; ++i) {
                if (checkBoxes[i])
                    playSyntacts(i);
            }
        }
        ImGui::SameLine();
        if (ImGui::Button(ICON_FA_STOP))
            syntacts::stopAll();
        ImGui::SameLine();
        if (ImGui::Button(ICON_FA_VOLUME_UP))
            playSpeaker();
        ImGui::SameLine();
        if (ImGui::Button(ICON_FA_FILE_EXPORT))
            exportCpp();
        //====================================================================
        ImGui::SameLine();
        if (ImGui::Button(ICON_FA_SYNC_ALT)) 
            refresh();
        ImGui::SameLine();
        auto devName = syntacts::getCurrentDevice().name;
        if (devName != "none")
            ImGui::Text(devName.c_str());
        else {
            ImGui::TextColored(Yellows::Gold, "ASIO Device Not Found");
            tooltip("Connect an ASIO sound device and then press " ICON_FA_SYNC_ALT ".\nAlternately, install the ASIO4ALL driver to \nenable your non-ASIO device.");
        }
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
        for (int i = 0; i < numCh; ++i)
        {
            auto label = str("Ch",i+1);
            if (ImGui::Button(label.c_str()) || Input::getKeyDown((Key)((int)Key::Num1 + i)))
                playSyntacts(i);
            ImGui::SameLine();
            ImGui::Checkbox(str("##",i).c_str(), &checkBoxes[i]);

            if (((i+1) % numCh) != 0)
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
        ImGui::DragFloat("Amplitude", &amp, 0.005f, 0.0f, 1.0f);
        ImGui::DragInt("Attack", &a, 0.5f,  0, 1000, "%i ms");
        ImGui::DragInt("Sustain", &s, 0.5f,  0, 1000, "%i ms");
        ImGui::DragInt("Release", &r, 0.5f,  0, 1000, "%i ms");
        ImGui::Separator();
        //====================================================================
        plot.clear();
        int durMs = a+s+r;
        float durS = (float)durMs / 1000.0f;

        auto cue = buildCue();
        plot.resize(cue->sampleCount());       
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
     
    int   numCh      = 2;
    float amp       = 0.75f;
    int   freq      = 175;
    int   freq_wave = 0;
    int   modMode     = 1;
    int   mod       = 25;
    int   mod_wave  = 0;
    int   a         = 50;
    int   s         = 75;
    int   r         = 25;
    float fmIdx      = 2;

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
