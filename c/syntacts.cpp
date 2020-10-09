#include "syntacts.h"
#include <syntacts>
#include <unordered_map>
#include <iostream>

using namespace tact;

std::unordered_map<Handle, Signal> g_sigs;
std::unordered_map<Handle, std::unique_ptr<Session>> g_sessions;
std::unordered_map<Handle, std::unique_ptr<Spatializer>> g_spats;

struct Finalizer {
    ~Finalizer()
    { 
        g_sigs.clear();
        g_sessions.clear(); 
        g_spats.clear();
    }
};

Finalizer g_finalizer;

template <typename S>
inline Handle store(const S& s) {
    Signal sig(s);
    Handle h = sig.get();
    g_sigs.emplace(h, std::move(sig));
    return h;
}

///////////////////////////////////////////////////////////////////////////////

int Syntacts_versionMajor() {
    return SYNTACTS_VERSION_MAJOR;
}

int Syntacts_versionMinor() {
    return SYNTACTS_VERSION_MINOR;
}

int Syntacts_versionPatch() {
    return SYNTACTS_VERSION_PATCH;
}

bool Syntacts_asioSupport() {
#ifdef PA_USE_ASIO
    return true;
#else
    return false;
#endif
}

int Syntacts_maxVoices() {
    return SYNTACTS_MAX_VOICES;
}

///////////////////////////////////////////////////////////////////////////////

Handle Session_create() {
    std::unique_ptr session = std::make_unique<Session>();
    Handle h = static_cast<Handle>(session.get());
    g_sessions.emplace(h, std::move(session));
    return h;
}

void Session_delete(Handle session) {
    g_sessions.erase(session);    
}

bool Session_valid(Handle session) {
    return g_sessions.count(session) > 0;
}

int Session_open1(Handle session) {
    return static_cast<Session*>(session)->open();
}

int Session_open2(Handle session, int index) {
    return static_cast<Session*>(session)->open(index);
}

int Session_open3(Handle session, int index, int channelCount, double sampleRate) {
    return static_cast<Session*>(session)->open(index, channelCount, sampleRate);
}

int Session_open4(Handle session, int api) {
    return static_cast<Session*>(session)->open(static_cast<API>(api));
}

int Session_open5(Handle session, char* name, int api) {
    std::string name_str(name);
    return static_cast<Session*>(session)->open(name_str, static_cast<API>(api));
}


int Session_close(Handle session) {
    return static_cast<Session*>(session)->close();
}

bool Session_isOpen(Handle session) {
    return static_cast<Session*>(session)->isOpen();
}

int Session_play(Handle session, int channel, Handle signal) {
    return static_cast<Session*>(session)->play(channel, g_sigs.at(signal));
}

int Session_playAll(Handle session, Handle signal) {
    return static_cast<Session*>(session)->playAll(g_sigs.at(signal));
}

int Session_stop(Handle session, int channel) {
    return static_cast<Session*>(session)->stop(channel);
}

int Session_stopAll(Handle session) {
    return static_cast<Session*>(session)->stopAll();
}

int Session_pause(Handle session, int channel) {
    return static_cast<Session*>(session)->pause(channel);
}

int Session_pauseAll(Handle session) {
    return static_cast<Session*>(session)->pauseAll();
}

int Session_resume(Handle session, int channel) {
    return static_cast<Session*>(session)->resume(channel);
}

int Session_resumeAll(Handle session) {
    return static_cast<Session*>(session)->resumeAll();
}

bool Session_isPlaying(Handle session, int channel) {
    return static_cast<Session*>(session)->isPlaying(channel);
}

bool Session_isPaused(Handle session, int channel) {
    return static_cast<Session*>(session)->isPaused(channel);
}

int Session_setVolume(Handle session, int channel, double volume) {
    return static_cast<Session*>(session)->setVolume(channel, volume);
}

double Session_getVolume(Handle session, int channel) {
    return static_cast<Session*>(session)->getVolume(channel);
}

int Session_setPitch(Handle session, int channel, double pitch) {
    return static_cast<Session*>(session)->setPitch(channel, pitch);
}

double Session_getPitch(Handle session, int channel) {
    return static_cast<Session*>(session)->getPitch(channel);
}

double Session_getLevel(Handle session, int channel) {
    return static_cast<Session*>(session)->getLevel(channel);
}

int Session_getChannelCount(Handle session) {
    return static_cast<Session*>(session)->getChannelCount();
}

