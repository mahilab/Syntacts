#pragma once

#define EXPORT extern "C" __declspec(dllexport)

typedef void* Handle;

///////////////////////////////////////////////////////////////////////////////

EXPORT Handle Session_create();
EXPORT void Session_delete(Handle session);
EXPORT int Session_open(Handle session, int index, int channelCount, double sampleRate);
EXPORT int Session_close(Handle session);
EXPORT int Session_play(Handle session, int channel, Handle signal);
EXPORT int Session_stop(Handle session, int channel);
EXPORT int Session_pause(Handle session, int channel);
EXPORT int Session_resume(Handle session, int channel);
EXPORT int Session_setVolume(Handle session, int channel, float volume);
EXPORT int Session_setPitch(Handle session, int channel, float pitch);
EXPORT bool Session_isOpen(Handle session);
EXPORT int Session_count();

///////////////////////////////////////////////////////////////////////////////

EXPORT bool Signal_valid(Handle sig);
EXPORT void Signal_delete(Handle sig);
EXPORT float Signal_sample(Handle sig, float t);
EXPORT float Signal_length(Handle sig);
EXPORT int Signal_count();

///////////////////////////////////////////////////////////////////////////////

EXPORT Handle Scalar_create(float value);
EXPORT Handle Ramp_create(float initial, float rate);

///////////////////////////////////////////////////////////////////////////////

EXPORT Handle Envelope_create(float duration);
EXPORT Handle ASR_create(float a, float s, float r);
EXPORT Handle ADSR_create(float a, float d, float s, float r);

///////////////////////////////////////////////////////////////////////////////

EXPORT Handle Sine_create(float frequency);
EXPORT Handle Square_create(float frequency);
EXPORT Handle Saw_create(float frequency);
EXPORT Handle Triangle_create(float frequency);

///////////////////////////////////////////////////////////////////////////////

EXPORT Handle Product_create(Handle lhs, Handle rhs);
EXPORT Handle Sum_create(Handle lhs, Handle rhs);

///////////////////////////////////////////////////////////////////////////////

EXPORT bool Library_saveSignal(Handle signal, const char* name);
EXPORT Handle Library_loadSignal(const char* name);

///////////////////////////////////////////////////////////////////////////////

EXPORT int Debug_sigMapSize();

EXPORT int Debug_thread();

///////////////////////////////////////////////////////////////////////////////
