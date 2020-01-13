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
EXPORT int Session_setVolume(Handle session, int channel, double volume);
EXPORT int Session_setPitch(Handle session, int channel, double pitch);
EXPORT bool Session_isOpen(Handle session);
EXPORT int Session_count();

///////////////////////////////////////////////////////////////////////////////

EXPORT bool Signal_valid(Handle signal);
EXPORT void Signal_delete(Handle signal);
EXPORT double Signal_sample(Handle signal, double t);
EXPORT double Signal_length(Handle signal);
EXPORT int Signal_count();

/////////////////////////////////////////////////////////////////////////////

EXPORT Handle Mul_SigFlt(Handle lhs, double rhs);
EXPORT Handle Mul_FltSig(double lhs, Handle rhs);

EXPORT Handle Add_SigFlt(Handle lhs, double rhs);
EXPORT Handle Add_FltSig(double lhs, Handle rhs);

EXPORT Handle Sub_SigFlt(Handle lhs, double rhs);
EXPORT Handle Sub_FltSig(double lhs, Handle rhs);

EXPORT Handle Neg_Sig(Handle signal);

///////////////////////////////////////////////////////////////////////////////

EXPORT Handle Product_create(Handle lhs, Handle rhs);
EXPORT Handle Sum_create(Handle lhs, Handle rhs);

///////////////////////////////////////////////////////////////////////////////

EXPORT Handle Scalar_create(double value);
EXPORT Handle Ramp_create(double initial, double rate);

///////////////////////////////////////////////////////////////////////////////

EXPORT Handle Envelope_create(double duration);
EXPORT Handle ASR_create(double a, double s, double r);
EXPORT Handle ADSR_create(double a, double d, double s, double r);

///////////////////////////////////////////////////////////////////////////////

EXPORT Handle Sine_create(double frequency);
EXPORT Handle Square_create(double frequency);
EXPORT Handle Saw_create(double frequency);
EXPORT Handle Triangle_create(double frequency);

///////////////////////////////////////////////////////////////////////////////

EXPORT Handle Sequence_create();

EXPORT Handle Sequence_SigSig(Handle lhs, Handle rhs);
EXPORT Handle Sequence_SigFlt(Handle lhs, double rhs);
EXPORT Handle Sequence_FltSig(double lhs, Handle rhs);

EXPORT void Sequence_SeqFlt(Handle lhs, double rhs);
EXPORT void Sequence_SeqSig(Handle lhs, Handle rhs);
EXPORT void Sequence_SeqSeq(Handle lhs, Handle rhs);

///////////////////////////////////////////////////////////////////////////////

EXPORT bool Library_saveSignal(Handle signal, const char* name);
EXPORT Handle Library_loadSignal(const char* name);

///////////////////////////////////////////////////////////////////////////////

EXPORT int Debug_sigMapSize();

///////////////////////////////////////////////////////////////////////////////
