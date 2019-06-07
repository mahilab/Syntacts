#define CARNOT_NO_CONSOLE
// #define CARNOT_USE_DISCRETE_GPU

#include <carnot>
#include <TactorFX/TactorFX.hpp>
#include <deque>
#include <functional>

#define NUM_CH 8
#define WIDTH 640
#define HEIGHT 480

using namespace carnot;

//==============================================================================
// WAVEFORM DRAWING FUNCTIONS
//==============================================================================

float sinWave(float f, float t) {
    return std::sin(2.0f * Math::PI * f *t);
}

float sqrWave(float f, float t) {
    return sinWave(f,t) > 0 ? 1.0f : -1.0f;
}

float sawWave(float f, float t) {
   return -2.0f / Math::PI * std::atan(std::cos(Math::PI * f * t) / std::sin(Math::PI * f * t));
}

float triWave(float f, float t) {
    return 2.0f / Math::PI * std::asin(std::sin(2.0f * Math::PI * f * t));
}

std::vector<std::function<float(float,float)>> waveFuncs = {sinWave, sqrWave, sawWave, triWave};

//==============================================================================
// DEMO GAMEOBJECT
//==============================================================================

class TfxDemo : public GameObject {
public:

    /// Constructor
    TfxDemo() : chCB(NUM_CH, false), plot() { 
        plot.reserve(44100*10);
        chCB[0] = true;
        tfx::initialize(NUM_CH);
    }

    /// Destructor
    ~TfxDemo() {
        tfx::finalize();
    }

    /// Creats and plays the user's cue
    void play(std::size_t ch) {
        Ptr<tfx::Oscillator> osc;
        if (freq_wave == 0)
            osc = make<tfx::SineWave>((float)freq, freq_amp);
        else if (freq_wave == 1)
            osc = make<tfx::SquareWave>((float)freq, freq_amp);
        else if (freq_wave == 2)
            osc = make<tfx::SawWave>((float)freq, freq_amp);
        else if (freq_wave == 3)
            osc = make<tfx::TriWave>((float)freq, freq_amp);            
        auto env = make<tfx::ASR>(a/1000.0f, s/1000.0f, r/1000.f);
        Ptr<tfx::Cue> cue;
        if (mod > 0) {
            Ptr<tfx::Oscillator> modOsc;
            if (mod_wave == 0)
                modOsc = make<tfx::SineWave>((float)mod, 1.0f);
            else if (mod_wave == 1)
                modOsc = make<tfx::SquareWave>((float)mod, mod_amp);
            else if (mod_wave == 2)
                modOsc = make<tfx::SawWave>((float)mod, mod_amp);
            else if (mod_wave == 3)
                modOsc = make<tfx::TriWave>((float)mod, mod_amp);  
            cue = make<tfx::Cue>(osc, modOsc, env);
        }
        else
            cue = make<tfx::Cue>(osc, env);
        tfx::playCue((int)ch, cue);
    }

