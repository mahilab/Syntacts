#include <syntacts>
#include <thread>
#include <chrono>

namespace tact {

namespace {
std::chrono::time_point<std::chrono::high_resolution_clock> g_tic, g_toc;
}

void sleep(double seconds, double max) {
    seconds = std::abs(seconds);
    max = std::abs(max);
    if (seconds != INF)
        std::this_thread::sleep_for(std::chrono::nanoseconds((std::size_t)(seconds * 1000000000)));
    else
        std::this_thread::sleep_for(std::chrono::nanoseconds((std::size_t)(max * 1000000000)));
}


void tic() {
    g_tic = std::chrono::high_resolution_clock::now();
}

double toc() {
    g_toc = std::chrono::high_resolution_clock::now();
    auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(g_toc - g_tic).count();
    return ns / 1000000000.0;
}

const std::string& signalName(const Signal& signal) {
    auto id =  signal.typeId();
    static std::string unkown = "Unkown";
    static std::unordered_map<std::type_index, std::string> names = {
        // General.hpp
        {typeid(Scalar),           "Scalar"},
        {typeid(Time),             "Time"},
        {typeid(Ramp),             "Ramp"},
        {typeid(Noise),            "Noise"},
        {typeid(Expression),       "Expression"},
        {typeid(PolyBezier),       "PolyBezier"},
        {typeid(Samples),          "Samples"},
        // Operator.hpp
        {typeid(Sum),              "Sum"},
        {typeid(Product),          "Product"},
        // Sequence.hpp  
        {typeid(Sequence),         "Sequence"},
        // Oscillator.hpp
        {typeid(Sine),             "Sine"},
        {typeid(Square),           "Square"},
        {typeid(Saw),              "Saw"},
        {typeid(Triangle),         "Triangle"},
        {typeid(Pwm),              "PWM"},
        // Envelope.hpp
        {typeid(Envelope),         "Envelope"},
        {typeid(KeyedEnvelope),    "Keyed Envelope"},
        {typeid(ASR),              "ASR"},
        {typeid(ADSR),             "ADSR"},
        {typeid(ExponentialDecay), "Exponential Decay"},
        {typeid(SignalEnvelope),   "Signal Envelope"},
        // Process.hpp
        {typeid(Repeater),         "Repeater"},
        {typeid(Stretcher),        "Stretcher"},
        {typeid(Reverser),         "Reverser"}};
    if (names.count(id))
        return names[id];
    else
        return unkown;
}

void recurseSignalPriv(const Signal& sig, std::function<void(const Signal&, int depth)> func, int depth) {
    auto id = sig.typeId();
    func(sig, depth);
    if (id == typeid(Sum)) {
        recurseSignalPriv(sig.getAs<Sum>()->lhs,func,depth+1);
        recurseSignalPriv(sig.getAs<Sum>()->rhs,func,depth+1);
    }
    else if (id == typeid(Product)) {
        recurseSignalPriv(sig.getAs<Product>()->lhs,func,depth+1);
        recurseSignalPriv(sig.getAs<Product>()->rhs,func,depth+1);
    }
    else if (id == typeid(Sequence)) {
        auto seq = sig.getAs<Sequence>();
        int K = seq->keyCount();
        for (int k = 0; k < K; ++k)
            recurseSignalPriv(seq->getKey(k).signal,func,depth+1);
    }
    else if (id == typeid(Repeater))
        recurseSignalPriv(sig.getAs<Repeater>()->signal,func,depth+1);
    else if (id == typeid(Stretcher))
        recurseSignalPriv(sig.getAs<Stretcher>()->signal,func,depth+1);
    else if (id == typeid(Reverser))
        recurseSignalPriv(sig.getAs<Reverser>()->signal,func,depth+1);   
    else if (id == typeid(Sine))
         recurseSignalPriv(sig.getAs<Sine>()->x,func,depth+1);
    else if (id == typeid(Square))
         recurseSignalPriv(sig.getAs<Square>()->x,func,depth+1);
    else if (id == typeid(Saw))
         recurseSignalPriv(sig.getAs<Saw>()->x,func,depth+1);
    else if (id == typeid(Triangle))
         recurseSignalPriv(sig.getAs<Triangle>()->x,func,depth+1);
    else if (id == typeid(SignalEnvelope))
         recurseSignalPriv(sig.getAs<SignalEnvelope>()->signal,func,depth+1);
}

/// Recurse a signal for embedded signals and calls func on each
void recurseSignal(const Signal& signal, std::function<void(const Signal&, int depth)> func) {
    recurseSignalPriv(signal, func, 0);
}

const std::string& syntactsVersion() {
    static std::string ver = std::to_string(SYNTACTS_VERSION_MAJOR) + "."
                           + std::to_string(SYNTACTS_VERSION_MINOR) + "."
                           + std::to_string(SYNTACTS_VERSION_PATCH);
    return ver;
}

};