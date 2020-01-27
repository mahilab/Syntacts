#pragma once

#ifdef _WIN32
#define EXPORT extern "C" __declspec(dllexport)
#else
#define EXPORT extern "C"
#endif

typedef void* Handle;

///////////////////////////////////////////////////////////////////////////////
// SESSION
///////////////////////////////////////////////////////////////////////////////

EXPORT Handle Session_create();
EXPORT void Session_delete(Handle session);
EXPORT int Session_open1(Handle session);
EXPORT int Session_open2(Handle session, int index);
EXPORT int Session_open3(Handle session, int index, int channelCount, double sampleRate);
EXPORT int Session_close(Handle session);
EXPORT bool Session_isOpen(Handle session);

EXPORT int Session_play(Handle session, int channel, Handle signal);
EXPORT int Session_playAll(Handle session, Handle signal);
EXPORT int Session_stop(Handle session, int channel);
EXPORT int Session_stopAll(Handle session);
EXPORT int Session_pause(Handle session, int channel);
EXPORT int Session_pauseAll(Handle session);
EXPORT int Session_resume(Handle session, int channel);
EXPORT int Session_resumeAll(Handle session);
EXPORT bool Session_isPlaying(Handle session, int channel);
EXPORT bool Session_isPaused(Handle session, int channel);

EXPORT int Session_setVolume(Handle session, int channel, double volume);
EXPORT double Session_getVolume(Handle session, int channel);
EXPORT int Session_setPitch(Handle session, int channel, double pitch);
EXPORT double Session_getPitch(Handle session, int channel);
EXPORT int Session_getChannelCount(Handle session);
EXPORT double Session_getSampleRate(Handle session);
EXPORT double Session_getCpuLoad(Handle session);

EXPORT int Session_count();

// TODO: Device

///////////////////////////////////////////////////////////////////////////////
// SIGNAL
///////////////////////////////////////////////////////////////////////////////

EXPORT bool Signal_valid(Handle signal);
EXPORT void Signal_delete(Handle signal);
EXPORT double Signal_sample(Handle signal, double t);
EXPORT double Signal_length(Handle signal);
EXPORT void Signal_setGain(Handle signal, double gain);
EXPORT double Signal_getGain(Handle signal);
EXPORT void Signal_setBias(Handle signal, double bias);
EXPORT double Signal_getBias(Handle signal);
EXPORT int Signal_count();

/////////////////////////////////////////////////////////////////////////////
// OPERATORS
/////////////////////////////////////////////////////////////////////////////

EXPORT Handle Product_create(Handle lhs, Handle rhs);
EXPORT Handle Sum_create(Handle lhs, Handle rhs);

EXPORT Handle Mul_SigFlt(Handle lhs, double rhs);
EXPORT Handle Mul_FltSig(double lhs, Handle rhs);

EXPORT Handle Add_SigFlt(Handle lhs, double rhs);
EXPORT Handle Add_FltSig(double lhs, Handle rhs);

EXPORT Handle Sub_SigFlt(Handle lhs, double rhs);
EXPORT Handle Sub_FltSig(double lhs, Handle rhs);

EXPORT Handle Neg_Sig(Handle signal);

///////////////////////////////////////////////////////////////////////////////
// GENERAL
///////////////////////////////////////////////////////////////////////////////

EXPORT Handle Time_create();
EXPORT Handle Scalar_create(double value);
EXPORT Handle Ramp_create(double initial, double rate);
EXPORT Handle Noise_create();
EXPORT Handle Expression_create(const char* expr);
EXPORT Handle Samples_create(float* samples, int nSamples, double sampleRate);

// TODO: PolyBezier

///////////////////////////////////////////////////////////////////////////////
// PROCESS
///////////////////////////////////////////////////////////////////////////////

EXPORT Handle Repeater_create(Handle signal, int repetitions, double delay);
EXPORT Handle Stretcher_create(Handle signal, double factor);
EXPORT Handle Reverser_create(Handle signal);

///////////////////////////////////////////////////////////////////////////////
// ENVELOPE
///////////////////////////////////////////////////////////////////////////////

EXPORT Handle Envelope_create(double duration, double amp);
EXPORT Handle ASR_create(double a, double s, double r, double amp);
EXPORT Handle ADSR_create(double a, double d, double s, double r, double amp1, double amp2);

// TODO: KeyedEnvelope, Curves, SignalEnvelope 

///////////////////////////////////////////////////////////////////////////////
// OSCILLATOR
///////////////////////////////////////////////////////////////////////////////

EXPORT Handle Sine_create1(Handle x);
EXPORT Handle Sine_create2(double hertz);
EXPORT Handle Sine_create3(double initial, double rate);
EXPORT Handle Sine_create4(double hertz, Handle modulation, double index);

EXPORT Handle Square_create1(Handle x);
EXPORT Handle Square_create2(double hertz);
EXPORT Handle Square_create3(double initial, double rate);
EXPORT Handle Square_create4(double hertz, Handle modulation, double index);

EXPORT Handle Saw_create1(Handle x);
EXPORT Handle Saw_create2(double hertz);
EXPORT Handle Saw_create3(double initial, double rate);
EXPORT Handle Saw_create4(double hertz, Handle modulation, double index);

EXPORT Handle Triangle_create1(Handle x);
EXPORT Handle Triangle_create2(double hertz);
EXPORT Handle Triangle_create3(double initial, double rate);
EXPORT Handle Triangle_create4(double hertz, Handle modulation, double index);

EXPORT Handle Pwm_create(double frequency, double dutyCycle);

///////////////////////////////////////////////////////////////////////////////
// SEQUENCE
///////////////////////////////////////////////////////////////////////////////

EXPORT Handle Sequence_create();

EXPORT Handle Sequence_SigSig(Handle lhs, Handle rhs);
EXPORT Handle Sequence_SigFlt(Handle lhs, double rhs);
EXPORT Handle Sequence_FltSig(double lhs, Handle rhs);

EXPORT void Sequence_SeqFlt(Handle lhs, double rhs);
EXPORT void Sequence_SeqSig(Handle lhs, Handle rhs);
EXPORT void Sequence_SeqSeq(Handle lhs, Handle rhs);

// TODO: push, insert, clear, head

///////////////////////////////////////////////////////////////////////////////
// LIBRARY
///////////////////////////////////////////////////////////////////////////////

EXPORT bool Library_saveSignal(Handle signal, const char* name);
EXPORT Handle Library_loadSignal(const char* name);

// TODO: deleteSignal, exportSignal, importSignal

///////////////////////////////////////////////////////////////////////////////
// DEBUG
///////////////////////////////////////////////////////////////////////////////

EXPORT int Debug_sigMapSize();