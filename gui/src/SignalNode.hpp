#pragma once
#include <syntacts>
#include <carnot>
#include "helpers.hpp"
#include <deque>

///////////////////////////////////////////////////////////////////////////////

tact::Signal newSignal(std::type_index id) {
    if (id == typeid(tact::Zero))           return tact::Zero();
    if (id == typeid(tact::Time))           return tact::Time();
    if (id == typeid(tact::Scalar))         return tact::Scalar();
    if (id == typeid(tact::Ramp))           return tact::Ramp();
    if (id == typeid(tact::Noise))          return tact::Noise();
    if (id == typeid(tact::Expression))     return tact::Expression();
    if (id == typeid(tact::Sum))            return tact::Sum();
    if (id == typeid(tact::Product))        return tact::Product();
    if (id == typeid(tact::Sine))           return tact::Sine();
    if (id == typeid(tact::Square))         return tact::Square();
    if (id == typeid(tact::Saw))            return tact::Saw();
    if (id == typeid(tact::Triangle))       return tact::Triangle();
    if (id == typeid(tact::SineFM))         return tact::SineFM();
    if (id == typeid(tact::Chirp))          return tact::Chirp();
    if (id == typeid(tact::Pwm))            return tact::Pwm();
    if (id == typeid(tact::Envelope))       return tact::Envelope();
    if (id == typeid(tact::ASR))            return tact::ASR();
    if (id == typeid(tact::ADSR))           return tact::ADSR();
    if (id == typeid(tact::KeyedEnvelope))  return tact::KeyedEnvelope();
    if (id == typeid(tact::SignalEnvelope)) return tact::SignalEnvelope();
    print("Couldn't Make Signal!");
    return tact::Signal();
}

///////////////////////////////////////////////////////////////////////////////

void renderSignal(tact::Signal& sig);

///////////////////////////////////////////////////////////////////////////////

void renderOscillator(tact::Signal& sig) {
    auto cast = (tact::IOscillator*)sig.get();
    if (ImGui::RadioButton("Sine", sig.isType<tact::Sine>())) {
        sig = tact::Sine(cast->x);
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("Square",sig.isType<tact::Square>())) {
        sig = tact::Square(cast->x);
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("Saw",sig.isType<tact::Saw>())) {
        sig = tact::Saw(cast->x);
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("Triangle",sig.isType<tact::Triangle>())) {
        sig = tact::Triangle(cast->x);
    }
    cast = (tact::IOscillator*)sig.get();
    if (cast->x.isType<tact::Time>()) {
        float f = cast->x.scale / tact::TWO_PI;
        ImGui::DragFloat("Frequency", &f, 1, 0, 1000, "%.0f Hz");
        cast->x.scale = f * tact::TWO_PI;
    }
    else {
        ImGui::Indent();
        renderSignal(cast->x);
        ImGui::Unindent();
    }
}

void renderChirp(tact::Signal& sig) {
    auto cast = (tact::Chirp*)sig.get();
    if (cast->x.isType<tact::Time>()) {
        float f = cast->x.scale / tact::TWO_PI;
        ImGui::DragFloat("Frequency", &f, 1, 0, 1000, "%.0f Hz");
        cast->x.scale = f * tact::TWO_PI;
    }
    ImGui::DragFloat("Rate", &cast->rate, 1, 0, 0, "%.0f Hz/s");
}

void renderEnvelope(tact::Signal& sig) {
    auto cast = (tact::Envelope*)sig.get();
    ImGui::DragFloat("Duration", &cast->duration, 0.001f, 0, 1, "%0.3f s");
    ImGui::DragFloat("Amplitude", &cast->amplitude, 0.01f, 0, 1);
}

void renderExpression(tact::Signal& sig) {
    static char buffer[256];
    auto cast = (tact::Expression*)sig.get();
    strcpy(buffer, cast->getExpression().c_str());
    std::string expr = cast->getExpression();
    bool entered = ImGui::InputText("f(t)", buffer, 256, ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue);
    if (entered) {
        std::string newExpr(buffer);
        cast->setExpression(newExpr);
    }
}