    /// Overrides GameObject::update to draw GUI and get user inputs
    void update() override {    
        ImGui::SetNextWindowPos(Vector2f(5,5), ImGuiCond_Always);
        ImGui::SetNextWindowSize(Vector2f(WIDTH-10,HEIGHT-10), ImGuiCond_Always);
        ImGui::Begin("TactorFX", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);   
        //------------------------------------------------------------------
        if (ImGui::Button(ICON_FA_PLAY) || Input::getKeyDown(Key::Space)) {
            for (std::size_t i = 0; i < NUM_CH; ++i) {
                if (chCB[i])
                    play(i);
            }
        }
        ImGui::SameLine();
        if (ImGui::Button(ICON_FA_STOP))
            tfx::stopAllCues();
        ImGui::SameLine();
        if (ImGui::Button(ICON_FA_SYNC_ALT)) {
            tfx::finalize();
            tfx::initialize(NUM_CH);
        }
        ImGui::SameLine();
        ImGui::Text(tfx::getCurrentDevice().name.c_str());
        ImGui::Separator();
        //------------------------------------------------------------------
        for (std::size_t i = 0; i < NUM_CH; ++i)
        {
            auto label = str("Ch",i+1);
            if (ImGui::Button(label.c_str()) || Input::getKeyDown((Key)((int)Key::Num1 + i)))
                play(i);
            ImGui::SameLine();
            ImGui::Checkbox(str("##",i).c_str(), &chCB[i]);

            if (((i+1) % NUM_CH) != 0)
                ImGui::SameLine();
        }
        ImGui::Separator();
        //------------------------------------------------------------------
        ImGui::Text("Carrier");
        ImGui::RadioButton("Sine##C", &freq_wave, 0);   ImGui::SameLine();
        ImGui::RadioButton("Square##C", &freq_wave, 1); ImGui::SameLine();
        ImGui::RadioButton("Saw##C", &freq_wave, 2);    ImGui::SameLine();
        ImGui::RadioButton("Triangle##C", &freq_wave, 3);
        ImGui::DragInt("Frequency##C", &freq, 0.5f, 0, 250, "%i Hz");
        ImGui::DragFloat("Amplitude##C", &freq_amp, 0.005f, 0.0f, 1.0f);
        ImGui::Separator();
        //------------------------------------------------------------------
        ImGui::Text("Modulation");
        ImGui::RadioButton("Sine##M", &mod_wave, 0);   ImGui::SameLine();
        ImGui::RadioButton("Square##M", &mod_wave, 1); ImGui::SameLine();
        ImGui::RadioButton("Saw##M", &mod_wave, 2);    ImGui::SameLine();
        ImGui::RadioButton("Triangle##M", &mod_wave, 3);
        ImGui::DragInt("Frequency##M", &mod, 0.5f,  0, 250, "%i Hz");
        ImGui::DragFloat("Amplitude##M", &mod_amp, 0.005f, 0.0f, 1.0f);
        ImGui::Separator();
        //------------------------------------------------------------------
        ImGui::Text("Envelope");
        ImGui::DragInt("Attack", &a, 0.5f,  0, 1000, "%i ms");
        ImGui::DragInt("Sustain", &s, 0.5f,  0, 1000, "%i ms");
        ImGui::DragInt("Release", &r, 0.5f,  0, 1000, "%i ms");
        ImGui::Separator();
        //------------------------------------------------------------------
        plot.clear();
        int durMs = a+s+r;
        float durS = (float)durMs / 1000.0f;
        int samples = (int)(durS * 44100.0f);

        for (int i = 0; i < samples; ++i) {
            float t = durS / (float)samples * (float)i;
            float v = 1.0f;
            v *= freq_amp * waveFuncs[freq_wave]((float)freq, t);
            // modulation
            if (mod > 0)
                v*= mod_amp * waveFuncs[mod_wave]((float)mod, t);
            // envelope
            float env = 1.0f;
            if (t < (float)a/1000.0f)
                env = Math::interp(t,0.0f,(float)a/1000.0f,0.0f,1.0f);
            else if (t > (float)(a+s)/1000.0f)
                env = Math::interp(t, (float)(a+s)/1000.0f, (float)(a+s+r)/1000.0f, 1.0f, 0.0f);
            v *= env;

            plot.push_back(v);
        }
        ImGui::PushItemWidth(615);
        ImGui::PushStyleColor(ImGuiCol_PlotLines, hexCode("cf94c2"));
        ImGui::PushStyleColor(ImGuiCol_PlotLinesHovered, hexCode("cf94c2"));
        auto title = str(durMs, "ms /", samples, " samples");
        ImGui::PlotLines("", &plot[0], (int)plot.size(),  0, title.c_str(), -1.0f, 1.0f, ImVec2(0,104));
        ImGui::PopStyleColor();
        ImGui::PopStyleColor();
        ImGui::PopItemWidth();
        ImGui::End();
    }

private:

    int   freq = 175;
    float freq_amp  = 0.5f;
    int   freq_wave = 0;

    int   mod = 25;
    float mod_amp = 1.0f;
    int   mod_wave = 0;

    int a = 50;
    int s = 75;
    int r = 25;

    std::deque<bool> chCB;
    std::vector<float> plot;
};


int main(int argc, char const *argv[])
{
    /// Initialize and run Engine
    Engine::init(WIDTH, HEIGHT, WindowStyle::Close);
    Engine::window->setTitle("TactorFX");
    Engine::makeRoot<TfxDemo>();
    Engine::run();  
    return 0;
}
