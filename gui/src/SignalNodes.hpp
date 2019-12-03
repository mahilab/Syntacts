#ifndef IMGUI_DEFINE_MATH_OPERATORS
    #define IMGUI_DEFINE_MATH_OPERATORS
#endif

#pragma once
#include <syntacts>
#include <carnot>
#include "helpers.hpp"
#include <deque>

///////////////////////////////////////////////////////////////////////////////

struct Node {
    virtual void gui() = 0;
    virtual tact::Signal signal() = 0;
    virtual std::string name() = 0;
    virtual bool empty() { return false; };
    virtual float* scale() = 0;
    virtual float* offset() = 0;
};

Ptr<Node> makeNode(std::type_index id);

///////////////////////////////////////////////////////////////////////////////

struct NodeList : public Node { 
    void gui()
    {
        // render nodes
        for (std::size_t i = 0; i < m_nodes.size(); ++i)
        {
            auto &header = m_nodes[i]->name() + "###SignalSlot";
            ImGui::PushID(m_ids[i]);
            ImGui::PushStyleColor(ImGuiCol_Header, ImGui::GetStyle().Colors[ImGuiCol_TabActive]);
            if (ImGui::CollapsingHeader(header.c_str(), &m_closeHandles[i]))
            {
                ImGui::Indent();
                m_nodes[i]->gui();
                ImGui::Unindent();
            }
            ImGui::PopStyleColor();
            ImGui::PopID();
        }
        // 
        helpers::nodeSlot(m_nodes.size() == 0);
        if (helpers::nodeDropped(m_dropped))
        {
            m_nodes.emplace_back(makeNode(m_dropped));
            m_closeHandles.emplace_back(true);
            m_ids.push_back(m_nextId++);
        }
        // clean up 
        std::vector<Ptr<Node>> newNodes;
        std::vector<int>             newIds;
        for (int i = 0; i < m_nodes.size(); ++i) {
            if (m_closeHandles[i]) {
                newNodes.emplace_back(std::move(m_nodes[i]));   
                newIds.push_back(m_ids[i]);
            }     
        }
        m_nodes = std::move(newNodes);
        m_ids     = std::move(newIds);
        m_closeHandles = std::deque<bool>(m_nodes.size(), true);
    }

    float* scale() override { return &m_scale; }
    float* offset() override { return &m_offset; }

    std::vector<Ptr<Node>> m_nodes;
    std::deque<bool> m_closeHandles;
    std::vector<int> m_ids;
    int m_nextId;
    std::type_index m_dropped = typeid(tact::Scalar(0));
    float m_scale = 1, m_offset = 0;
};

///////////////////////////////////////////////////////////////////////////////

struct ProductNode : public NodeList
{
    tact::Signal signal() override {
        if (m_nodes.size() > 0) {
            auto sig = m_nodes[0]->signal();
            for (int i = 1; i < m_nodes.size(); ++i)
                sig = sig * m_nodes[i]->signal();
            return sig;
        }
        return tact::Scalar(1);
    }

    std::string name() override {
        return "Product";
    }
};

///////////////////////////////////////////////////////////////////////////////

struct SumNode : public NodeList
{
    tact::Signal signal() override {
        if (m_nodes.size() > 0) {
            auto sig = m_nodes[0]->signal();
            for (int i = 1; i < m_nodes.size(); ++i)
                sig = sig + m_nodes[i]->signal();
            return sig;
        }
        return tact::Scalar(1);
    }

    std::string name() override {
        return "Sum";
    }
};

///////////////////////////////////////////////////////////////////////////////

template <typename T>
struct SignalNode : public Node {

    tact::Signal signal() override { return sig; }
    std::string name() override { return helpers::signalName(sig.typeId()); }
    float* scale() override { return &sig.scale; }
    float* offset() override { return &sig.offset; }

protected:
    tact::Signal sig = T();

};

///////////////////////////////////////////////////////////////////////////////

struct OscillatorNode : public SignalNode<tact::Sine> {

    OscillatorNode(tact::Signal osc) 
    { 
        sig = std::move(osc);
    }

    void gui() override {
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
            ImGui::Text("Oops!");
            ImGui::Unindent();
        }
    }
};

///////////////////////////////////////////////////////////////////////////////

struct ChirpNode : public SignalNode<tact::Chirp> {
    
    void gui() override {
        auto cast = (tact::Chirp*)sig.get();
        if (cast->x.isType<tact::Time>()) {
            float f = cast->x.scale / tact::TWO_PI;
            ImGui::DragFloat("Frequency", &f, 1, 0, 1000, "%.0f Hz");
            cast->x.scale = f * tact::TWO_PI;
        }
        ImGui::DragFloat("Rate", &cast->rate, 1, 0, 0, "%.0f Hz/s");
    }
};

///////////////////////////////////////////////////////////////////////////////

struct ExpressionNode : public SignalNode<tact::Expression> {

    ExpressionNode() {
        auto cast = (tact::Expression*)sig.get();
        strcpy(buffer, cast->getExpression().c_str());
    }

    void gui() override {
        if (!ok)
            ImGui::PushStyleColor(ImGuiCol_FrameBg, Reds::FireBrick);
        bool entered = ImGui::InputText("f(t)", buffer, 256, ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue);
        if (!ok)
            ImGui::PopStyleColor();
        if (entered) {
            auto cast = (tact::Expression*)sig.get();
            std::string oldExpr = cast->getExpression();
            std::string newExpr(buffer);
            if (!cast->setExpression(newExpr)) {
                cast->setExpression(oldExpr);
                ok = false;
            }
            else {
                ok = true;
            }
        }
    }

