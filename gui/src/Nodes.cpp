#include "Nodes.hpp"
#include "Gui.hpp"

using namespace mahi::gui;

namespace {
    int g_nextNodeId = 0;
}

///////////////////////////////////////////////////////////////////////////////

void NodeSlot(const char *label, const ImVec2 &size) {
    BeginPulsable(true,true);
    ImGui::Button(label, size);
    EndPulsable();
}

std::shared_ptr<Node> makeNode(PItem id) {
    if (id == PItem::Time)
        return std::make_shared<TimeNode>();
    if (id == PItem::Scalar)
        return std::make_shared<ScalarNode>();
    if (id == PItem::Ramp)
        return std::make_shared<RampNode>();
    if (id == PItem::Noise)
        return std::make_shared<NoiseNode>();
    if (id == PItem::Expression)
        return std::make_shared<ExpressionNode>();
    if (id == PItem::Sum)
        return std::make_shared<SumNode>();
    if (id == PItem::Product)
        return std::make_shared<ProductNode>();
    if (id == PItem::Sine)
        return std::make_shared<OscillatorNode>(tact::Sine());
    if (id == PItem::Square)
        return std::make_shared<OscillatorNode>(tact::Square());
    if (id == PItem::Saw)
        return std::make_shared<OscillatorNode>(tact::Saw());
    if (id == PItem::Triangle)
        return std::make_shared<OscillatorNode>(tact::Triangle());
    if (id == PItem::Chirp)
        return std::make_shared<ChirpNode>();
    if (id == PItem::Envelope)
        return std::make_shared<EnvelopeNode>();
    if (id == PItem::KeyedEnvelope)
        return std::make_shared<KeyedEnvelopeNode>();
    if (id == PItem::ASR)
        return std::make_shared<ASRNode>();
    if (id == PItem::ADSR)
        return std::make_shared<ADSRNode>();
    if (id == PItem::ExponentialDecay)
        return std::make_shared<ExponentialDecayNode>();
    if (id == PItem::SignalEnvelope)
        return std::make_shared<SignalEnvelopeNode>();
    if (id == PItem::PolyBezier)
        return std::make_shared<PolyBezierNode>();
    if (id == PItem::Stretcher)
        return std::make_shared<StretcherNode>();
    if (id == PItem::Repeater)
        return std::make_shared<RepeaterNode>();
    if (id == PItem::Reverser)
        return std::make_shared<ReverserNode>();
    if (id == PItem::Sequencer)
        return std::make_shared<SequencerNode>();
    if (id == PItem::Pwm)
        return std::make_shared<PwmNode>();
    if (id == PItem::FM)
        return std::make_shared<FmNode>();
    return nullptr;
}

template <typename T>
std::shared_ptr<Node> makeOscNode(const tact::Signal& sig, int idx) {
    auto osc = sig.getAs<T>();
    if (osc->x.template isType<tact::Time>()) {
        auto node = std::make_shared<OscillatorNode>(T());
        node->sig = sig;
        return node;
    }
    else if (osc->x.template isType<tact::Product>()) {
        auto x = osc->x.template getAs<tact::Product>();
        if (x->lhs.template isType<tact::Time>() && x->rhs.template isType<tact::Time>()) {
            auto node = std::make_shared<ChirpNode>();
            node->f = x->lhs.bias / tact::TWO_PI;
            node->r = x->lhs.gain / tact::PI;
            return node;
        }
    }
    else if (osc->x.template isType<tact::Sum>()) {
        auto x = osc->x.template getAs<tact::Sum>();
        if (x->lhs.template isType<tact::Time>() && x->rhs.template isType<tact::Sine>()) {
            auto node = std::make_shared<FmNode>();
            node->f = x->lhs.gain / tact::TWO_PI;
            node->index = x->rhs.gain;
            node->ftype = idx;
            node->m =  x->rhs.template getAs<tact::Sine>()->x.gain / tact::TWO_PI;
            return node;
        }
    }
    return nullptr;
}

