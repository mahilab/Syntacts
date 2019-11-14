#include "plugin.hpp"
#include <Syntacts/Syntacts.hpp>
#include <Syntacts/Session.hpp>
#include <unordered_map>

using namespace tact;

std::unordered_map<void*, Ptr<Oscillator>> g_oscs;
std::unordered_map<void*, Ptr<Envelope>>   g_envs; 
std::unordered_map<void*, Ptr<Cue>>        g_cues;

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

int Signal_count() {
    return Signal::count();
}

Handle Envelope_create(float duration) {
    auto env = create<Envelope>(duration);
    Handle hand = env.get();
    g_envs[hand] = std::move(env);
    return hand;
}

void Envelope_delete(Handle env) {
    g_envs.erase(env);
}

Handle ASR_create(float a, float s, float r) {
    auto env = create<ASR>(a,s,r);
    Handle hand = env.get();
    g_envs[hand] = std::move(env);
    return hand;
}

Handle ADSR_create(float a, float d, float s, float r) {
    auto env = create<ADSR>(a,d,s,r);
    Handle hand = env.get();
    g_envs[hand] = std::move(env);
    return hand;
}


void Oscillator_delete(Handle osc) {
    g_oscs.erase(osc);
}

Handle Sine_create(float frequency) {
    auto osc = create<Sine>(frequency);
    Handle hand = osc.get();
    g_oscs[hand] = std::move(osc);
    return hand;
}

Handle Square_create(float frequency) {
    auto osc = create<Square>(frequency);
    Handle hand = osc.get();
    g_oscs[hand] = std::move(osc);
    return hand;
}

Handle Cue_create(Handle osc, Handle env) {
    auto cue = create<Cue>(g_oscs.at(osc), g_envs.at(env));
    Handle hand = cue.get();
    g_cues[hand] = std::move(cue);
    return hand;
}

void Cue_delete(Handle cue) {
    g_cues.erase(cue);
}

int Debug_oscMapSize() {
    return static_cast<int>(g_oscs.size());
}

int Debug_envMapSize() {
    return static_cast<int>(g_envs.size());
}

int Debug_cueMapSize() {
    return static_cast<int>(g_cues.size());
}