    char buffer[256];
    bool ok = true;
};

///////////////////////////////////////////////////////////////////////////////

struct EnvelopeNode : public SignalNode<tact::Envelope> {

    void gui() {
        auto cast = (tact::Envelope*)sig.get();
        ImGui::DragFloat("Duration", &cast->duration, 0.001f, 0, 1, "%0.3f s");
        ImGui::DragFloat("Amplitude", &cast->amplitude, 0.01f, 0, 1);
    }
};

///////////////////////////////////////////////////////////////////////////////

struct ASRNode : public SignalNode<tact::ASR> {

    void gui() {
        auto cast = (tact::ASR*)sig.get();
        auto it = cast->keys.begin(); it++;
        auto& a = *(it++); auto& s = *(it++); auto& r = *(it++);

        float attack  = a.first;
        float sustain = s.first - a.first;
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
};

///////////////////////////////////////////////////////////////////////////////

struct ADSRNode : public SignalNode<tact::ADSR> {

    void gui() {
        auto cast = (tact::ASR*)sig.get();
        auto it = cast->keys.begin(); it++;
        auto& a = *(it++); auto& d = *(it++); auto& s = *(it++); auto& r = *(it++);

        float attack  = a.first;
        float decay = d.first - a.first;
        float sustain = s.first - d.first;
        float release = r.first - s.first;

        bool changed = false;
        if (ImGui::DragFloat("Attack",  &attack,  0.001f, 0.0001f, 1, "%0.3f s"))
            changed = true;    
        if (ImGui::DragFloat("Decay",  &decay,  0.001f, 0.0001f, 1, "%0.3f s"))
            changed = true;    
        if (ImGui::DragFloat("Sustain", &sustain, 0.001f, 0.0001f, 1, "%0.3f s"))
            changed = true;
        if (ImGui::DragFloat("Release", &release, 0.001f, 0.0001f, 1, "%0.3f s"))
            changed = true;

        if (changed)
            sig = tact::ADSR(attack, decay, sustain, release);
    }
};

///////////////////////////////////////////////////////////////////////////////

struct NoiseNode : public SignalNode<tact::Noise> {
    void gui() { 
        ImGui::DragFloat("Gain", &sig.scale,  0.001f, -1, 1, "%0.3f");
        ImGui::DragFloat("Bias", &sig.offset, 0.001f, -1, 1, "%0.3f");
    }
};

///////////////////////////////////////////////////////////////////////////////

struct TimeNode : public SignalNode<tact::Time> {
    void gui() { }
    std::string name() { return "Time"; }
    tact::Signal signal() { return sig; }
    tact::Signal sig = tact::Time();
};

///////////////////////////////////////////////////////////////////////////////

struct ScalarNode : SignalNode<tact::Scalar> {
    void gui() { 
        auto cast = (tact::Scalar*)sig.get();
        ImGui::DragFloat("Value", &cast->value, 0.01f, -10, 10);
    }
    std::string name() { return "Scalar"; }
    tact::Signal signal() { return sig; }
    tact::Signal sig = tact::Scalar();
};

///////////////////////////////////////////////////////////////////////////////

struct RampNode : SignalNode<tact::Ramp> {
    void gui() { 
        auto cast = (tact::Ramp*)sig.get();
        ImGui::DragFloat("Initial", &cast->initial, 0.01f, -10, 10);
        ImGui::DragFloat("Rate", &cast->rate, 0.01f, -10, 10);
    }
    std::string name() { return "Ramp"; }
    tact::Signal signal() { return sig; }
    tact::Signal sig = tact::Ramp();
};

///////////////////////////////////////////////////////////////////////////////

Ptr<Node> makeNode(std::type_index id) {
    // if (id == typeid(tact::Zero))           return tact::Zero();
    if (id == typeid(tact::Time))           return make<TimeNode>();
    if (id == typeid(tact::Scalar))         return make<ScalarNode>();
    if (id == typeid(tact::Ramp))           return make<RampNode>();
    if (id == typeid(tact::Noise))          return make<NoiseNode>();
    if (id == typeid(tact::Expression))     return make<ExpressionNode>();
    if (id == typeid(tact::Sum))            return make<SumNode>();
    if (id == typeid(tact::Product))        return make<ProductNode>();
    if (id == typeid(tact::Sine))           return make<OscillatorNode>(tact::Sine());
    if (id == typeid(tact::Square))         return make<OscillatorNode>(tact::Square());
    if (id == typeid(tact::Saw))            return make<OscillatorNode>(tact::Saw());
    if (id == typeid(tact::Triangle))       return make<OscillatorNode>(tact::Triangle());
    // if (id == typeid(tact::SineFM))         return tact::SineFM();
    if (id == typeid(tact::Chirp))          return make<ChirpNode>();
    // if (id == typeid(tact::Pwm))            return tact::Pwm();
    if (id == typeid(tact::Envelope))       return make<EnvelopeNode>();
    if (id == typeid(tact::ASR))            return make<ASRNode>();
    if (id == typeid(tact::ADSR))           return make<ADSRNode>();
    // if (id == typeid(tact::KeyedEnvelope))  return tact::KeyedEnvelope();
    // if (id == typeid(tact::SignalEnvelope)) return tact::SignalEnvelope();
    print("Couldn't Make Signal!");
    return nullptr;
}