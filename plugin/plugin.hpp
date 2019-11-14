#pragma once

#define EXPORT extern "C" __declspec(dllexport)

typedef void *Handle;

EXPORT Handle Session_create();
EXPORT void Session_delete(Handle session);
EXPORT int Session_open(Handle session, int index, int channelCount);
EXPORT int Session_close(Handle session);
EXPORT int Session_play(Handle session, int channel, Handle cue);
EXPORT int Session_stop(Handle session, int channel);
EXPORT int Session_pause(Handle session, int channel);
EXPORT int Session_resume(Handle session, int channel);

EXPORT int Signal_count();

EXPORT Handle Envelope_create(float duration);
EXPORT void Envelope_delete(Handle env);
EXPORT Handle ASR_create(float a, float s, float r);
EXPORT Handle ADSR_create(float a, float d, float s, float r);

EXPORT void Oscillator_delete(Handle osc);
EXPORT Handle Sine_create(float frequency);
EXPORT Handle Square_create(float frequency);

EXPORT Handle Cue_create(Handle osc, Handle env);
EXPORT void Cue_delete(Handle cue);

EXPORT int Debug_oscMapSize();
EXPORT int Debug_envMapSize();
EXPORT int Debug_cueMapSize();
