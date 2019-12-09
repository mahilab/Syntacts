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

EXPORT bool Signal_valid(Handle signal);
EXPORT void Signal_delete(Handle signal);
EXPORT float Signal_sample(Handle signal, float t);
EXPORT float Signal_length(Handle signal);
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

EXPORT Handle Sequence_create();

EXPORT Handle Sequence_SigSig(Handle lhs, Handle rhs);
EXPORT Handle Sequence_SigFlt(Handle lhs, float rhs);
EXPORT Handle Sequence_FltSig(float lhs, Handle rhs);

EXPORT void Sequence_SeqFlt(Handle lhs, float rhs);
EXPORT void Sequence_SeqSig(Handle lhs, Handle rhs);
EXPORT void Sequence_SeqSeq(Handle lhs, Handle rhs);

///////////////////////////////////////////////////////////////////////////////

EXPORT bool Library_saveSignal(Handle signal, const char* name);
EXPORT Handle Library_loadSignal(const char* name);

///////////////////////////////////////////////////////////////////////////////

EXPORT int Debug_sigMapSize();

///////////////////////////////////////////////////////////////////////////////