bool recurseProduct(std::shared_ptr<ProductNode> root, const tact::Signal& sig) {
    auto prod = sig.getAs<tact::Product>();
    if (prod->lhs.isType<tact::Product>()) 
        recurseProduct(root, prod->lhs);    
    else if (auto lhs = makeNode(prod->lhs))
        root->m_nodes.push_back(lhs);    
    else
        return false;

    if (prod->rhs.isType<tact::Product>()) 
        recurseProduct(root, prod->rhs);    
    else if (auto rhs = makeNode(prod->rhs))
        root->m_nodes.push_back(rhs);    
    else
        return false;

    return true;
}

bool recurseSum(std::shared_ptr<SumNode> root, const tact::Signal& sig) {
    auto prod = sig.getAs<tact::Sum>();
    if (prod->lhs.isType<tact::Sum>()) 
        recurseSum(root, prod->lhs);    
    else if (auto lhs = makeNode(prod->lhs))
        root->m_nodes.push_back(lhs);    
    else 
        return false;

    if (prod->rhs.isType<tact::Sum>()) 
        recurseSum(root, prod->rhs);    
    else if (auto rhs = makeNode(prod->rhs))
        root->m_nodes.push_back(rhs); 
    else
        return false;

    return true;   
}

template <typename N, typename S>
std::shared_ptr<N> makeProcessNode(const tact::Signal& sig) {
    auto pro = sig.getAs<S>();
    if (auto root = makeRoot(pro->signal)) {
        auto node = std::make_shared<N>();
        node->sig = sig;
        node->root = root;
        return node;
    }
    return nullptr;
}

std::shared_ptr<Node> makeNode(const tact::Signal& sig) {
    if (sig.isType<tact::Scalar>()) 
        return std::make_shared<ScalarNode>(sig); 
    else if (sig.isType<tact::Time>()) 
        return std::make_shared<TimeNode>(sig);  
    else if (sig.isType<tact::Ramp>()) 
        return std::make_shared<RampNode>(sig);  
    else if (sig.isType<tact::Noise>())
        return std::make_shared<NoiseNode>(sig);
    else if (sig.isType<tact::Expression>()) 
        return std::make_shared<ExpressionNode>(sig);    
    else if (sig.isType<tact::PolyBezier>())
        return std::make_shared<PolyBezierNode>(sig);
    else if (sig.isType<tact::Samples>()) 
        return std::make_shared<SamplesNode>(sig);
    else if (sig.isType<tact::Sum>()) {
        auto node = std::make_shared<SumNode>();
        if (recurseSum(node, sig))
            return node;
    }
    else if (sig.isType<tact::Product>()) {
        auto node = std::make_shared<ProductNode>();
        if (recurseProduct(node, sig))
            return node;
    }
    else if (sig.isType<tact::Sequence>()) {
        auto seq = sig.getAs<tact::Sequence>();
        return std::make_shared<SequencerNode>(*seq);
    }
    else if (sig.isType<tact::Sine>())
        return makeOscNode<tact::Sine>(sig, 0);    
    else if (sig.isType<tact::Square>())
        return makeOscNode<tact::Square>(sig, 1);    
    else if (sig.isType<tact::Saw>())
        return makeOscNode<tact::Saw>(sig, 2);    
    else if (sig.isType<tact::Triangle>())
        return makeOscNode<tact::Triangle>(sig, 3);
    else if (sig.isType<tact::Pwm>())
        return std::make_shared<PwmNode>(sig);  
    else if (sig.isType<tact::Envelope>()) 
        return std::make_shared<EnvelopeNode>(sig);  
    else if (sig.isType<tact::KeyedEnvelope>())
        return std::make_shared<KeyedEnvelopeNode>(sig);
    else if (sig.isType<tact::ASR>()) 
        return std::make_shared<ASRNode>(sig);       
    else if (sig.isType<tact::ADSR>()) 
        return std::make_shared<ADSRNode>(sig);   
    else if (sig.isType<tact::ExponentialDecay>()) 
        return std::make_shared<ExponentialDecayNode>(sig); 
    else if (sig.isType<tact::SignalEnvelope>()) 
        return makeProcessNode<SignalEnvelopeNode, tact::SignalEnvelope>(sig);
    else if (sig.isType<tact::Repeater>())
        return makeProcessNode<RepeaterNode, tact::Repeater>(sig);
    else if (sig.isType<tact::Stretcher>())
        return makeProcessNode<StretcherNode, tact::Stretcher>(sig);
    else if (sig.isType<tact::Reverser>())
        return makeProcessNode<ReverserNode, tact::Reverser>(sig);
    return nullptr;
}

