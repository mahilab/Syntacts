#include "Nodes.hpp"
#include "Gui.hpp"

using namespace mahi::gui;

///////////////////////////////////////////////////////////////////////////////

void NodeSlot(const char *label, const ImVec2 &size)
{
    BeginPulsable(true,true);
    ImGui::Button(label, size);
    EndPulsable();
}

/// Returns the name of a Syntacts signal
const std::string &signalName(std::type_index id)
{
    static std::string unkown = "Unkown";
    static std::unordered_map<std::type_index, std::string> names = {
        // {typeid(tact::Zero),           "Zero"},
        {typeid(tact::Time), "Time"},
        {typeid(tact::Scalar), "Scalar"},
        {typeid(tact::Ramp), "Ramp"},
        {typeid(tact::Noise), "Noise"},
        {typeid(tact::Expression), "Expression"},
        {typeid(tact::Sum), "Sum"},
        {typeid(tact::Product), "Product"},
        {typeid(tact::Sine), "Sine"},
        {typeid(tact::Square), "Square"},
        {typeid(tact::Saw), "Saw"},
        {typeid(tact::Triangle), "Triangle"},
        {typeid(tact::Pwm), "PWM"},
        {typeid(tact::Envelope), "Envelope"},
        {typeid(tact::ASR), "ASR"},
        {typeid(tact::ADSR), "ADSR"},
        {typeid(tact::KeyedEnvelope), "Keyed Envelope"},
        {typeid(tact::SignalEnvelope), "Signal Envelope"},
        {typeid(tact::Stretcher), "Stretcher"},
        {typeid(tact::Repeater), "Repeater"},
        {typeid(tact::Reverser), "Reverser"},
        {typeid(tact::PolyBezier), "PolyBezier"}};
    if (names.count(id))
        return names[id];
    else
        return unkown;
}

const std::string &signalName(const tact::Signal &sig)
{
    return signalName(sig.typeId());
}

std::shared_ptr<Node> makeNode(PItem id)
{
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
    if (id == PItem::ASR)
        return std::make_shared<ASRNode>();
    if (id == PItem::ADSR)
        return std::make_shared<ADSRNode>();
    if (id == PItem::PolyBezier)
        return std::make_shared<PolyBezierNode>();
    if (id == PItem::Stretcher)
        return std::make_shared<StretcherNode>();
    if (id == PItem::Repeater)
        return std::make_shared<RepeaterNode>();
    if (id == PItem::Reverser)
        return std::make_shared<ReverserNode>();
    if (id == PItem::Pwm)
        return std::make_shared<PwmNode>();
    if (id == PItem::FM)
        return std::make_shared<FmNode>();
    // static auto gui = Engine::getRoot().as<Gui>();
    // gui.status->pushMessage("Failed to create Node!", StatusBar::Error);
    return nullptr;
}

///////////////////////////////////////////////////////////////////////////////

void NodeList::gui()
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
    // node slot
    if (m_nodes.size() == 0 || SignalHeld() || PaletteHeld())
        NodeSlot("##EmpySlot", ImVec2(-1, 0));
    // check for incomming palette items
    if (PaletteTarget())
    {
        auto node = makeNode(PalettePayload());
        if (node)
        {
            m_nodes.emplace_back(node);
            m_closeHandles.emplace_back(true);
            m_ids.push_back(m_nextId++);
        }
    }
    // check for incomming library items
    if (SignalTarget())
    {
        auto node = std::make_shared<LibrarySignalNode>(SignalPayload().first);
        m_nodes.emplace_back(node);
        m_closeHandles.emplace_back(true);
        m_ids.push_back(m_nextId++);
    }
    // clean up
    std::vector<std::shared_ptr<Node>> newNodes;
    std::vector<int> newIds;
    for (int i = 0; i < m_nodes.size(); ++i)
    {
        if (m_closeHandles[i])
        {
            newNodes.emplace_back(std::move(m_nodes[i]));
            newIds.push_back(m_ids[i]);
        }
    }
    m_nodes = std::move(newNodes);
    m_ids = std::move(newIds);
    m_closeHandles = std::deque<bool>(m_nodes.size(), true);
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
void LibrarySignalNode::gui()
{
}

///////////////////////////////////////////////////////////////////////////////

void StretcherNode::gui()
{
    auto cast = (tact::Stretcher *)sig.get();
    NodeSlot(m_sigName.c_str(), ImVec2(ImGui::CalcItemWidth(), 0));
    if (SignalTarget())
    {
        m_sigName = SignalPayload().first;
        cast->signal = SignalPayload().second;
    }
    ImGui::SameLine();
    ImGui::Text("Signal");
    float factor = (float)cast->factor;
    if (ImGui::SliderFloat("Factor", &factor, 0, 10))
        cast->factor = factor;
}

///////////////////////////////////////////////////////////////////////////////

void RepeaterNode::gui()
{
    auto cast = (tact::Repeater *)sig.get();
    NodeSlot(m_sigName.c_str(), ImVec2(ImGui::CalcItemWidth(), 0));
    if (SignalTarget())
    {
        m_sigName = SignalPayload().first;
        cast->signal = SignalPayload().second;
    }
    ImGui::SameLine();
    ImGui::Text("Signal");
    ImGui::SliderInt("Repetitions", &cast->repetitions, 1, 100);
    float delay = (float)cast->delay;
    if (ImGui::SliderFloat("Delay", &delay, 0, 1))
        cast->delay = delay;
}

