#include "dll.hpp"
#include <syntacts>
#include <unordered_map>
#include <iostream>

using namespace tact;

std::unordered_map<void*, Signal> g_sigs;

template <typename S>
inline Handle store(const S& s) {
    Signal sig(s);
    Handle h = sig.get();
    g_sigs.emplace(h, std::move(sig));
    return h;
}

///////////////////////////////////////////////////////////////////////////////

Handle Session_create() {
    Session* session = new Session();
    return static_cast<void*>(session);
}

void Session_delete(Handle session) {
    delete static_cast<Session*>(session);
}

int Session_open1(Handle session) {
    return static_cast<Session*>(session)->open();
}

int Session_open2(Handle session, int index) {
    return static_cast<Session*>(session)->open(index);
}

int Session_open3(Handle session, int index, int channelCount, double sampleRate) {
    return static_cast<Session*>(session)->open(index, channelCount, sampleRate);
}

int Session_close(Handle session) {
    return static_cast<Session*>(session)->close();
}

bool Session_isOpen(Handle session) {
    return static_cast<Session*>(session)->isOpen();
}

int Session_play(Handle session, int channel, Handle signal) {
    return static_cast<Session*>(session)->play(channel, g_sigs.at(signal));
}

int Session_playAll(Handle session, Handle signal) {
    return static_cast<Session*>(session)->playAll(g_sigs.at(signal));
}

int Session_stop(Handle session, int channel) {
    return static_cast<Session*>(session)->stop(channel);
}

int Session_stopAll(Handle session) {
    return static_cast<Session*>(session)->stopAll();
}

int Session_pause(Handle session, int channel) {
    return static_cast<Session*>(session)->pause(channel);
}

int Session_pauseAll(Handle session) {
    return static_cast<Session*>(session)->pauseAll();
}

int Session_resume(Handle session, int channel) {
    return static_cast<Session*>(session)->resume(channel);
}

int Session_resumeAll(Handle session) {
    return static_cast<Session*>(session)->resumeAll();
}

bool Session_isPlaying(Handle session, int channel) {
    return static_cast<Session*>(session)->isPlaying(channel);
}

bool Session_isPaused(Handle session, int channel) {
    return static_cast<Session*>(session)->isPaused(channel);
}

int Session_setVolume(Handle session, int channel, double volume) {
    return static_cast<Session*>(session)->setVolume(channel, volume);
}

double Session_getVolume(Handle session, int channel) {
    return static_cast<Session*>(session)->getVolume(channel);
}

int Session_setPitch(Handle session, int channel, double pitch) {
    return static_cast<Session*>(session)->setPitch(channel, pitch);
}

double Session_getPitch(Handle session, int channel) {
    return static_cast<Session*>(session)->getPitch(channel);
}

int Session_getChannelCount(Handle session) {
    return static_cast<Session*>(session)->getChannelCount();
}

double Session_getSampleRate(Handle session) {
    return static_cast<Session*>(session)->getSampleRate();
}

double Session_getCpuLoad(Handle session) {
    return static_cast<Session*>(session)->getCpuLoad();
}


int Session_count() {
    return Session::count();
}

///////////////////////////////////////////////////////////////////////////////

bool Signal_valid(Handle signal) {
    return g_sigs.count(signal) > 0;
}

void Signal_delete(Handle signal) {
    g_sigs.erase(signal);
}

double Signal_sample(Handle signal, double t) {
    return g_sigs.at(signal).sample(t);
}

double Signal_length(Handle signal) {
    return g_sigs.at(signal).length();
}

void Signal_setGain(Handle signal, double gain) {
    g_sigs.at(signal).gain = gain;
}

double Signal_getGain(Handle signal) {
    return g_sigs.at(signal).gain;
}

void Signal_setBias(Handle signal, double bias) {
    g_sigs.at(signal).bias = bias;
}

double Signal_getBias(Handle signal) {
    return g_sigs.at(signal).bias;
}


int Signal_count() {
    return Signal::count();
}

///////////////////////////////////////////////////////////////////////////////

Handle Product_create(Handle lhs, Handle rhs) {
    return store(Product(g_sigs.at(lhs), g_sigs.at(rhs)));
}

Handle Sum_create(Handle lhs, Handle rhs) {
    return store(Sum(g_sigs.at(lhs), g_sigs.at(rhs)));
}

Handle Mul_SigFlt(Handle lhs, double rhs) {
    return store(g_sigs.at(lhs) * rhs);
}

Handle Mul_FltSig(double lhs, Handle rhs) {
    return store(lhs * g_sigs.at(rhs));
}

Handle Add_SigFlt(Handle lhs, double rhs) {
    return store(g_sigs.at(lhs) + rhs);
}

Handle Add_FltSig(double lhs, Handle rhs) {
    return store(lhs + g_sigs.at(rhs));
}

Handle Sub_SigFlt(Handle lhs, double rhs) {
    return store(g_sigs.at(lhs) - rhs);
}

Handle Sub_FltSig(double lhs, Handle rhs) {
    return store(lhs - g_sigs.at(rhs));
}