double Session_getSampleRate(Handle session) {
    return static_cast<Session*>(session)->getSampleRate();
}

double Session_getCpuLoad(Handle session) {
    return static_cast<Session*>(session)->getCpuLoad();
}

int Session_getCurrentDevice(Handle session) {
    return static_cast<Session*>(session)->getCurrentDevice().index;
}

int Session_getDefaultDevice(Handle session) {
    return static_cast<Session*>(session)->getDefaultDevice().index;
}

int Session_getAvailableDevicesCount(Handle session) {
    return static_cast<int>(static_cast<Session*>(session)->getAvailableDevices().size());
}

void Session_getAvailableDevices(Handle session, int* devices) {
    int i = 0; 
    for (auto& dev : static_cast<Session*>(session)->getAvailableDevices()) {
        devices[i++] = dev.second.index;
    }
}

int Session_count() {
    return Session::count();
}

///////////////////////////////////////////////////////////////////////////////

int  Device_nameLength(Handle session, int d) {
    auto& devs = static_cast<Session*>(session)->getAvailableDevices();
    return (int)devs.at(d).name.length();
}

void Device_name(Handle session, int d, char* buf) {
    auto& devs = static_cast<Session*>(session)->getAvailableDevices();
    auto& name = devs.at(d).name;
    name.copy(buf, name.length());
    buf[name.length()] = '\0';
}

bool Device_isDefault(Handle session, int d) {
    auto& devs = static_cast<Session*>(session)->getAvailableDevices();
    return devs.at(d).isDefault;
}

int  Device_api(Handle session, int d) {
    auto& devs = static_cast<Session*>(session)->getAvailableDevices();
    return static_cast<int>(devs.at(d).api);
}

int  Device_apiNameLength(Handle session, int d) {
    auto& devs = static_cast<Session*>(session)->getAvailableDevices();
    return (int)devs.at(d).apiName.length();
}

void Device_apiName(Handle session, int d, char* buf) {
    auto& devs = static_cast<Session*>(session)->getAvailableDevices();
    auto& name = devs.at(d).apiName;
    name.copy(buf, name.length());
    buf[name.length()] = '\0';
}

bool Device_isApiDefault(Handle session, int d) {
    auto& devs = static_cast<Session*>(session)->getAvailableDevices();
    return devs.at(d).isApiDefault;
}

int  Device_maxChannels(Handle session, int d) {
    auto& devs = static_cast<Session*>(session)->getAvailableDevices();
    return devs.at(d).maxChannels;
}

int  Device_sampleRatesCount(Handle session, int d) {
    auto& devs = static_cast<Session*>(session)->getAvailableDevices();
    return (int)devs.at(d).sampleRates.size();
}

void Device_sampleRates(Handle session, int d, int* sampleRates) {
    auto& devs = static_cast<Session*>(session)->getAvailableDevices();
    auto& srs = devs.at(d).sampleRates;
    for (int i = 0; i < srs.size(); ++i)
        sampleRates[i] = srs[i];
}

int  Device_defaultSampleRate(Handle session, int d) {
    auto& devs = static_cast<Session*>(session)->getAvailableDevices();
    return devs.at(d).defaultSampleRate;
}


///////////////////////////////////////////////////////////////////////////////

Handle Spatializer_create(Handle session) {
    std::unique_ptr<Spatializer> spat;
    if (g_sessions.count(session)) 
        spat = std::make_unique<Spatializer>(static_cast<Session*>(session));
    else
        spat = std::make_unique<Spatializer>();
    Handle h = static_cast<Handle>(spat.get());
    g_spats.emplace(h, std::move(spat));    
    return h;
}

void Spatializer_delete(Handle spat) {
    g_spats.erase(spat);
}

bool Spatializer_valid(Handle spat) {
    return g_spats.count(spat) > 0;
}

void Spatializer_bind(Handle spat, Handle session) {
    static_cast<Spatializer*>(spat)->bind(static_cast<Session*>(session));
}

void Spatializer_unbind(Handle spat) {
    static_cast<Spatializer*>(spat)->unbind();
}

void Spatializer_setPosition(Handle spat, int channel, double x, double y) {
    static_cast<Spatializer*>(spat)->setPosition(channel, x, y);
}

void Spatializer_getPosition(Handle spat, int channel, double* x, double* y) {
    auto p = static_cast<Spatializer*>(spat)->getPosition(channel);
    *x = p.x;
    *y = p.y;
}

void Spatializer_setTarget(Handle spat, double x, double y) {
    static_cast<Spatializer*>(spat)->setTarget(x,y);
}

