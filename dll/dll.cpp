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

int Session_open(Handle session, int index, int channelCount, double sampleRate) {
    return static_cast<Session*>(session)->open(index, channelCount, sampleRate);
}

int Session_close(Handle session) {
    return static_cast<Session*>(session)->close();
}

int Session_play(Handle session, int channel, Handle signal) {
    return static_cast<Session*>(session)->play(channel, g_sigs.at(signal));
}

int Session_stop(Handle session, int channel) {
    return static_cast<Session*>(session)->stop(channel);
}

int Session_pause(Handle session, int channel) {
    return static_cast<Session*>(session)->pause(channel);
}

int Session_resume(Handle session, int channel) {
    return static_cast<Session*>(session)->resume(channel);
}

int Session_setVolume(Handle session, int channel, float volume) {
    return static_cast<Session*>(session)->setVolume(channel, volume);
}

int Session_setPitch(Handle session, int channel, float pitch) {
    return static_cast<Session*>(session)->setPitch(channel, pitch);
}

bool Session_isOpen(Handle session) {
    return static_cast<Session*>(session)->isOpen();
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

float Signal_sample(Handle signal, float t) {
    return g_sigs.at(signal).sample(t);
}

float Signal_length(Handle signal) {
    return g_sigs.at(signal).length();
}

int Signal_count() {
    return Signal::count();
}

///////////////////////////////////////////////////////////////////////////////

Handle Scalar_create(float value) {
    return store(Scalar(value));
}

Handle Ramp_create(float initial, float rate) {
    return store(Ramp(initial, rate));
}

///////////////////////////////////////////////////////////////////////////////


Handle Envelope_create(float duration) {
    return store(Envelope(duration));
}

Handle ASR_create(float a, float s, float r) {
    return store(ASR(a,s,r));
}

Handle ADSR_create(float a, float d, float s, float r) {
    return store(ADSR(a,d,s,r));
}

///////////////////////////////////////////////////////////////////////////////

Handle Sine_create(float frequency) {
    return store(Sine(frequency));
}

Handle Square_create(float frequency) {
    return store(Square(frequency));
}

Handle Saw_create(float frequency) {
    return store(Saw(frequency));
}

Handle Triangle_create(float frequency) {
    return store(Triangle(frequency));
}

///////////////////////////////////////////////////////////////////////////////

Handle Product_create(Handle lhs, Handle rhs) {
    return store(Product(g_sigs.at(lhs), g_sigs.at(rhs)));
}

Handle Sum_create(Handle lhs, Handle rhs) {
    return store(Sum(g_sigs.at(lhs), g_sigs.at(rhs)));
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