///////////////////////////////////////////////////////////////////////////////

void ReverserNode::gui()
{
    auto cast = (tact::Repeater *)sig.get();
    NodeSlot(m_sigName.c_str(), ImVec2(ImGui::CalcItemWidth(), 0));
    if (SignalTarget())
    {
        m_sigName = SignalPayload().first;
        cast->signal = SignalPayload().second;
    }
    ImGui::SameLine();
    ImGui::Text("Signal");
}


///////////////////////////////////////////////////////////////////////////////

OscillatorNode::OscillatorNode(tact::Signal osc)
{
    sig = std::move(osc);
}

void OscillatorNode::gui()
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
        BeginPulsable(true,true);
        ImGui::DragFloat("##Frequency", &f, 1, 0, 1000, "%.0f Hz");
        EndPulsable();
        if (PaletteTarget())
        {
        }
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

void ChirpNode::gui()
{
    for (int i = 0; i < 4; ++i) {
        if (ImGui::RadioButton(oscName(i).c_str(), ftype == i))
            ftype = i;
        if (i != 3)
            ImGui::SameLine();
    }
    ImGui::DragFloat("Frequency", &f, 1, 0, 1000, "%.0f Hz");
    ImGui::DragFloat("Rate", &r, 1, 0, 1000, "%.0f Hz/s");
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

void FmNode::gui()
{
    for (int i = 0; i < 4; ++i) {
        if (ImGui::RadioButton(oscName(i).c_str(), ftype == i))
            ftype = i;
        if (i != 3)
            ImGui::SameLine();
    }
    ImGui::DragFloat("Frequency", &f, 1, 0, 1000, "%.0f Hz");
    ImGui::DragFloat("Modulation", &m, 0.1f, 0, 100, "%.0f Hz");
    ImGui::DragFloat("Index", &index, 0.01f, 0, 10);
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

void PwmNode::gui()
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
    auto cast = (tact::Expression *)sig.get();
    strcpy(buffer, cast->getExpression().c_str());
}

void ExpressionNode::gui()
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

PolyBezierNode::PolyBezierNode() : pb(Blues::DeepSkyBlue, ImVec2(0, 0), ImVec2(1, 1)) {
    update();
}

void PolyBezierNode::gui()
{
    ImGui::PushItemWidth(-1);

    ImGui::PolyBezierEdit("PolyBezier", &pb, 10, 10, ImVec2(-1, 125));
    ImGui::PushItemWidth(-1);
    ImGui::DragFloat4("##Bounds", bounds, 0.01f, -10, 10);
    pb.min.x = bounds[0];
    pb.min.y = bounds[1];
    pb.max.x = bounds[2];
    pb.max.y = bounds[3];
    ImGui::PopItemWidth();
    update();
}

void PolyBezierNode::update() {
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


///////////////////////////////////////////////////////////////////////////////

void EnvelopeNode::gui()
{
    auto cast = (tact::Envelope *)sig.get();
    float duration = (float)cast->duration;
    float amplitude = (float)cast->amplitude;
    ImGui::DragFloat("Duration", &duration, 0.001f, 0, 1, "%0.3f s");
    ImGui::DragFloat("Amplitude", &amplitude, 0.01f, 0, 1);
    cast->amplitude = amplitude;
    cast->duration = duration;
}

///////////////////////////////////////////////////////////////////////////////

void ASRNode::gui()
{
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
    if (ImGui::DragFloat3("Durations", asr, 0.001f, 0.0001f, 1.0f, "%0.3f s"))
        changed = true;
    if (ImGui::DragFloat("Amplitude", &amp, 0.001f, 0, 1))
        changed = true;

    if (changed)
        sig = tact::ASR(asr[0], asr[1], asr[2], amp);
}

///////////////////////////////////////////////////////////////////////////////

void ADSRNode::gui()
{
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
    if (ImGui::DragFloat4("Durations", adsr, 0.001f, 0.0001f, 1, "%0.3f s"))
        changed = true;
    if (ImGui::DragFloat2("Amplitudes", amp, 0.001f, 0, 1))
        changed = true;
    if (changed)
        sig = tact::ADSR(adsr[0], adsr[1], adsr[2], adsr[3], amp[0], amp[1]);
}

///////////////////////////////////////////////////////////////////////////////

void NoiseNode::gui()
{
    float gain = (float)sig.gain;
    float bias = (float)sig.bias;
    ImGui::DragFloat("Gain", &gain, 0.001f, -1, 1, "%0.3f");
    ImGui::DragFloat("Bias", &bias, 0.001f, -1, 1, "%0.3f");
    sig.gain = gain;
    sig.bias = bias;
}

///////////////////////////////////////////////////////////////////////////////

void TimeNode::gui() {}

///////////////////////////////////////////////////////////////////////////////

void ScalarNode::gui()
{
    auto cast = (tact::Scalar *)sig.get();
    float value = (float)cast->value;
    ImGui::DragFloat("Value", &value, 0.01f, -10, 10);
    cast->value = value;
}

///////////////////////////////////////////////////////////////////////////////

void RampNode::gui()
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