void Spatializer_getTarget(Handle spat, double* x, double* y) {
    auto p = static_cast<Spatializer*>(spat)->getTarget();
    *x = p.x;
    *y = p.y;
}

void Spatializer_setRadius(Handle spat, double r) {
    static_cast<Spatializer*>(spat)->setRadius(r);
}

double Spatializer_getRadius(Handle spat) {
    return static_cast<Spatializer*>(spat)->getRadius();
}

// 0 = lin, 1 = smooth, 2 = smoother, 3 = smoothest, 4 = log, 5 = exp
void Spatializer_setRollOff(Handle spat, int type) {
    auto sp = static_cast<Spatializer*>(spat);
    switch (type) {
        case 0: sp->setRollOff(Curves::Linear()); break;
        case 1: sp->setRollOff(Curves::Smoothstep()); break;
        case 2: sp->setRollOff(Curves::Smootherstep()); break;
        case 3: sp->setRollOff(Curves::Smootheststep()); break;
        case 4: sp->setRollOff(Curves::Exponential::In()); break;
        case 5: sp->setRollOff(Curves::Exponential::Out()); break;
        default: sp->setRollOff(Curves::Linear()); break;
    }
} 

int Spatializer_getRollOff(Handle spat) {
    auto sp = static_cast<Spatializer*>(spat);
    auto cv = sp->getRollOff();
    // TODO: make this not bad
    if (cv.name() == "Linear")
        return 0;
    if (cv.name() == "Smoothstep")
        return 1;
    if (cv.name() == "Smootherstep")
        return 2;
    if (cv.name() == "Smootheststep")
        return 3;
    if (cv.name() == "Exponential::In")
        return 4;
    if (cv.name() == "Exponential::Out")
        return 5;
    return -1;    
}

void Spatializer_setWrap(Handle spat, double x, double y) {
    auto sp = static_cast<Spatializer*>(spat);
    sp->setWrap(x,y);
}

void Spatializer_getWrap(Handle spat, double* x, double* y) {
    auto sp = static_cast<Spatializer*>(spat);
    auto wr = sp->getWrap();
    *x = wr.x;
    *y = wr.y;
}

bool Spatializer_createGrid(Handle spat, int rows, int cols) {
    return static_cast<Spatializer*>(spat)->createGrid(rows,cols);
}

void Spatializer_clear(Handle spat) {
    static_cast<Spatializer*>(spat)->clear();
}

void Spatializer_remove(Handle spat, int channel) {
    static_cast<Spatializer*>(spat)->remove(channel);
}

int Spatializer_getChannelCount(Handle spat) {
    return static_cast<Spatializer*>(spat)->getChannelCount();
}

bool Spatializer_hasChannel(Handle spat, int channel) {
    return static_cast<Spatializer*>(spat)->hasChannel(channel);
}


void Spatializer_play(Handle spat, Handle signal) {
    static_cast<Spatializer*>(spat)->play(g_sigs.at(signal));
}

void Spatializer_stop(Handle spat) {
    static_cast<Spatializer*>(spat)->stop();
}

void Spatializer_setVolume(Handle spat, double volume) {
    static_cast<Spatializer*>(spat)->setVolume(volume);
}

double Spatializer_getVolume(Handle spat) {
    return static_cast<Spatializer*>(spat)->getVolume();
}

void Spatializer_setPitch(Handle spat, double pitch) {
    static_cast<Spatializer*>(spat)->setPitch(pitch);
}

double Spatializer_getPitch(Handle spat) {
    return static_cast<Spatializer*>(spat)->getPitch();
}


void Spatializer_autoUpdate(Handle spat, bool enable) {
    static_cast<Spatializer*>(spat)->autoUpdate(enable);
}

void Spatializer_update(Handle spat) {
    static_cast<Spatializer*>(spat)->update();
}


///////////////////////////////////////////////////////////////////////////////

void Signal_delete(Handle signal) {
    g_sigs.erase(signal);
}

bool Signal_valid(Handle signal) {
    return g_sigs.count(signal) > 0;
}

double Signal_sample(Handle signal, double t) {
    return g_sigs.at(signal).sample(t);
}

double Signal_length(Handle signal) {
    return g_sigs.at(signal).length();
}

void Signal_setGain(Handle signal, double gain) {
    g_sigs.at(signal).gain = gain;
}

double Signal_getGain(Handle signal) {
    return g_sigs.at(signal).gain;
}

