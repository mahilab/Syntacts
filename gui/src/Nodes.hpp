#pragma once
#include <deque>
#include "PolyBezier.hpp"
#include "Palette.hpp"
#include "DragAndDrop.hpp"
#include <utility>

///////////////////////////////////////////////////////////////////////////////

void NodeSlot(const char* label, const ImVec2& size = ImVec2(-1, 0));

///////////////////////////////////////////////////////////////////////////////

struct Node {
    Node();
    virtual void update() = 0;
    virtual tact::Signal signal() = 0;
    virtual const std::string& name() = 0;
    virtual bool empty() { return false; };
    bool active = true;
    int id;
};

std::shared_ptr<Node> makeNode(PItem id);

///////////////////////////////////////////////////////////////////////////////

struct NodeList : public Node { 
    void update();
    std::vector<std::shared_ptr<Node>> m_nodes;
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
    void update() override;
protected:
    std::string libName;
    tact::Signal sig;    
};

///////////////////////////////////////////////////////////////////////////////

template <typename T>
struct SignalNode : public Node {
    tact::Signal signal() override { return sig; }
    const std::string& name() override { return signalName(sig); }
protected:
    tact::Signal sig = T();
};

///////////////////////////////////////////////////////////////////////////////

struct StretcherNode : public SignalNode<tact::Stretcher> {
    void update() override;
    std::string m_sigName = "##Empty";
};

///////////////////////////////////////////////////////////////////////////////

struct RepeaterNode : public SignalNode<tact::Repeater> {
    void update() override;
    std::string m_sigName = "##Empty";
};

///////////////////////////////////////////////////////////////////////////////

struct ReverserNode : public SignalNode<tact::Reverser> {
    void update() override;
    std::string m_sigName = "##Empty";
};

///////////////////////////////////////////////////////////////////////////////

struct OscillatorNode : public SignalNode<tact::Sine> {
    OscillatorNode(tact::Signal osc);
    void update() override;
};

///////////////////////////////////////////////////////////////////////////////

struct ChirpNode : public Node {    
    virtual void update() override;
    virtual tact::Signal signal() override;
    virtual const std::string& name() override;
    float f = 100;
    float r = 100;
    int ftype = 0;
};

///////////////////////////////////////////////////////////////////////////////

struct FmNode : public Node {
    virtual void update() override;
    virtual tact::Signal signal() override;
    virtual const std::string& name() override;
    float f = 100;
    float m = 10;
    int ftype = 0;
    float index = 2;
};

///////////////////////////////////////////////////////////////////////////////

struct PwmNode : public SignalNode<tact::Pwm> {
    void update() override;
};

///////////////////////////////////////////////////////////////////////////////

struct ExpressionNode : public SignalNode<tact::Expression> {
    ExpressionNode();
    void update();
    char buffer[256];
    bool ok = true;
};

///////////////////////////////////////////////////////////////////////////////

struct PolyBezierNode : public SignalNode<tact::PolyBezier> {
    PolyBezierNode();
    void update();
    void sync();
    ImGui::PolyBezier pb;
    float bounds[4] = {0,0,1,1};
};

///////////////////////////////////////////////////////////////////////////////

struct EnvelopeNode : public SignalNode<tact::Envelope> {
    void update();
};

///////////////////////////////////////////////////////////////////////////////

struct ASRNode : public SignalNode<tact::ASR> {
    void update() ;
};

///////////////////////////////////////////////////////////////////////////////

struct ADSRNode : public SignalNode<tact::ADSR> {
    void update();
};

///////////////////////////////////////////////////////////////////////////////
struct ExponentialDecayNode : public SignalNode<tact::ExponentialDecay> {
    void update();
};

///////////////////////////////////////////////////////////////////////////////

struct NoiseNode : public SignalNode<tact::Noise> {
    void update();
};

///////////////////////////////////////////////////////////////////////////////

struct TimeNode : public SignalNode<tact::Time> {
    void update();
};

///////////////////////////////////////////////////////////////////////////////

struct ScalarNode : SignalNode<tact::Scalar> {
    void update();
};

///////////////////////////////////////////////////////////////////////////////

struct RampNode : SignalNode<tact::Ramp> {
    void update();
};

///////////////////////////////////////////////////////////////////////////////