void renderASR(tact::Signal& sig) {
    static std::vector<std::string> labels = {"Attack", "Sustain", "Release"};
    auto cast = (tact::ASR*)sig.get();
    auto it = cast->keys.begin(); it++;
    auto& a = *(it++);
    auto& s = *(it++);
    auto& r = *(it++);

    float attack = a.first;
    float sustain =  s.first - a.first;
    float release = r.first - s.first;

    bool changed = false;
    if (ImGui::DragFloat("Attack",  &attack,  0.001f, 0.0001f, 1, "%0.3f s"))
        changed = true;    
    if (ImGui::DragFloat("Sustain", &sustain, 0.001f, 0.0001f, 1, "%0.3f s"))
        changed = true;
    if (ImGui::DragFloat("Release", &release, 0.001f, 0.0001f, 1, "%0.3f s"))
        changed = true;

    if (changed)
        sig = tact::ASR(attack, sustain, release);
}

void renderNothing(tact::Signal& sig) { }

void renderSignal(tact::Signal& sig) {
    static std::unordered_map<std::type_index, std::function<void(tact::Signal&)>> renderFuncs = {
        {typeid(tact::Sine), renderOscillator},
        {typeid(tact::Square), renderOscillator},
        {typeid(tact::Saw), renderOscillator},
        {typeid(tact::Triangle), renderOscillator},
        {typeid(tact::Chirp), renderChirp},
        {typeid(tact::Envelope), renderEnvelope},
        {typeid(tact::ASR), renderASR},
        {typeid(tact::Expression), renderExpression},
        {typeid(tact::Noise), renderNothing},
        {typeid(tact::Time), renderNothing}

    };
    if (renderFuncs.count(sig.typeId()))
        renderFuncs[sig.typeId()](sig);
    else
        ImGui::Text("?");
}

///////////////////////////////////////////////////////////////////////////////

class SignalNode {
public:
    virtual void renderGui() = 0;
    virtual tact::Signal getOutput() = 0;
    virtual const std::string& name() = 0;
};

///////////////////////////////////////////////////////////////////////////////

class ProductNode : public SignalNode
{
public:
    ProductNode() 
    { }

    void renderGui()
    {
        for (std::size_t i = 0; i < m_signals.size(); ++i)
        {
            auto &header = helpers::signalName(m_signals[i]) + "###SignalHeader";
            ImGui::PushID(m_ids[i]);
            ImGui::PushStyleColor(ImGuiCol_Header, ImGui::GetStyle().Colors[ImGuiCol_TabActive]);
            if (ImGui::CollapsingHeader(header.c_str(), &m_closeHandles[i]))
            {
                ImGui::Indent();
                renderSignal(m_signals[i]);
                ImGui::Unindent();
            }
            ImGui::PopStyleColor();
            ImGui::PopID();
        }
        ImGui::PushStyleColor(ImGuiCol_Button, Color::Transparent);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, Color::Transparent);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Color::Transparent);
        helpers::beginSignalSink();
        ImGui::Button("##SignalDND", ImVec2(-1, 0));
        if (helpers::endSignalSink(m_dropped))
        {
            m_signals.emplace_back(newSignal(m_dropped));
            m_closeHandles.emplace_back(true);
            m_ids.push_back(m_nextId++);
        }
        ImGui::PopStyleColor(3);
        cleanStack();
    }

    tact::Signal getOutput() override {
        if (m_signals.size() > 0) {
            auto sig = m_signals[0];
            for (int i = 1; i < m_signals.size(); ++i)
                sig = sig * m_signals[i];
            return sig;
        }
        return tact::Signal();
    }

    void cleanStack() {
        std::vector<tact::Signal> newSignals;
        std::vector<int>          newIds;
        for (int i = 0; i < m_signals.size(); ++i) {
            if (m_closeHandles[i]) {
                newSignals.emplace_back(std::move(m_signals[i]));   
                newIds.push_back(m_ids[i]);
            }     
        }
        m_signals = std::move(newSignals);
        m_ids     = std::move(newIds);
        m_closeHandles = std::deque<bool>(m_signals.size(), true);
    }

    const std::string& name() override {
        static std::string n = "Product";
        return n;
    }

private:
    std::vector<tact::Signal> m_signals;
    std::deque<bool> m_closeHandles;
    std::vector<int> m_ids;
    int m_nextId;
    std::type_index m_dropped = typeid(tact::Zero);
};