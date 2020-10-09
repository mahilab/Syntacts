#ifndef SYNTACTS_C_H
#define SYNTACTS_C_H

#ifdef __cplusplus
    #ifdef _WIN32
        #define EXPORT  __declspec(dllexport)
    #else
        #define EXPORT 
    #endif
#else
    #define EXPORT
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef void* Handle;

///////////////////////////////////////////////////////////////////////////////
// SYNTACTS CONFIG
///////////////////////////////////////////////////////////////////////////////

EXPORT int Syntacts_versionMajor();
EXPORT int Syntacts_versionMinor();
EXPORT int Syntacts_versionPatch();
EXPORT bool Syntacts_asioSupport();
EXPORT int Syntacts_maxVoices();

///////////////////////////////////////////////////////////////////////////////
// SESSION
///////////////////////////////////////////////////////////////////////////////

EXPORT Handle Session_create();
EXPORT void Session_delete(Handle session);
EXPORT bool Session_valid(Handle session);
EXPORT int Session_open1(Handle session);
EXPORT int Session_open2(Handle session, int index);
EXPORT int Session_open3(Handle session, int index, int channelCount, double sampleRate);
EXPORT int Session_open4(Handle session, int api);
EXPORT int Session_open5(Handle session, char* name, int api);
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
EXPORT double Session_getLevel(Handle session, int channel);
EXPORT int Session_getChannelCount(Handle session);
EXPORT double Session_getSampleRate(Handle session);
EXPORT double Session_getCpuLoad(Handle session);

EXPORT int Session_getCurrentDevice(Handle session);
EXPORT int Session_getDefaultDevice(Handle session);
EXPORT int Session_getAvailableDevicesCount(Handle session);
EXPORT void Session_getAvailableDevices(Handle session, int* devices);

EXPORT int Session_count();

///////////////////////////////////////////////////////////////////////////////
// DEVICES
///////////////////////////////////////////////////////////////////////////////

EXPORT int  Device_nameLength(Handle session, int d);
EXPORT void Device_name(Handle session, int d, char* buf);
EXPORT bool Device_isDefault(Handle session, int d);
EXPORT int  Device_api(Handle session, int d);
EXPORT int  Device_apiNameLength(Handle session, int d);
EXPORT void Device_apiName(Handle session, int d, char* buf);
EXPORT bool Device_isApiDefault(Handle session, int d);
EXPORT int  Device_maxChannels(Handle session, int d);
EXPORT int  Device_sampleRatesCount(Handle session, int d);
EXPORT void Device_sampleRates(Handle session, int d, int* sampleRates);
EXPORT int  Device_defaultSampleRate(Handle session, int d);

///////////////////////////////////////////////////////////////////////////////
// SPATIALIZER
///////////////////////////////////////////////////////////////////////////////

EXPORT Handle Spatializer_create(Handle session);
EXPORT void Spatializer_delete(Handle spat);
EXPORT bool Spatializer_valid(Handle spat);
EXPORT void Spatializer_bind(Handle spat, Handle session);
EXPORT void Spatializer_unbind(Handle spat);
EXPORT void Spatializer_setPosition(Handle spat, int channel, double x, double y);
EXPORT void Spatializer_getPosition(Handle spat, int channel, double* x, double* y);
EXPORT void Spatializer_setTarget(Handle spat, double x, double y);
EXPORT void Spatializer_getTarget(Handle spat, double* x, double* y);
EXPORT void Spatializer_setRadius(Handle spat, double r);
EXPORT double Spatializer_getRadius(Handle spat);
EXPORT void Spatializer_setRollOff(Handle spat, int type); 
EXPORT int Spatializer_getRollOff(Handle spat);
EXPORT void Spatializer_setWrap(Handle spat, double x, double y);
EXPORT void Spatializer_getWrap(Handle spat, double* x, double* y);

EXPORT bool Spatializer_createGrid(Handle spat, int rows, int cols);
EXPORT void Spatializer_clear(Handle spat);
EXPORT void Spatializer_remove(Handle spat, int channel);
EXPORT int Spatializer_getChannelCount(Handle spat);
EXPORT bool Spatializer_hasChannel(Handle spat, int channel);

EXPORT void Spatializer_play(Handle spat, Handle signal);
EXPORT void Spatializer_stop(Handle spat);
EXPORT void Spatializer_setVolume(Handle spat, double volume);
EXPORT double Spatializer_getVolume(Handle spat);
EXPORT void Spatializer_setPitch(Handle spat, double pitch);
EXPORT double Spatializer_getPitch(Handle spat);

EXPORT void Spatializer_autoUpdate(Handle spat, bool enable);
EXPORT void Spatializer_update(Handle spat);

// TODO: getRollOff, getChannels

///////////////////////////////////////////////////////////////////////////////
// SIGNAL
///////////////////////////////////////////////////////////////////////////////

EXPORT void Signal_delete(Handle signal);
EXPORT bool Signal_valid(Handle signal);
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
// SEQUENCE
///////////////////////////////////////////////////////////////////////////////

EXPORT Handle Sequence_create();
EXPORT double Sequence_getHead(Handle handle);
EXPORT void Sequence_setHead(Handle handle, double head);
EXPORT void Sequence_pushFlt(Handle handle, double t);
EXPORT void Sequence_pushSig(Handle handle, Handle signal);
EXPORT void Sequence_pushSeq(Handle handle, Handle sequence);
EXPORT void Sequence_insertSig(Handle handle, Handle signal, double t);
EXPORT void Sequence_insertSeq(Handle handle, Handle sequence, double t);
EXPORT void Sequence_clear(Handle handle);

EXPORT Handle Sequence_SigSig(Handle lhs, Handle rhs);
EXPORT Handle Sequence_SigFlt(Handle lhs, double rhs);
EXPORT Handle Sequence_FltSig(double lhs, Handle rhs);
EXPORT void Sequence_SeqFlt(Handle lhs, double rhs);
EXPORT void Sequence_SeqSig(Handle lhs, Handle rhs);
EXPORT void Sequence_SeqSeq(Handle lhs, Handle rhs);

///////////////////////////////////////////////////////////////////////////////
// GENERAL
///////////////////////////////////////////////////////////////////////////////

EXPORT Handle Time_create();
EXPORT Handle Scalar_create(double value);
EXPORT Handle Ramp_create1(double initial, double rate);
EXPORT Handle Ramp_create2(double initial, double final, double duration);
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
EXPORT Handle ExponentialDecay_create(double amplitude, double decay);
EXPORT Handle SignalEnvelope_create(Handle signal, double duration, double amplitude);

// TODO: KeyedEnvelope, ASR/ADSR curves

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
// LIBRARY
///////////////////////////////////////////////////////////////////////////////

EXPORT bool Library_saveSignal(Handle signal, const char* name);
EXPORT Handle Library_loadSignal(const char* name);
EXPORT bool Library_deleteSignal(const char* name);
EXPORT bool Library_exportSignal(Handle signal, const char* filePath, int format, int sampleRate, double maxLength);
EXPORT Handle Library_importSignal(const char* filePath, int format, int sampleRate);

///////////////////////////////////////////////////////////////////////////////
// DEBUG
///////////////////////////////////////////////////////////////////////////////

EXPORT int Debug_sigMapSize();

///////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
} // extern "C"
#endif

#endif // SYNTACTS_C_H