/// Make Node from Signal
std::shared_ptr<Node> makeRoot(const tact::Signal& sig) {
    auto root = std::make_shared<ProductNode>();
    if (sig.isType<tact::Product>()) {
        if (recurseProduct(root, sig))
            return root;
    }
    else {
        auto node = makeNode(sig);
        if (node) {
            root->m_nodes.push_back(node);
            return root;
        }
    }
    return nullptr;
}

///////////////////////////////////////////////////////////////////////////////

Node::Node() : active(true), id(g_nextNodeId++) { }

///////////////////////////////////////////////////////////////////////////////

void NodeList::update()
{
    // render nodes
    for (std::size_t i = 0; i < m_nodes.size(); ++i)
    {
        std::string header = m_nodes[i]->name() + "###SignalSlot";
        ImGui::PushID(m_nodes[i]->id);
        ImGui::PushStyleColor(ImGuiCol_Header, ImGui::GetStyle().Colors[ImGuiCol_TabActive]);
        if (ImGui::CollapsingHeader(header.c_str(), &m_nodes[i]->active, ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Indent();
            m_nodes[i]->update();
            ImGui::Unindent();
        }
        ImGui::PopStyleColor();
        ImGui::PopID();
    }
    // node slot
    if (m_nodes.size() == 0 || SignalHeld() || PaletteHeld())
        NodeSlot("##EmptySlot", ImVec2(-1, 0));
    // check for incoming palette items
    if (PaletteTarget())
    {
        auto node = makeNode(PalettePayload());
        if (node)
            m_nodes.emplace_back(node);
    }
    // check for incoming library items
    if (SignalTarget())
    {
        auto node = std::make_shared<LibrarySignalNode>(SignalPayload().first);
        m_nodes.emplace_back(node);
    }
    // clean up
    std::vector<std::shared_ptr<Node>> newNodes;
    newNodes.reserve(m_nodes.size());
    std::vector<int> newIds;
    for (int i = 0; i < m_nodes.size(); ++i)
    {
        if (m_nodes[i]->active)        
            newNodes.emplace_back(std::move(m_nodes[i]));        
    }
    m_nodes = std::move(newNodes);
}

///////////////////////////////////////////////////////////////////////////////

tact::Signal ProductNode::signal()
{
    if (m_nodes.size() > 0)
    {
        auto sig = m_nodes[0]->signal();
        for (int i = 1; i < m_nodes.size(); ++i)
            sig = sig * m_nodes[i]->signal();
        return sig;
    }
    return tact::Signal();
}

const std::string &ProductNode::name()
{
    static std::string n = "Product";
    return n;
}

///////////////////////////////////////////////////////////////////////////////

tact::Signal SumNode::signal()
{
    if (m_nodes.size() > 0)
    {
        auto sig = m_nodes[0]->signal();
        for (int i = 1; i < m_nodes.size(); ++i)
            sig = sig + m_nodes[i]->signal();
        return sig;
    }
    return tact::Signal();
}

const std::string &SumNode::name()
{
    static std::string n = "Sum";
    return n;
}

///////////////////////////////////////////////////////////////////////////////

LibrarySignalNode::LibrarySignalNode(const std::string &name) : libName(name)
{
    tact::Library::loadSignal(sig, libName);
}

tact::Signal LibrarySignalNode::signal() { return sig; }
const std::string &LibrarySignalNode::name() { return libName; }
// float* LibrarySignalNode::gain() { return &sig.scale; }
// float* LibrarySignalNode::bias() { return &sig.offset; }
void LibrarySignalNode::update()
{
}

///////////////////////////////////////////////////////////////////////////////

void StretcherNode::update()
{
    auto cast = (tact::Stretcher *)sig.get();
    root->update();
    cast->signal = root->signal();
    float factor = (float)cast->factor;
    if (ImGui::SliderFloat("Factor", &factor, 0, 10))
        cast->factor = factor;
}

///////////////////////////////////////////////////////////////////////////////

void RepeaterNode::update()
{
    auto cast = (tact::Repeater *)sig.get();
    root->update();
    cast->signal = root->signal();
    ImGui::SliderInt("Repetitions", &cast->repetitions, 1, 100);
    float delay = (float)cast->delay;
    if (ImGui::SliderFloat("Delay", &delay, 0, 1))
        cast->delay = delay;
}

///////////////////////////////////////////////////////////////////////////////

void ReverserNode::update()
{
    auto cast = (tact::Repeater *)sig.get();
    root->update();
    cast->signal = root->signal();
}

///////////////////////////////////////////////////////////////////////////////

SequencerNode::SequencerNode() {
    interface.activeColor   = mahi::gui::Purples::Plum;
    interface.inactiveColor = mahi::gui::Grays::Gray50;
}

SequencerNode::SequencerNode(tact::Sequence _seq) : SequencerNode() {
    seq = _seq;
    int key_count = seq.keyCount();
    for (int i = 0; i < key_count; ++i) {
        auto& key = seq.getKey(i);
        ImGui::SeqInterface::Track track;
        track.signal    = key.signal;
        track.label     = "Track " + std::to_string(i+1);
        track.t         = key.t;
        track.populated = true;
        interface.tracks.push_back(track);
    }
    interface.fitThisFrame = true;
}

tact::Signal SequencerNode::signal() {
    seq.clear();
    for (auto& track : interface.tracks) {
        if (track.populated && track.visible)
            seq.insert(track.signal, track.t);
    }
    return seq;
}

const std::string& SequencerNode::name() {
    static const std::string n = "Sequencer";
    return n;
}

void SequencerNode::update() {
    ImGui::Sequencer("Sequencer", interface, false);
}


///////////////////////////////////////////////////////////////////////////////

OscillatorNode::OscillatorNode(tact::Signal osc)
{
    sig = std::move(osc);
}

void OscillatorNode::update()
{
    auto cast = (tact::IOscillator *)sig.get();
    if (ImGui::RadioButton("Sine", sig.isType<tact::Sine>()))
    {
        sig = tact::Sine(cast->x);
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("Square", sig.isType<tact::Square>()))
    {
        sig = tact::Square(cast->x);
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("Saw", sig.isType<tact::Saw>()))
    {
        sig = tact::Saw(cast->x);
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("Triangle", sig.isType<tact::Triangle>()))
    {
        sig = tact::Triangle(cast->x);
    }
    cast = (tact::IOscillator *)sig.get();
    if (cast->x.isType<tact::Time>())
    {
        float f = (float)cast->x.gain / (float)tact::TWO_PI;
        ImGui::DragFloat("##Frequency", &f, 1, 0, 1000, "%.0f Hz");
        ImGui::SameLine();
        ImGui::Text("Frequency");
        cast->x.gain = f * tact::TWO_PI;
    }
    else
    {
        ImGui::Indent();
        ImGui::Text("Oops!");
        ImGui::Unindent();
    }
}

///////////////////////////////////////////////////////////////////////////////

namespace {

const std::string& oscName(int type) {
    static std::vector<std::string> types = {"Sine", "Square", "Saw", "Triangle"};
    return types[type];
}

template <typename ... Args> 
tact::Signal makeOsc(int type, Args ... args) {
    if (type == 0)
        return tact::Sine(args ...);
    else if (type == 1) 
        return tact::Square(args ...);
    else if (type == 2)
        return tact::Saw(args ...);
    else 
        return tact::Triangle(args ...);
}
}

void ChirpNode::update()
{
    for (int i = 0; i < 4; ++i) {
        if (ImGui::RadioButton(oscName(i).c_str(), ftype == i))
            ftype = i;
        if (i != 3)
            ImGui::SameLine();
    }
    ImGui::DragDouble("Frequency", &f, 1, 0, 1000, "%.0f Hz");
    ImGui::DragDouble("Rate", &r, 1, 0, 1000, "%.0f Hz/s");
}

tact::Signal ChirpNode::signal()
{
    return makeOsc(ftype, f, r);
}

const std::string &ChirpNode::name()
{
    static std::string fmName = "Chirp";
    return fmName;
}

///////////////////////////////////////////////////////////////////////////////

void FmNode::update()
{
    for (int i = 0; i < 4; ++i) {
        if (ImGui::RadioButton(oscName(i).c_str(), ftype == i))
            ftype = i;
        if (i != 3)
            ImGui::SameLine();
    }
    ImGui::DragDouble("Frequency", &f, 1, 0, 1000, "%.0f Hz");
    ImGui::DragDouble("Modulation", &m, 0.1f, 0, 100, "%.0f Hz");
    ImGui::DragDouble("Index", &index, 0.01f, 0, 10);
}

tact::Signal FmNode::signal()
{
    return makeOsc(ftype, f, tact::Sine(m), index);
}

const std::string &FmNode::name()
{
    static std::string fmName = "FM";
    return fmName;
}

///////////////////////////////////////////////////////////////////////////////

void PwmNode::update()
{
    auto cast = (tact::Pwm *)sig.get();
    float f = (float)cast->frequency;
    float d = (float)cast->dutyCycle;
    if (ImGui::DragFloat("Frequency", &f, 1, 0, 1000))
        cast->frequency = f;
    if (ImGui::DragFloat("Duty Cycle", &d, 0.001f, 0, 1.0f))
        cast->dutyCycle = d;
}

///////////////////////////////////////////////////////////////////////////////

ExpressionNode::ExpressionNode()
{
    getString();
}

ExpressionNode::ExpressionNode(const tact::Signal& _sig) {
    sig = _sig;
    getString();
}

void ExpressionNode::getString() {
    auto cast = (tact::Expression *)sig.get();
    strcpy(buffer, cast->getExpression().c_str());
}

void ExpressionNode::update()
{
    if (!ok)
        ImGui::PushStyleColor(ImGuiCol_FrameBg, Reds::FireBrick);
    bool entered = ImGui::InputText("f(t)", buffer, 256, ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue);
    if (!ok)
        ImGui::PopStyleColor();
    if (entered)
    {
        auto cast = (tact::Expression *)sig.get();
        std::string oldExpr = cast->getExpression();
        std::string newExpr(buffer);
        if (!cast->setExpression(newExpr))
        {
            cast->setExpression(oldExpr);
            ok = false;
        }
        else
        {
            ok = true;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////

PolyBezierNode::PolyBezierNode() : 
    pb(ImGui::GetStyle().Colors[ImGuiCol_PlotLines], ImVec2(0, 0), ImVec2(1, 1)) 
{
    sync();
}

PolyBezierNode::PolyBezierNode(const tact::Signal& _sig) :
    SignalNode<tact::PolyBezier>(_sig),
    pb(ImGui::GetStyle().Colors[ImGuiCol_PlotLines], ImVec2(0, 0), ImVec2(1, 1)) 
{ 
    bounds[2] = (float)(sig.length());
    auto tpb = *sig.getAs<tact::PolyBezier>();
    pb.clearPoints();
    for (auto& p : tpb.points) 
        pb.addPoint({(float)p.cpL.t, (float)p.cpL.y}, {(float)p.p.t, (float)p.p.y}, {(float)p.cpR.t, (float)p.cpR.y});       
}

void PolyBezierNode::update()
{
    ImGui::PushItemWidth(-1);
    ImGui::PolyBezierEdit("##PolyBezier", &pb, 10, 10, ImVec2(-1, 125));
    if (HelpTarget())
        ImGui::OpenPopup("PolyBezier Editor");
    if (ImGui::BeginHelpPopup("PolyBezier Editor")) {
        ImGui::BulletText("Drag points and handles with left mouse");
        ImGui::BulletText("Ctrl + left-click handles to toggle tangency constraints");
        ImGui::BulletText("Right-click to add new points");
        ImGui::BulletText("Press the Delete key to remove the selected point");
        ImGui::EndPopup();
    }
    ImGui::PopItemWidth();
    ImGui::DragFloat("Duration", &bounds[2], 0.1f, 0, 10, "%.3f s");
    pb.min.x = bounds[0];
    pb.min.y = bounds[1];
    pb.max.x = bounds[2];
    pb.max.y = bounds[3];
    sync();
}

void PolyBezierNode::sync() {
    auto cast = (tact::PolyBezier *)sig.get();
    int points = pb.pointCount();
    cast->points.resize(points);
    for (int i = 0; i < points; ++i)
    {
        ImVec2 cpL, pos, cpR;
        pb.getPoint(i, &cpL, &pos, &cpR);
        cast->points[i].cpL = {cpL.x, cpL.y};
        cast->points[i].p = {pos.x, pos.y};
        cast->points[i].cpR = {cpR.x, cpR.y};
    }
    cast->solve();
}

////////////////////////////////////////////////////////////////////////////////

void SamplesNode::update() {
    auto samples = sig.getAs<tact::Samples>();
    ImGui::Text("Sample Count: %d", samples->sampleCount());
    ImGui::Text("Sample Rate:  %.0f Hz", samples->sampleRate());
}

///////////////////////////////////////////////////////////////////////////////

void EnvelopeNode::update()
{
    auto cast = (tact::Envelope *)sig.get();
    ImGui::DragDouble("Duration", &cast->duration, 0.001f, 0, 1, "%0.3f s");
    ImGui::DragDouble("Amplitude", &cast->amplitude, 0.01f, 0, 1);
}

///////////////////////////////////////////////////////////////////////////////

static const char* CURVE_NAMES[] = {
    "Instant",
    "Delayed",
    "Linear",
    "Smoothstep",
    "Smootherstep",
    "Smootheststep",
    "Quadratic::In",
    "Quadratic::Out",
    "Quadratic::InOut",
    "Quadratic::In",
    "Quadratic::Out",
    "Quadratic::InOut",
    "Cubic::In",
    "Cubic::Out",
    "Cubic::InOut",
    "Quartic::In",
    "Quartic::Out",
    "Quartic::InOut",
    "Quintic::In",
    "Quintic::Out",
    "Quintic::InOut",
    "Sinusoidal::In",
    "Sinusoidal::Out",
    "Sinusoidal::InOut",
    "Exponential::In",
    "Exponential::Out",
    "Exponential::InOut",
    "Circular::In",
    "Circular::Out",
    "Circular::InOut",
    "Elastic::In",
    "Elastic::Out",
    "Elastic::InOut",
    "Back::In",
    "Back::Out",
    "Back::InOut",
    "Bounce::In",
    "Bounce::Out",
    "Bounce::InOut",
};

static const tact::Curve CURVES[] = {
    tact::Curves::Instant(),
    tact::Curves::Delayed(),
    tact::Curves::Linear(),
    tact::Curves::Smoothstep(),
    tact::Curves::Smootherstep(),
    tact::Curves::Smootheststep(),
    tact::Curves::Quadratic::In(),
    tact::Curves::Quadratic::Out(),
    tact::Curves::Quadratic::InOut(),
    tact::Curves::Quadratic::In(),
    tact::Curves::Quadratic::Out(),
    tact::Curves::Quadratic::InOut(),
    tact::Curves::Cubic::In(),
    tact::Curves::Cubic::Out(),
    tact::Curves::Cubic::InOut(),
    tact::Curves::Quartic::In(),
    tact::Curves::Quartic::Out(),
    tact::Curves::Quartic::InOut(),
    tact::Curves::Quintic::In(),
    tact::Curves::Quintic::Out(),
    tact::Curves::Quintic::InOut(),
    tact::Curves::Sinusoidal::In(),
    tact::Curves::Sinusoidal::Out(),
    tact::Curves::Sinusoidal::InOut(),
    tact::Curves::Exponential::In(),
    tact::Curves::Exponential::Out(),
    tact::Curves::Exponential::InOut(),
    tact::Curves::Circular::In(),
    tact::Curves::Circular::Out(),
    tact::Curves::Circular::InOut(),
    tact::Curves::Elastic::In(),
    tact::Curves::Elastic::Out(),
    tact::Curves::Elastic::InOut(),
    tact::Curves::Back::In(),
    tact::Curves::Back::Out(),
    tact::Curves::Back::InOut(),
    tact::Curves::Bounce::In(),
    tact::Curves::Bounce::Out(),
    tact::Curves::Bounce::InOut()
};

void KeyedEnvelopeNode::update() {
    auto cast = sig.getAs<tact::KeyedEnvelope>();
    Ts.clear(); As.clear(); Cs.clear();
    int key_count = cast->keys.size();
    int i = 0;
    for (auto it = cast->keys.begin(); it != cast->keys.end(); ++it) {
        double t       = it->first;
        double a       = it->second.first;
        tact::Curve c  = it->second.second;
        bool first_key = it == cast->keys.begin();
        bool last_key  = std::next(it) == cast->keys.end();
        double tprev, tnext;
        if (first_key)
            tprev = 0;
        else 
            tprev = std::prev(it)->first;
        if (last_key)
            tnext = 0;
        else
            tnext = std::next(it)->first;   
        double tmin = first_key ? 0 : tprev + 0.001;
        double tmax = last_key  ? t + 1000 : tnext - 0.001; 
        ImGui::PushID(i);
        ImGui::BeginDisabled(first_key);
        ImGui::SetNextItemWidth(160);      
        ImGui::DragDouble("##T",&t, 0.0005f, tmin, tmax, "%.3f s");
        ImGui::EndDisabled();
        ImGui::SameLine();
        ImGui::SetNextItemWidth(160);
        ImGui::DragDouble("##A",&a,0.005f,0,1);
        ImGui::SameLine();
        ImGui::BeginDisabled(first_key);
        ImGui::SetNextItemWidth(160);
        if (ImGui::BeginCombo("##Curve", c.name())) {
            for (int i = 0; i < 39; ++i) {
                bool selected = strcmp(c.name(),CURVE_NAMES[i]) == 0;
                if (ImGui::Selectable(CURVE_NAMES[i],&selected))
                    c = CURVES[i];
            }
            ImGui::EndCombo();
        }
        ImGui::SameLine();
        if (!ImGui::Button(ICON_FA_MINUS)) {
            Ts.push_back(first_key ?  0 : t);
            As.push_back(a);
            Cs.push_back(c);
        }
        ImGui::EndDisabled();
        ImGui::SameLine();
        if (ImGui::Button(ICON_FA_PLUS)) {
            double tp = last_key ? + t + 0.1f : (t + tnext) / 2;
            Ts.push_back(tp);
            As.push_back(a);
            Cs.push_back(tact::Curves::Linear());
        }       
        ImGui::PopID();
        i++;
    }
    cast->keys.clear();
    for (int i = 0; i < Ts.size(); ++i) 
        cast->addKey(Ts[i], As[i], Cs[i]);    
}

///////////////////////////////////////////////////////////////////////////////

void ASRNode::update()
{
    static const float minDur = 0.001f;
    auto cast = (tact::ASR *)sig.get();
    auto it = cast->keys.begin();
    it++;
    auto &a = *(it++);
    auto &s = *(it++);
    auto &r = *(it++);

    float asr[3];
    asr[0] = a.first;
    asr[1] = s.first - a.first;
    asr[2] = r.first - s.first;

    float amp = a.second.first;

    bool changed = false;
    if (ImGui::DragFloat3("Durations", asr, 0.001f, minDur, 1.0f, "%0.3f s"))
        changed = true;
    if (ImGui::DragFloat("Amplitude", &amp, 0.001f, 0, 1))
        changed = true;

    for (int i = 0; i < 3; ++i)
    {
        if (asr[i] <= 0)
            asr[i] = minDur;
    }

    if (changed)
        sig = tact::ASR(asr[0], asr[1], asr[2], amp);
}

///////////////////////////////////////////////////////////////////////////////

void ADSRNode::update()
{
    static const float minDur = 0.001f;

    auto cast = (tact::ASR *)sig.get();
    auto it = cast->keys.begin();
    it++;
    auto &a = *(it++);
    auto &d = *(it++);
    auto &s = *(it++);
    auto &r = *(it++);
    float adsr[4];
    adsr[0] = a.first;
    adsr[1] = d.first - a.first;
    adsr[2] = s.first - d.first;
    adsr[3] = r.first - s.first;
    float amp[2];
    amp[0] = a.second.first;
    amp[1] = d.second.first;
    bool changed = false;
    if (ImGui::DragFloat4("Durations", adsr, 0.001f, minDur, 1, "%0.3f s"))
        changed = true;
    if (ImGui::DragFloat2("Amplitudes", amp, 0.001f, 0, 1))
        changed = true;

    for (int i = 0; i < 4; ++i)
    {
        if (adsr[i] <= 0)
            adsr[i] = minDur;
    }

    if (changed)
        sig = tact::ADSR(adsr[0], adsr[1], adsr[2], adsr[3], amp[0], amp[1]);
}

void ExponentialDecayNode::update() {
    bool changed = false;
    auto cast = (tact::ExponentialDecay*)sig.get();
    float amplitude = (float)cast->amplitude;
    float decay     = (float)cast->decay;
    if (ImGui::DragFloat("Amplitude", &amplitude, 0.001, 0, 1))
        changed = true;
    if (ImGui::DragFloat("Decay", &decay, 0.01, 0.000001, 100))
        changed = true;
    if (changed)
        sig = tact::ExponentialDecay(amplitude, decay);
}

///////////////////////////////////////////////////////////////////////////////

void SignalEnvelopeNode::update()
{
    auto cast = sig.getAs<tact::SignalEnvelope>();
    root->update();
    cast->signal = root->signal();
    ImGui::DragDouble("Duration", &cast->duration, 0.001f, 0, 1, "%0.3f s");
    ImGui::DragDouble("Amplitude", &cast->amplitude, 0.01f, 0, 1);
}

///////////////////////////////////////////////////////////////////////////////

void NoiseNode::update()
{
    float gain = (float)sig.gain;
    float bias = (float)sig.bias;
    ImGui::DragFloat("Gain", &gain, 0.001f, -1, 1, "%0.3f");
    ImGui::DragFloat("Bias", &bias, 0.001f, -1, 1, "%0.3f");
    sig.gain = gain;
    sig.bias = bias;
}

///////////////////////////////////////////////////////////////////////////////

void TimeNode::update() {}

///////////////////////////////////////////////////////////////////////////////

void ScalarNode::update()
{
    auto cast = (tact::Scalar *)sig.get();
    float value = (float)cast->value;
    ImGui::DragFloat("Value", &value, 0.01f, -10, 10);
    cast->value = value;
}

///////////////////////////////////////////////////////////////////////////////

void RampNode::update()
{
    auto cast = (tact::Ramp *)sig.get();
    float initial = (float)cast->initial;
    float rate = (float)cast->rate;
    ImGui::DragFloat("Initial", &initial, 0.01f, -10, 10);
    ImGui::DragFloat("Rate", &rate, 0.01f, -10, 10);
    cast->initial = initial;
    cast->rate = rate;
}

///////////////////////////////////////////////////////////////////////////////

