#pragma once

#define EXPORT extern "C" __declspec(dllexport)

typedef void* Handle;

//=============================================================================

EXPORT Handle Session_create();
EXPORT void Session_delete(Handle session);
EXPORT int Session_open(Handle session, int index, int channelCount);
EXPORT int Session_close(Handle session);
EXPORT int Session_play(Handle session, int channel, Handle cue);
EXPORT int Session_stop(Handle session, int channel);
EXPORT int Session_pause(Handle session, int channel);
EXPORT int Session_resume(Handle session, int channel);
EXPORT int Session_setVolume(Handle session, int channel, float volume);

//=============================================================================

EXPORT bool Signal_valid(Handle sig);
EXPORT void Signal_delete(Handle sig);
EXPORT float Signal_sample(Handle sig, float t);
EXPORT int Signal_count();

//=============================================================================

EXPORT Handle Scalar_create(float value);
EXPORT Handle Ramp_create(float initial, float rate);

//=============================================================================

EXPORT Handle Envelope_create(float duration);
EXPORT Handle ASR_create(float a, float s, float r);
EXPORT Handle ADSR_create(float a, float d, float s, float r);

//=============================================================================

EXPORT Handle Sine_create(float frequency);
EXPORT Handle Square_create(float frequency);
EXPORT Handle Saw_create(float frequency);
EXPORT Handle Triangle_create(float frequency);

//=============================================================================

EXPORT Handle Cue_create();
EXPORT void Cue_setEnvelope(Handle cue, Handle env);
EXPORT void Cue_chain(Handle cue, Handle sig);

//=============================================================================

EXPORT bool Library_saveCue(Handle cue, const char* name);
EXPORT Handle Library_loadCue(const char* name);

//=============================================================================

EXPORT int Debug_oscMapSize();
EXPORT int Debug_envMapSize();
EXPORT int Debug_cueMapSize();

//=============================================================================
