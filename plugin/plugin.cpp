#include "plugin.hpp"
#include <Syntacts/Syntacts.hpp>
#include <Syntacts/Session.hpp>
#include <unordered_map>

using namespace tact;

std::unordered_map<void*, Ptr<Signal>>     g_sigs;
std::unordered_map<void*, Ptr<Oscillator>> g_oscs;
std::unordered_map<void*, Ptr<Envelope>>   g_envs; 
std::unordered_map<void*, Ptr<Cue>>        g_cues;

template <typename P, typename M>
inline Handle store(P p, M& m) {
    Handle hand = p.get(); 
    m[hand] = move(p);
    return hand;
}

inline Ptr<Signal> findSignal(Handle sig) {
    if (g_sigs.count(sig))
        return g_sigs[sig];
    else if (g_oscs.count(sig))
        return g_oscs[sig];
    else if (g_envs.count(sig))
        return g_envs[sig];
    else if (g_cues.count(sig))
        return g_cues[sig];
    else
        return nullptr;
}

//=============================================================================

Handle Session_create() {
    Session* session = new Session();
    return static_cast<void*>(session);
}

void Session_delete(Handle session) {
    delete static_cast<Session*>(session);
}

int Session_open(Handle session, int index, int channelCount) {
    return static_cast<Session*>(session)->open(index, channelCount);
}

int Session_close(Handle session) {
    return static_cast<Session*>(session)->close();
}

int Session_play(Handle session, int channel, Handle cue) {
    return static_cast<Session*>(session)->play(channel, g_cues.at(cue));
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

//=============================================================================

bool Signal_valid(Handle sig) {
    return findSignal(sig) != nullptr ? true : false;
}

void Signal_delete(Handle sig) {
    g_sigs.erase(sig);
    g_oscs.erase(sig);
    g_envs.erase(sig);
    g_cues.erase(sig);
}

float Signal_sample(Handle sig, float t) {
    if (auto s = findSignal(sig))
        return s->sample(t);
    else
        return std::numeric_limits<float>::infinity();
}

int Signal_count() {
    return Signal::count();
}

//=============================================================================

Handle Scalar_create(float value) {
    return store(create<Scalar>(value), g_sigs);
}

Handle Ramp_create(float initial, float rate) {
    return store(create<Ramp>(initial, rate), g_sigs);
}

//=============================================================================


Handle Envelope_create(float duration) {
    return store(create<Envelope>(duration), g_envs);
}

Handle ASR_create(float a, float s, float r) {
    return store(create<ASR>(a,s,r), g_envs);
}

Handle ADSR_create(float a, float d, float s, float r) {
    return store(create<ADSR>(a,d,s,r), g_envs);
}

//=============================================================================

Handle Sine_create(float frequency) {
    return store(create<Sine>(frequency), g_oscs);
}

Handle Square_create(float frequency) {
    return store(create<Square>(frequency), g_oscs);
}

Handle Saw_create(float frequency) {
    return store(create<Saw>(frequency), g_oscs);
}

Handle Triangle_create(float frequency) {
    return store(create<Triangle>(frequency), g_oscs);
}

//=============================================================================

Handle Cue_create() {
    return store(create<Cue>(), g_cues);
}

void Cue_setEnvelope(Handle cue, Handle env) {
    g_cues.at(cue)->setEnvelope(g_envs.at(env));
}

void Cue_chain(Handle cue, Handle sig) {
    if (auto s = findSignal(sig))
        g_cues.at(cue)->chain(s);
}

//=============================================================================

bool Library_saveCue(Handle cue, const char* name) {
    Library::saveCue(g_cues.at(cue), name);
} 

Handle Library_loadCue(const char* name) {
    auto cue = Library::loadCue(name);
    if (cue != nullptr)
        return store(cue, g_cues);
    else
        return nullptr;
}

//=============================================================================

int Debug_oscMapSize() {
    return static_cast<int>(g_oscs.size());
}

int Debug_envMapSize() {
    return static_cast<int>(g_envs.size());
}

int Debug_cueMapSize() {
    return static_cast<int>(g_cues.size());
}

//=============================================================================