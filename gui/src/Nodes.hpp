#pragma once
#include <deque>
#include "ImGui/PolyBezier.hpp"
#include "Palette.hpp"
#include "DragAndDrop.hpp"
#include <utility>

///////////////////////////////////////////////////////////////////////////////

void NodeSlot(const char* label, const ImVec2& size = ImVec2(-1, 0), ImGuiCol col = ImGuiCol_Button);

const std::string& signalName(std::type_index id);
const std::string& signalName(const tact::Signal& sig);

///////////////////////////////////////////////////////////////////////////////

struct Node {
    virtual void gui() = 0;
    virtual tact::Signal signal() = 0;
    virtual const std::string& name() = 0;
    virtual bool empty() { return false; };
};

std::shared_ptr<Node> makeNode(PItem id);

///////////////////////////////////////////////////////////////////////////////

struct NodeList : public Node { 
    void gui();
    std::vector<std::shared_ptr<Node>> m_nodes;
    std::deque<bool> m_closeHandles;
    std::vector<int> m_ids;
    int m_nextId;
    float m_scale = 1, m_offset = 0;
};

///////////////////////////////////////////////////////////////////////////////

struct ProductNode : public NodeList
{
    tact::Signal signal() override;
    const std::string& name() override;
};

///////////////////////////////////////////////////////////////////////////////

struct SumNode : public NodeList
{
    tact::Signal signal() override;
    const std::string& name() override;
};

///////////////////////////////////////////////////////////////////////////////

struct LibrarySignalNode : public Node {
    LibrarySignalNode(const std::string& name);
    tact::Signal signal() override;
    const std::string& name() override;
    void gui() override;
protected:
    std::string libName;
    tact::Signal sig;    
};

///////////////////////////////////////////////////////////////////////////////

template <typename T>
struct SignalNode : public Node {
    tact::Signal signal() override { return sig; }
    const std::string& name() override { return signalName(sig.typeId()); }
protected:
    tact::Signal sig = T();
};

///////////////////////////////////////////////////////////////////////////////

struct StretcherNode : public SignalNode<tact::Stretcher> {
    void gui() override;
    std::string m_sigName = "##Empty";
};

///////////////////////////////////////////////////////////////////////////////

struct RepeaterNode : public SignalNode<tact::Repeater> {
    void gui() override;
    std::string m_sigName = "##Empty";
};

///////////////////////////////////////////////////////////////////////////////

struct ReverserNode : public SignalNode<tact::Reverser> {
    void gui() override;
    std::string m_sigName = "##Empty";
};

///////////////////////////////////////////////////////////////////////////////

struct OscillatorNode : public SignalNode<tact::Sine> {
    OscillatorNode(tact::Signal osc);
    void gui() override;
};

///////////////////////////////////////////////////////////////////////////////

struct ChirpNode : public Node {    
    virtual void gui() override;
    virtual tact::Signal signal() override;
    virtual const std::string& name() override;
    float f = 100;
    float r = 100;
    int ftype = 0;
};

///////////////////////////////////////////////////////////////////////////////

struct FmNode : public Node {
    virtual void gui() override;
    virtual tact::Signal signal() override;
    virtual const std::string& name() override;
    float f = 100;
    float m = 10;
    int ftype = 0;
    float index = 2;
};

///////////////////////////////////////////////////////////////////////////////

struct PwmNode : public SignalNode<tact::Pwm> {
    void gui() override;
};

///////////////////////////////////////////////////////////////////////////////

struct ExpressionNode : public SignalNode<tact::Expression> {
    ExpressionNode();
    void gui();
    char buffer[256];
    bool ok = true;
};

///////////////////////////////////////////////////////////////////////////////

struct PolyBezierNode : public SignalNode<tact::PolyBezier> {
    PolyBezierNode();
    void gui();
    void update();
    ImGui::PolyBezier pb;
    float bounds[4] = {0,0,1,1};
};

///////////////////////////////////////////////////////////////////////////////

struct EnvelopeNode : public SignalNode<tact::Envelope> {
    void gui();
};

///////////////////////////////////////////////////////////////////////////////

struct ASRNode : public SignalNode<tact::ASR> {
    void gui() ;
};

///////////////////////////////////////////////////////////////////////////////

struct ADSRNode : public SignalNode<tact::ADSR> {
    void gui();
};

///////////////////////////////////////////////////////////////////////////////

struct NoiseNode : public SignalNode<tact::Noise> {
    void gui();
};

///////////////////////////////////////////////////////////////////////////////

struct TimeNode : public SignalNode<tact::Time> {
    void gui();
};

///////////////////////////////////////////////////////////////////////////////

struct ScalarNode : SignalNode<tact::Scalar> {
    void gui();
};

///////////////////////////////////////////////////////////////////////////////

struct RampNode : SignalNode<tact::Ramp> {
    void gui();
};

///////////////////////////////////////////////////////////////////////////////