void Signal_setBias(Handle signal, double bias) {
    g_sigs.at(signal).bias = bias;
}

double Signal_getBias(Handle signal) {
    return g_sigs.at(signal).bias;
}


int Signal_count() {
    return Signal::count();
}

///////////////////////////////////////////////////////////////////////////////

Handle Product_create(Handle lhs, Handle rhs) {
    return store(Product(g_sigs.at(lhs), g_sigs.at(rhs)));
}

Handle Sum_create(Handle lhs, Handle rhs) {
    return store(Sum(g_sigs.at(lhs), g_sigs.at(rhs)));
}

Handle Mul_SigFlt(Handle lhs, double rhs) {
    return store(g_sigs.at(lhs) * rhs);
}

Handle Mul_FltSig(double lhs, Handle rhs) {
    return store(lhs * g_sigs.at(rhs));
}

Handle Add_SigFlt(Handle lhs, double rhs) {
    return store(g_sigs.at(lhs) + rhs);
}

Handle Add_FltSig(double lhs, Handle rhs) {
    return store(lhs + g_sigs.at(rhs));
}

Handle Sub_SigFlt(Handle lhs, double rhs) {
    return store(g_sigs.at(lhs) - rhs);
}

Handle Sub_FltSig(double lhs, Handle rhs) {
    return store(lhs - g_sigs.at(rhs));
}

Handle Neg_Sig(Handle signal) {
    return store(-g_sigs.at(signal));
}

///////////////////////////////////////////////////////////////////////////////

Handle Sequence_create() {
    return store(Sequence());
}

double Sequence_getHead(Handle handle) {
    Sequence& s = *(Sequence*)g_sigs.at(handle).get();
    return s.head;
}

void Sequence_setHead(Handle handle, double head) {
    Sequence& s = *(Sequence*)g_sigs.at(handle).get();
    s.head = head;
}

void Sequence_pushFlt(Handle handle, double t) {
    Sequence& s = *(Sequence*)g_sigs.at(handle).get();
    s.push(t);
}

void Sequence_pushSig(Handle handle, Handle signal) {
    Sequence& s = *(Sequence*)g_sigs.at(handle).get();
    s.push(g_sigs.at(signal));
}

void Sequence_pushSeq(Handle handle, Handle sequence) {
    Sequence& s1 = *(Sequence*)g_sigs.at(handle).get();
    Sequence& s2 = *(Sequence*)g_sigs.at(sequence).get();
    s1.push(s2);
}

void Sequence_insertSig(Handle handle, Handle signal, double t) {
    Sequence& s = *(Sequence*)g_sigs.at(handle).get();
    s.insert(g_sigs.at(signal), t);
}

void Sequence_insertSeq(Handle handle, Handle sequence, double t) {
    Sequence& s1 = *(Sequence*)g_sigs.at(handle).get();
    Sequence& s2 = *(Sequence*)g_sigs.at(sequence).get();
    s1.insert(s2, t);
}

void Sequence_clear(Handle handle) {
    Sequence& s = *(Sequence*)g_sigs.at(handle).get();
    s.clear();
}



Handle Sequence_SigSig(Handle lhs, Handle rhs) {
    auto s = g_sigs.at(lhs) << g_sigs.at(rhs);
    return store(s);
}

Handle Sequence_SigFlt(Handle lhs, double rhs) {
    auto s = g_sigs.at(lhs) << rhs;
    return store(s);
}

Handle Sequence_FltSig(double lhs, Handle rhs) {
    auto s = lhs << g_sigs.at(rhs);
    return store(s);
}

void Sequence_SeqFlt(Handle lhs, double rhs) {
    Sequence& s = *(Sequence*)g_sigs.at(lhs).get();
    s << rhs;
}

void Sequence_SeqSig(Handle lhs, Handle rhs) {
    Sequence& s = *(Sequence*)g_sigs.at(lhs).get();
    s << g_sigs.at(rhs);
}

void Sequence_SeqSeq(Handle lhs, Handle rhs) {
    Sequence& s1 = *(Sequence*)g_sigs.at(lhs).get();
    Sequence& s2 = *(Sequence*)g_sigs.at(rhs).get();
    s1 << s2;
}

///////////////////////////////////////////////////////////////////////////////

Handle Time_create() {
    return store(Time());
}

Handle Scalar_create(double value) {
    return store(Scalar(value));
}

Handle Ramp_create1(double initial, double rate) {
    return store(Ramp(initial, rate));
}