Handle Neg_Sig(Handle signal) {
    return store(-g_sigs.at(signal));
}

///////////////////////////////////////////////////////////////////////////////

Handle Time_create() {
    return store(Time());
}

Handle Scalar_create(double value) {
    return store(Scalar(value));
}

Handle Ramp_create(double initial, double rate) {
    return store(Ramp(initial, rate));
}

Handle Noise_create() {
    return store(Noise());
}

Handle Expression_create(const char* expr) {
    return store(Expression(expr));
}

Handle Samples_create(float* samples, int nSamples, double sampleRate) {
    std::vector<float> vsamples(samples, samples + nSamples);
    return store(Samples(vsamples, sampleRate));
}

///////////////////////////////////////////////////////////////////////////////

Handle Repeater_create(Handle signal, int repetitions, double delay) {
    return store(Repeater(g_sigs.at(signal), repetitions, delay));
}

Handle Stretcher_create(Handle signal, double factor) {
    return store(Stretcher(g_sigs.at(signal), factor));
}

Handle Reverser_create(Handle signal) {
    return store(Reverser(g_sigs.at(signal)));
}

///////////////////////////////////////////////////////////////////////////////

Handle Envelope_create(double duration, double amp) {
    return store(Envelope(duration, amp));
}

Handle ASR_create(double a, double s, double r, double amp) {
    return store(ASR(a,s,r,amp));
}

Handle ADSR_create(double a, double d, double s, double r, double amp1, double amp2) {
    return store(ADSR(a,d,s,r, amp1, amp2));
}

///////////////////////////////////////////////////////////////////////////////

Handle Sine_create1(Handle x) {
    return store(Sine(g_sigs.at(x)));
}

Handle Sine_create2(double hertz) {
    return store(Sine(hertz));
}

Handle Sine_create3(double initial, double rate) {
    return store(Sine(initial, rate));
}

Handle Sine_create4(double hertz, Handle modulation, double index) {
    return store(Sine(hertz, g_sigs.at(modulation), index));
}

Handle Square_create1(Handle x) {
    return store(Square(g_sigs.at(x)));
}

Handle Square_create2(double hertz) {
    return store(Square(hertz));
}

Handle Square_create3(double initial, double rate) {
    return store(Square(initial, rate));
}

Handle Square_create4(double hertz, Handle modulation, double index) {
    return store(Square(hertz, g_sigs.at(modulation), index));
}

Handle Saw_create1(Handle x) {
    return store(Saw(g_sigs.at(x)));
}

Handle Saw_create2(double hertz) {
    return store(Saw(hertz));
}

Handle Saw_create3(double initial, double rate) {
    return store(Saw(initial, rate));
}

Handle Saw_create4(double hertz, Handle modulation, double index) {
    return store(Saw(hertz, g_sigs.at(modulation), index));
}

Handle Triangle_create1(Handle x) {
    return store(Triangle(g_sigs.at(x)));
}

Handle Triangle_create2(double hertz) {
    return store(Triangle(hertz));
}

Handle Triangle_create3(double initial, double rate) {
    return store(Triangle(initial, rate));
}

Handle Triangle_create4(double hertz, Handle modulation, double index) {
    return store(Triangle(hertz, g_sigs.at(modulation), index));
}

Handle Pwm_create(double frequency, double dutyCycle) {
    return store(Pwm(frequency, dutyCycle));
}

///////////////////////////////////////////////////////////////////////////////

Handle Sequence_create() {
    return store(Sequence());
}

Handle Sequence_SigSig(Handle lhs, Handle rhs) {
    auto s = g_sigs.at(lhs) << g_sigs.at(rhs);
    return store(s);
}

Handle Sequence_SigFlt(Handle lhs, double rhs) {
    auto s = g_sigs.at(lhs) << rhs;
    return store(s);
}

Handle Sequence_FltSig(double lhs, Handle rhs) {
    auto s = lhs << g_sigs.at(rhs);
    return store(s);
}

void Sequence_SeqFlt(Handle lhs, double rhs) {
    Sequence& s = *(Sequence*)g_sigs.at(lhs).get();
    s << rhs;
}

void Sequence_SeqSig(Handle lhs, Handle rhs) {
    Sequence& s = *(Sequence*)g_sigs.at(lhs).get();
    s << g_sigs.at(rhs);
}

void Sequence_SeqSeq(Handle lhs, Handle rhs) {
    Sequence& s1 = *(Sequence*)g_sigs.at(lhs).get();
    Sequence& s2 = *(Sequence*)g_sigs.at(rhs).get();
    s1 << s2;
}

///////////////////////////////////////////////////////////////////////////////

bool Library_saveSignal(Handle signal, const char* name) {
    return Library::saveSignal(g_sigs.at(signal), name);
} 

Handle Library_loadSignal(const char* name) {
    Signal sig;
    if (Library::loadSignal(sig, name)) {
        return store(sig);
    }
    else {
        return nullptr;
    }
}

///////////////////////////////////////////////////////////////////////////////

int Debug_sigMapSize() {
    return static_cast<int>(g_sigs.size());
}

///////////////////////////////////////////////////////////////////////////////
