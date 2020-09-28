#pragma once
#include <deque>
#include "PolyBezier.hpp"
#include "Palette.hpp"
#include "Sequencer.hpp"
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

/// Make Node from Palette ID
std::shared_ptr<Node> makeNode(PItem id);
/// Make Node from Signal
std::shared_ptr<Node> makeNode(const tact::Signal& sig);
/// Make Node from Signal
std::shared_ptr<Node> makeRoot(const tact::Signal& sig);

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
    std::string libName;
    tact::Signal sig;    
};

///////////////////////////////////////////////////////////////////////////////

template <typename T>
struct SignalNode : public Node {
    SignalNode() { sig = T(); }
    SignalNode(const tact::Signal& _sig) { sig = _sig; }
    tact::Signal signal() override { return sig; }
    const std::string& name() override { return signalName(sig); }
    tact::Signal sig;
};

///////////////////////////////////////////////////////////////////////////////

struct StretcherNode : public SignalNode<tact::Stretcher> {
    void update() override;
    std::shared_ptr<Node> root = std::make_shared<ProductNode>();
};

///////////////////////////////////////////////////////////////////////////////

struct RepeaterNode : public SignalNode<tact::Repeater> {
    void update() override;
    std::shared_ptr<Node> root = std::make_shared<ProductNode>();
};

///////////////////////////////////////////////////////////////////////////////

struct ReverserNode : public SignalNode<tact::Reverser> {
    void update() override;
    std::shared_ptr<Node> root = std::make_shared<ProductNode>();
};

///////////////////////////////////////////////////////////////////////////////
struct SequencerNode : public Node {
    SequencerNode();
    SequencerNode(tact::Sequence _seq);
    tact::Signal signal() override;
    const std::string& name() override;
    void update() override;
    tact::Sequence seq;
    ImGui::SeqInterface interface;
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
    double f = 100;
    double r = 100;
    int ftype = 0;
};

///////////////////////////////////////////////////////////////////////////////

struct FmNode : public Node {
    virtual void update() override;
    virtual tact::Signal signal() override;
    virtual const std::string& name() override;
    double f = 100;
    double m = 10;
    int ftype = 0;
    double index = 2;
};

///////////////////////////////////////////////////////////////////////////////

struct PwmNode : public SignalNode<tact::Pwm> {
    using SignalNode::SignalNode;
    void update() override;
};

///////////////////////////////////////////////////////////////////////////////

struct ExpressionNode : public SignalNode<tact::Expression> {
    ExpressionNode();
    ExpressionNode(const tact::Signal& _sig);
    void getString();
    void update();
    char buffer[256];
    bool ok = true;
};

///////////////////////////////////////////////////////////////////////////////

struct PolyBezierNode : public SignalNode<tact::PolyBezier> {
    PolyBezierNode();
    PolyBezierNode(const tact::Signal& _sig);
    void update();
    void sync();
    ImGui::PolyBezier pb;
    float bounds[4] = {0,0,1,1};
};

///////////////////////////////////////////////////////////////////////////////

struct SamplesNode : public SignalNode<tact::Samples> {
    using SignalNode::SignalNode;
    void update();
};

///////////////////////////////////////////////////////////////////////////////

struct EnvelopeNode : public SignalNode<tact::Envelope> {
    using SignalNode::SignalNode;
    void update();
};

///////////////////////////////////////////////////////////////////////////////

struct KeyedEnvelopeNode : public SignalNode<tact::KeyedEnvelope> {
    using SignalNode::SignalNode;
    void update();
    std::vector<double> Ts;
    std::vector<double> As;
    std::vector<tact::Curve> Cs;
};

///////////////////////////////////////////////////////////////////////////////

struct ASRNode : public SignalNode<tact::ASR> {
    using SignalNode::SignalNode;
    void update() ;
};

///////////////////////////////////////////////////////////////////////////////

struct ADSRNode : public SignalNode<tact::ADSR> {
    using SignalNode::SignalNode;
    void update();
};

///////////////////////////////////////////////////////////////////////////////

struct SignalEnvelopeNode : public SignalNode<tact::SignalEnvelope> {
    void update() override;
    std::shared_ptr<Node> root = std::make_shared<ProductNode>();
};

///////////////////////////////////////////////////////////////////////////////
struct ExponentialDecayNode : public SignalNode<tact::ExponentialDecay> {
    using SignalNode::SignalNode;
    void update();
};

///////////////////////////////////////////////////////////////////////////////

struct NoiseNode : public SignalNode<tact::Noise> {
    using SignalNode::SignalNode;
    void update();
};

///////////////////////////////////////////////////////////////////////////////

struct TimeNode : public SignalNode<tact::Time> {
    using SignalNode::SignalNode;
    void update();
};

///////////////////////////////////////////////////////////////////////////////

struct ScalarNode : SignalNode<tact::Scalar> {
    using SignalNode::SignalNode;
    void update();
};

///////////////////////////////////////////////////////////////////////////////

struct RampNode : SignalNode<tact::Ramp> {
    using SignalNode::SignalNode;
    void update();
};

///////////////////////////////////////////////////////////////////////////////