Handle Ramp_create2(double initial, double final, double duration) {
    return store(Ramp(initial, final, duration));
}

Handle Noise_create() {
    return store(Noise());
}

Handle Expression_create(const char* expr) {
    return store(Expression(expr));
}

Handle Samples_create(float* samples, int nSamples, double sampleRate) {
    std::vector<float> vsamples(samples, samples + nSamples);
    return store(Samples(vsamples, sampleRate));
}

///////////////////////////////////////////////////////////////////////////////

Handle Repeater_create(Handle signal, int repetitions, double delay) {
    return store(Repeater(g_sigs.at(signal), repetitions, delay));
}

Handle Stretcher_create(Handle signal, double factor) {
    return store(Stretcher(g_sigs.at(signal), factor));
}

Handle Reverser_create(Handle signal) {
    return store(Reverser(g_sigs.at(signal)));
}

///////////////////////////////////////////////////////////////////////////////

Handle Envelope_create(double duration, double amp) {
    return store(Envelope(duration, amp));
}

Handle ASR_create(double a, double s, double r, double amp) {
    return store(ASR(a,s,r,amp));
}

Handle ADSR_create(double a, double d, double s, double r, double amp1, double amp2) {
    return store(ADSR(a,d,s,r, amp1, amp2));
}

Handle ExponentialDecay_create(double amplitude, double decay) {
    return store(ExponentialDecay(amplitude, decay));
}

Handle SignalEnvelope_create(Handle signal, double duration, double amplitude) {
    return store(SignalEnvelope(g_sigs.at(signal), duration, amplitude));
}

///////////////////////////////////////////////////////////////////////////////

Handle Sine_create1(Handle x) {
    return store(Sine(g_sigs.at(x)));
}

Handle Sine_create2(double hertz) {
    return store(Sine(hertz));
}

Handle Sine_create3(double initial, double rate) {
    return store(Sine(initial, rate));
}

Handle Sine_create4(double hertz, Handle modulation, double index) {
    return store(Sine(hertz, g_sigs.at(modulation), index));
}

Handle Square_create1(Handle x) {
    return store(Square(g_sigs.at(x)));
}

Handle Square_create2(double hertz) {
    return store(Square(hertz));
}

Handle Square_create3(double initial, double rate) {
    return store(Square(initial, rate));
}

Handle Square_create4(double hertz, Handle modulation, double index) {
    return store(Square(hertz, g_sigs.at(modulation), index));
}

Handle Saw_create1(Handle x) {
    return store(Saw(g_sigs.at(x)));
}

Handle Saw_create2(double hertz) {
    return store(Saw(hertz));
}

Handle Saw_create3(double initial, double rate) {
    return store(Saw(initial, rate));
}

Handle Saw_create4(double hertz, Handle modulation, double index) {
    return store(Saw(hertz, g_sigs.at(modulation), index));
}

Handle Triangle_create1(Handle x) {
    return store(Triangle(g_sigs.at(x)));
}

Handle Triangle_create2(double hertz) {
    return store(Triangle(hertz));
}

Handle Triangle_create3(double initial, double rate) {
    return store(Triangle(initial, rate));
}

Handle Triangle_create4(double hertz, Handle modulation, double index) {
    return store(Triangle(hertz, g_sigs.at(modulation), index));
}

Handle Pwm_create(double frequency, double dutyCycle) {
    return store(Pwm(frequency, dutyCycle));
}

///////////////////////////////////////////////////////////////////////////////

bool Library_saveSignal(Handle signal, const char* name) {
    return Library::saveSignal(g_sigs.at(signal), name);
} 

Handle Library_loadSignal(const char* name) {
    Signal sig;
    if (Library::loadSignal(sig, name)) 
        return store(sig);    
    return nullptr;    
}

bool Library_deleteSignal(const char* name) {
    return Library::deleteSignal(name);
}

bool Library_exportSignal(Handle signal, const char* filePath, int format, int sampleRate, double maxLength) {
    return Library::exportSignal(g_sigs.at(signal), filePath, static_cast<FileFormat>(format), sampleRate, maxLength);
}

Handle Library_importSignal(const char* filePath, int format, int sampleRate) {
    Signal sig;
    if (Library::importSignal(sig, filePath, static_cast<FileFormat>(format), sampleRate))
        return store(sig);
    return nullptr;
}

///////////////////////////////////////////////////////////////////////////////

int Debug_sigMapSize() {
    return static_cast<int>(g_sigs.size());
}

///////////////////////////////////////////////////////////////////////////////
