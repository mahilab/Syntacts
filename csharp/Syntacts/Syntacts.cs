// MIT License
//
// Copyright (c) 2020 Mechatronics and Haptic Interfaces Lab
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// Author(s): Evan Pezent (epezent@rice.edu)

using System;
using System.Runtime.InteropServices;
using System.Text;

namespace Syntacts
{
    using Handle = System.IntPtr; // ~ C void*

    ///////////////////////////////////////////////////////////////////////////
    // VERSION 
    //////////////////////////////////////////////////////////////////////////

    public class Version {
        public static int major { get { return Dll.Syntacts_versionMajor(); } }
        public static int minor { get { return Dll.Syntacts_versionMinor(); } }
        public static int patch { get { return Dll.Syntacts_versionPatch(); } }
        public static bool asio { get { return Dll.Syntacts_asioSupport(); } }
        public static string text { get { return major.ToString() + "." + minor.ToString() + "." + patch.ToString(); } }
    } 
    
    ///////////////////////////////////////////////////////////////////////////
    // DEVICE
    //////////////////////////////////////////////////////////////////////////

    public enum API {
        Unknown         = 0,
        DirectSound     = 1,
        MME             = 2,
        ASIO            = 3,
        SoundManager    = 4,
        CoreAudio       = 5,
        OSS             = 7,
        ALSA            = 8,
        AL              = 9,
        BeOS            = 10,
        WDMKS           = 11,
        JACK            = 12,
        WASAPI          = 13,
        AudioScienceHPI = 14
    }

    public class Device {

        public int index;
        public string name;
        public bool isDefault;
        public API api;
        public string apiName;
        public bool isApiDefault;
        public int maxChannels;
        public int[] sampleRates;
        public int defaultSampleRate;

        internal Device(Handle session, int index) {            
            this.index = index;
            if (this.index != -1) {
                int size = Dll.Device_nameLength(session, index);
                StringBuilder buf = new StringBuilder(size + 1);
                Dll.Device_name(session, index, buf);
                this.name = buf.ToString();
                this.isDefault = Dll.Device_isDefault(session, index);
                this.api = (API)Dll.Device_api(session, index);
                size = Dll.Device_apiNameLength(session, index);
                buf = new StringBuilder(size + 1);
                Dll.Device_apiName(session, index, buf);
                this.apiName = buf.ToString();
                this.isApiDefault = Dll.Device_isApiDefault(session, index);
                this.maxChannels = Dll.Device_maxChannels(session, index);
                size = Dll.Device_sampleRatesCount(session, index);
                this.sampleRates = new int[size];
                Dll.Device_sampleRates(session, index, sampleRates);
                this.defaultSampleRate = Dll.Device_defaultSampleRate(session, index);
            }
        }      
    }

    ///////////////////////////////////////////////////////////////////////////
    // SESSION
    //////////////////////////////////////////////////////////////////////////

    public class Session : IDisposable
    {
        public Session()
        {
            handle = Dll.Session_create();
            int dev_count = Dll.Session_getAvailableDevicesCount(handle);
            this.availableDevices = new Device[dev_count];
            int[] indices = new int[dev_count];
            Dll.Session_getAvailableDevices(handle, indices);
            for (int i = 0; i < dev_count; ++i)            
                this.availableDevices[i] = new Device(handle, indices[i]);            
        }

        public int Open()
        {
            return Dll.Session_open1(handle);
        }

        public int Open(API api) {
            return Dll.Session_open4(handle, (int)api);
        }

        public int Open(string name, API api) {
            return Dll.Session_open5(handle, name, (int)api);
        }

        public int Open(int index)
        {
            return Dll.Session_open2(handle, index);
        }

        public int Open(int index, int channelCount, double sampleRate = 0)
        {
            return Dll.Session_open3(handle, index, channelCount, sampleRate);
        }

        public int Close()
        {
            return Dll.Session_close(handle);
        }

        public int Play(int channel, Signal signal)
        {
            return Dll.Session_play(handle, channel, signal.handle);
        }

        public int PlayAll(Signal signal)
        {
            return Dll.Session_playAll(handle, signal.handle);
        }

        public int Stop(int channel)
        {
            return Dll.Session_stop(handle, channel);
        }

        public int StopAll()
        {
            return Dll.Session_stopAll(handle);
        }

        public int Pause(int channel)
        {
            return Dll.Session_pause(handle, channel);
        }

        public int PauseAll()
        {
            return Dll.Session_pauseAll(handle);
        }

        public int Resume(int channel)
        {
            return Dll.Session_resume(handle, channel);
        }

        public int ResumeAll()
        {
            return Dll.Session_resumeAll(handle);
        }

        public int SetVolume(int channel, double volume)
        {
            return Dll.Session_setVolume(handle, channel, volume);
        }

        public double GetVolume(int channel)
        {
            return Dll.Session_getVolume(handle, channel);
        }

        public int SetPitch(int channel, double pitch)
        {
            return Dll.Session_setPitch(handle, channel, pitch);
        }

        public double GetPitch(int channel)
        {
            return Dll.Session_getPitch(handle, channel);
        }

        public double GetLevel(int channel) {
            return Dll.Session_getLevel(handle, channel);
        }

        public Device currentDevice
        {
            get { return new Device(handle, Dll.Session_getCurrentDevice(handle)); }
        }

        public Device defaultDevice
        {
            get { return new Device(handle, Dll.Session_getDefaultDevice(handle)); }
        }

        public Device[] availableDevices;

        public bool isOpen
        {
            get { return Dll.Session_isOpen(handle); }
        }

        public int channelCount
        {
            get { return Dll.Session_getChannelCount(handle); }
        }

        public double sampleRate
        {
            get { return Dll.Session_getSampleRate(handle); }
        }

        public double cpuLoad
        {
            get { return Dll.Session_getCpuLoad(handle); }
        }

        public bool valid 
        {
            get { return Dll.Session_valid(handle); }
        }

        public static int count
        {
            get { return Dll.Session_count(); }
        }

        ~Session()
        {
            Dispose(false);
        }

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        protected virtual void Dispose(bool disposing)
        {
            if (handle != Handle.Zero)
            {
                Dll.Session_delete(handle);
                handle = Handle.Zero;
            }
        }

        public Handle handle = Handle.Zero;
    }
    
    ///////////////////////////////////////////////////////////////////////////
    // SPATIALIZER
    ///////////////////////////////////////////////////////////////////////////

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
    public struct Point {
        public Point(double x, double y) { this.x = x; this.y = y; }
        public double x;
        public double y;
    }

    public enum Curve {
        Linear = 0, 
        Smoothstep = 1,
        Smootherstep = 2,
        Smootheststep = 3,
        Lograthmic = 4,
        Exponential = 5
    }

    public class Spatializer : IDisposable {

        public Spatializer(Session session = null) {
            if (session != null)
                this.handle = Dll.Spatializer_create(session.handle);
            else
                this.handle = Dll.Spatializer_create(Handle.Zero);
        }

        public void Bind(Session session) {
            Dll.Spatializer_bind(handle, session.handle);
        }

        public void Unbind() {
            Dll.Spatializer_unbind(handle);
        }

        public void SetPosition(int channel, Point p) {
            Dll.Spatializer_setPosition(handle, channel, p.x, p.y);
        }

        public Point GetPosition(int channel) {
            Point p = new Point();
            Dll.Spatializer_getPosition(handle, channel, ref p.x, ref p.y);
            return p;
        }

        public bool CreateGrid(int rows, int cols) {
            return Dll.Spatializer_createGrid(handle, rows, cols);
        }

        public void Clear() {
            Dll.Spatializer_clear(handle);
        }

        public void Remove(int channel) {
            Dll.Spatializer_remove(handle, channel);
        }        

        public bool HasChannel(int channel) {
            return Dll.Spatializer_hasChannel(handle, channel);
        }

        public void Play(Signal signal) {
            Dll.Spatializer_play(handle, signal.handle);
        }

        public void Stop() {
            Dll.Spatializer_stop(handle);
        }        

        public void Update() {
            Dll.Spatializer_update(handle);
        }

        ~Spatializer()
        {
            Dispose(false);
        }

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        protected virtual void Dispose(bool disposing)
        {
            if (handle != Handle.Zero)
            {
                Dll.Spatializer_delete(handle);
                handle = Handle.Zero;
            }
        }

        public Point target {
            get 
            { 
                Point p = new Point();
                Dll.Spatializer_getTarget(handle, ref p.x, ref p.y); 
                return p;
            }
            set { Dll.Spatializer_setTarget(handle, value.x, value.y);}
        }

        public double radius {
            get { return Dll.Spatializer_getRadius(handle); }
            set { Dll.Spatializer_setRadius(handle, value); }
        }

        public Curve rollOff {
            set { Dll.Spatializer_setRollOff(handle, (int)value); }
        }

        public Point wrapInterval {
            get 
            { 
                Point p = new Point();
                Dll.Spatializer_getWrap(handle, ref p.x, ref p.y); 
                return p;
            }
            set { Dll.Spatializer_setWrap(handle, value.x, value.y); }
        }

        public int channelCount {
            get { return Dll.Spatializer_getChannelCount(handle); }
        }

        public double volume {
            get { return Dll.Spatializer_getVolume(handle); }
            set { Dll.Spatializer_setVolume(handle, value); }
        }

        public double pitch {
            get { return Dll.Spatializer_getPitch(handle); }
            set { Dll.Spatializer_setPitch(handle, value); }
        }

        public bool autoUpdate {
            set { Dll.Spatializer_autoUpdate(handle, value); }
        }

        public bool valid 
        {
            get { return Dll.Spatializer_valid(handle); }
        }

        public Handle handle = Handle.Zero;
    }

    ///////////////////////////////////////////////////////////////////////////
    // SIGNAL
    ///////////////////////////////////////////////////////////////////////////

    public class Signal : IDisposable
    {
        public Signal() 
        { }

        public Signal(Handle handle)
        {
            this.handle = handle;
        }

        public double Sample(double t)
        {
            return Dll.Signal_sample(handle, t);
        }

        public double length
        {
            get { return Dll.Signal_length(handle); }
        }

        public double gain
        {
            set { Dll.Signal_setGain(handle, value); }
            get { return Dll.Signal_getGain(handle); }
        }

        public double bias
        {
            set { Dll.Signal_setBias(handle, value); }
            get { return Dll.Signal_getBias(handle); }
        }

        public bool valid
        {
            get { return Dll.Signal_valid(handle); }
        }

        public static int count
        {
            get { return Dll.Signal_count(); }
        }

        ~Signal()
        {
            Dispose(false);
        }

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        protected virtual void Dispose(bool disposing)
        {
            if (handle != Handle.Zero)
            {
                Dll.Signal_delete(handle);
            }
        }

        public static Signal operator *(Signal lhs, Signal rhs)
        {
            return new Product(lhs, rhs);
        }

        public static Signal operator *(Signal lhs, double rhs) 
        {
            return new Signal(Dll.Mul_SigFlt(lhs.handle, rhs));
        }

        public static Signal operator *(double lhs, Signal rhs) 
        {
            return new Signal(Dll.Mul_FltSig(lhs, rhs.handle));
        }

        public static Signal operator +(Signal lhs, Signal rhs)
        {
            return new Sum(lhs, rhs);
        }

        public static Signal operator +(Signal lhs, double rhs) 
        {
            return new Signal(Dll.Add_SigFlt(lhs.handle, rhs));
        }

        public static Signal operator +(double lhs, Signal rhs) 
        {
            return new Signal(Dll.Add_FltSig(lhs, rhs.handle));
        }

        public static Signal operator -(Signal lhs, double rhs) 
        {
            return new Signal(Dll.Sub_SigFlt(lhs.handle, rhs));
        }

        public static Signal operator -(double lhs, Signal rhs) 
        {
            return new Signal(Dll.Sub_FltSig(lhs, rhs.handle));
        }

        public static Signal operator -(Signal signal) {
            return new Signal(Dll.Neg_Sig(signal.handle));
        }

        internal Handle handle = Handle.Zero;
    }

    ///////////////////////////////////////////////////////////////////////////
    // OPERATORS
    ///////////////////////////////////////////////////////////////////////////

    public class Product : Signal
    {
        public Product(Signal lhs, Signal rhs) :
            base(Dll.Product_create(lhs.handle, rhs.handle))
        { }
    }

    public class Sum : Signal
    {
        public Sum(Signal lhs, Signal rhs) :
            base(Dll.Sum_create(lhs.handle, rhs.handle))
        { }
    }

    ///////////////////////////////////////////////////////////////////////////
    // SEQUENCE
    ///////////////////////////////////////////////////////////////////////////

    public class Sequence : Signal {
        public Sequence(Handle handle) 
        {
            this.handle = handle;
        }

        public Sequence() : 
            base(Dll.Sequence_create())
        { }

        public Sequence Push(double t) {
            Dll.Sequence_pushFlt(handle, t);
            return this;
        }

        public Sequence Push(Signal signal) {
            Dll.Sequence_pushSig(handle, signal.handle);
            return this;
        }

        public Sequence Push(Sequence sequence) {
            Dll.Sequence_pushSeq(handle, sequence.handle);
            return this;
        }

        public Sequence Insert(Signal signal, double t) {
            Dll.Sequence_insertSig(handle, signal.handle, t);
            return this;
        }

        public Sequence Insert(Sequence sequence, double t) {
            Dll.Sequence_insertSeq(handle, sequence.handle, t);
            return this;
        }
        
        public double head {
            get { return Dll.Sequence_getHead(handle); }
            set { Dll.Sequence_setHead(handle, value); }
        }

        public static Sequence Combine(Signal lhs, Signal rhs) {
            return new Sequence(Dll.Sequence_SigSig(lhs.handle,rhs.handle));
        }

        public static Sequence Combine(Signal lhs, double rhs) {
            return new Sequence(Dll.Sequence_SigFlt(lhs.handle,rhs));
        }

        public static Sequence Combine(double lhs, Signal rhs) {
            return new Sequence(Dll.Sequence_FltSig(lhs,rhs.handle));
        }
    }

    ///////////////////////////////////////////////////////////////////////////
    // GENERAL
    ///////////////////////////////////////////////////////////////////////////

    public class Time : Signal {
        public Time() :
            base(Dll.Time_create())
        { }
    }

    public class Scalar : Signal {
        public Scalar(double value) :
            base(Dll.Scalar_create(value))
        { }
    }

    public class Ramp : Signal {
        public Ramp(double initial, double rate) :
            base(Dll.Ramp_create1(initial, rate))
        { }

        public Ramp(double initial, double final, double duration) :
            base(Dll.Ramp_create2(initial, final, duration))
        { }
    }

    public class Noise : Signal {
        public Noise() :
            base(Dll.Noise_create())
        { }
    }

    public class Expression : Signal {
        public Expression(string expr) :
            base(Dll.Expression_create(expr))
        { }
    }

    public class Samples : Signal {
        public Samples(float[] samples, double sampleRate) :
            base(Dll.Samples_create(samples, samples.Length, sampleRate))
        { }
    }

    ///////////////////////////////////////////////////////////////////////////
    // PROCESS
    ///////////////////////////////////////////////////////////////////////////

    public class Repeater : Signal 
    {
        public Repeater(Signal signal, int repetitions, double delay) :
            base(Dll.Repeater_create(signal.handle, repetitions, delay))
        { }
    }

    public class Stretcher : Signal
    {
        public Stretcher(Signal signal, double factor) :
            base(Dll.Stretcher_create(signal.handle, factor))
        { }
    }

    public class Reverser : Signal
    {
        public Reverser(Signal signal) : 
            base(Dll.Reverser_create(signal.handle))
        { }
    }

    ///////////////////////////////////////////////////////////////////////////
    // ENVELOPE
    ///////////////////////////////////////////////////////////////////////////

    public class Envelope : Signal
    {
        public Envelope(double duration, double amplitude = 1) :        
            base(Dll.Envelope_create(duration, amplitude))
        { }
    }

    public class ASR : Signal
    {
        public ASR(double a, double s, double r, double amplitude = 1) :
            base(Dll.ASR_create(a, s, r, amplitude))
        { }
    }

    public class ADSR : Signal
    {
        public ADSR(double a, double d, double s, double r, double amplitude1 = 1, double amplitude2 = 0.5) :
            base(Dll.ADSR_create(a,d,s,r,amplitude1,amplitude2))
        { }
    }

    public class ExponentialDecay : Signal {
        public ExponentialDecay(double amplitude = 1, double decay = 6.907755) :
            base(Dll.ExponentialDecay_create(amplitude,decay))
        { }
    }

    ///////////////////////////////////////////////////////////////////////////
    // OSCILLATOR
    ///////////////////////////////////////////////////////////////////////////

    public class Sine : Signal
    {
        public Sine(Signal x) :
            base(Dll.Sine_create1(x.handle))
        { }

        public Sine(double hertz) :
            base(Dll.Sine_create2(hertz))
        { }

        public Sine(double initial, double rate) :
            base(Dll.Sine_create3(initial, rate))
        { }

        public Sine(double hertz, Signal modulation, double index) :
            base(Dll.Sine_create4(hertz, modulation.handle, index))
        { }        
    }

    public class Square : Signal
    {
        public Square(Signal x) :
            base(Dll.Square_create1(x.handle))
        { }

        public Square(double hertz) :
            base(Dll.Square_create2(hertz))
        { }

        public Square(double initial, double rate) :
            base(Dll.Square_create3(initial, rate))
        { }

        public Square(double hertz, Signal modulation, double index) :
            base(Dll.Square_create4(hertz, modulation.handle, index))
        { }        
    }

    public class Saw : Signal
    {
        public Saw(Signal x) :
            base(Dll.Saw_create1(x.handle))
        { }

        public Saw(double hertz) :
            base(Dll.Saw_create2(hertz))
        { }

        public Saw(double initial, double rate) :
            base(Dll.Saw_create3(initial, rate))
        { }

        public Saw(double hertz, Signal modulation, double index) :
            base(Dll.Saw_create4(hertz, modulation.handle, index))
        { }        
    }

    public class Triangle : Signal
    {
        public Triangle(Signal x) :
            base(Dll.Triangle_create1(x.handle))
        { }

        public Triangle(double hertz) :
            base(Dll.Triangle_create2(hertz))
        { }

        public Triangle(double initial, double rate) :
            base(Dll.Triangle_create3(initial, rate))
        { }

        public Triangle(double hertz, Signal modulation, double index) :
            base(Dll.Triangle_create4(hertz, modulation.handle, index))
        { }        
    }

    public class Pwm : Signal 
    {
        public Pwm(double frequency, double dutyCycle) :
            base(Dll.Pwm_create(frequency, dutyCycle))
        { }
    }

    ///////////////////////////////////////////////////////////////////////////
    // LIBRARY
    ///////////////////////////////////////////////////////////////////////////

    public enum FileFormat {
        Auto = 0, ///< automatic detection from file path extension
        SIG = 1,  ///< Syntacts file form
        WAV = 2,  ///< WAV audio file format
        AIFF = 3, ///< AIFF audio file format
        CSV = 4,  ///< comman-separated-value format,
        JSON = 5  ///< human readable serialized format
    }
    
    public class Library
    {
        public static bool LoadSignal(out Signal signal, string name)
        {
            signal = new Signal(Dll.Library_loadSignal(name));
            if (signal.handle == Handle.Zero)
                return false;
            return true;
        }

        public static bool SaveSignal(Signal signal, string name) {
            return Dll.Library_saveSignal(signal.handle, name);
        }

        public static bool DeleteSignal(string name) {
            return Dll.Library_deleteSignal(name);
        }

        public static bool ExportSignal(Signal signal, string filePath, FileFormat format = FileFormat.Auto, int sampleRate = 48000, double maxLength = 60) {
            return Dll.Library_exportSignal(signal.handle, filePath, (int)format, sampleRate, maxLength);
        }

        public static bool ImportSignal(out Signal signal, string filePath, FileFormat format = FileFormat.Auto, int sampleRate = 48000) {
            signal = new Signal(Dll.Library_importSignal(filePath, (int)format, sampleRate));
            if (signal.handle == Handle.Zero)
                return false;
            return true;
        }
    }

    ///////////////////////////////////////////////////////////////////////////
    // DLL IMPORTS (STAY OUT!)
    ///////////////////////////////////////////////////////////////////////////

    class Dll
    {
        [DllImport("syntacts_c")]
        public static extern int Syntacts_versionMajor();
        [DllImport("syntacts_c")]
        public static extern int Syntacts_versionMinor();
        [DllImport("syntacts_c")]
        public static extern int Syntacts_versionPatch();
        [DllImport("syntacts_c")]
        public static extern bool Syntacts_asioSupport();

        [DllImport("syntacts_c")]
        public static extern Handle Session_create();
        [DllImport("syntacts_c")]
        public static extern void Session_delete(Handle session);
        [DllImport("syntacts_c")]
        public static extern bool Session_valid(Handle session);
        [DllImport("syntacts_c")]
        public static extern int Session_open1(Handle session);
        [DllImport("syntacts_c")]
        public static extern int Session_open2(Handle session, int index);
        [DllImport("syntacts_c")]
        public static extern int Session_open3(Handle session, int index, int channelCount, double sampleRate);
        [DllImport("syntacts_c")]
        public static extern int Session_open4(Handle session, int api);
        [DllImport("syntacts_c", CallingConvention = CallingConvention.Cdecl)]
        public static extern int Session_open5(Handle session, string name, int api);
        [DllImport("syntacts_c")]
        public static extern int Session_close(Handle session);
        [DllImport("syntacts_c")]
        public static extern bool Session_isOpen(Handle session);
        [DllImport("syntacts_c")]
        public static extern int Session_play(Handle session, int channel, Handle signal);
        [DllImport("syntacts_c")]
        public static extern int Session_playAll(Handle session, Handle signal);
        [DllImport("syntacts_c")]
        public static extern int Session_stop(Handle session, int channel);
        [DllImport("syntacts_c")]
        public static extern int Session_stopAll(Handle session);
        [DllImport("syntacts_c")]
        public static extern int Session_pause(Handle session, int channel);
        [DllImport("syntacts_c")]
        public static extern int Session_pauseAll(Handle session);
        [DllImport("syntacts_c")]
        public static extern int Session_resume(Handle session, int channel);
        [DllImport("syntacts_c")]
        public static extern int Session_resumeAll(Handle session);
        [DllImport("syntacts_c")]
        public static extern bool Session_isPlaying(Handle session, int channel);
        [DllImport("syntacts_c")]
        public static extern bool Session_isPaused(Handle session, int channel);
        [DllImport("syntacts_c")]
        public static extern int Session_setVolume(Handle session, int channel, double volume);
        [DllImport("syntacts_c")]
        public static extern double Session_getVolume(Handle session, int channel);
        [DllImport("syntacts_c")]
        public static extern int Session_setPitch(Handle session, int channel, double pitch);
        [DllImport("syntacts_c")]
        public static extern double Session_getPitch(Handle session, int channel);
        [DllImport("syntacts_c")]
        public static extern double Session_getLevel(Handle session, int channel);
        [DllImport("syntacts_c")]
        public static extern int Session_getChannelCount(Handle session);
        [DllImport("syntacts_c")]
        public static extern double Session_getSampleRate(Handle session);
        [DllImport("syntacts_c")]
        public static extern double Session_getCpuLoad(Handle session);
        [DllImport("syntacts_c")]
        public static extern int Session_getCurrentDevice(Handle session);
        [DllImport("syntacts_c")]
        public static extern int Session_getDefaultDevice(Handle session);
        [DllImport("syntacts_c")]
        public static extern int Session_getAvailableDevicesCount(Handle session);
        [DllImport("syntacts_c")]
        public static extern void Session_getAvailableDevices(Handle session, int[] devices);
        [DllImport("syntacts_c")]
        public static extern int Session_count();

        [DllImport("syntacts_c")]
        public static extern int  Device_nameLength(Handle session, int d);
        [DllImport("syntacts_c", CallingConvention = CallingConvention.Cdecl)]
        public static extern void Device_name(Handle session, int d, StringBuilder buf);
        [DllImport("syntacts_c")]
        public static extern bool Device_isDefault(Handle session, int d);
        [DllImport("syntacts_c")]
        public static extern int  Device_api(Handle session, int d);
        [DllImport("syntacts_c")]
        public static extern int  Device_apiNameLength(Handle session, int d);
        [DllImport("syntacts_c", CallingConvention = CallingConvention.Cdecl)]
        public static extern void Device_apiName(Handle session, int d, StringBuilder buf);
        [DllImport("syntacts_c")]
        public static extern bool Device_isApiDefault(Handle session, int d);
        [DllImport("syntacts_c")]
        public static extern int  Device_maxChannels(Handle session, int d);
        [DllImport("syntacts_c")]
        public static extern int  Device_sampleRatesCount(Handle session, int d);
        [DllImport("syntacts_c")]
        public static extern void Device_sampleRates(Handle session, int d, int[] sampleRates);
        [DllImport("syntacts_c")]
        public static extern int  Device_defaultSampleRate(Handle session, int d);

        [DllImport("syntacts_c")]
        public static extern Handle Spatializer_create(Handle session);
        [DllImport("syntacts_c")]
        public static extern void Spatializer_delete(Handle spat);
        [DllImport("syntacts_c")]
        public static extern bool Spatializer_valid(Handle spat);
        [DllImport("syntacts_c")]
        public static extern void Spatializer_bind(Handle spat, Handle session);
        [DllImport("syntacts_c")]
        public static extern void Spatializer_unbind(Handle spat);
        [DllImport("syntacts_c")]
        public static extern void Spatializer_setPosition(Handle spat, int channel, double x, double y);
        [DllImport("syntacts_c")]
        public static extern void Spatializer_getPosition(Handle spat, int channel, ref double x, ref double y);
        [DllImport("syntacts_c")]
        public static extern void Spatializer_setTarget(Handle spat, double x, double y);
        [DllImport("syntacts_c")]
        public static extern void Spatializer_getTarget(Handle spat, ref double x, ref double y);
        [DllImport("syntacts_c")]
        public static extern void Spatializer_setRadius(Handle spat, double r);
        [DllImport("syntacts_c")]
        public static extern double Spatializer_getRadius(Handle spat);
        [DllImport("syntacts_c")]
        public static extern void Spatializer_setRollOff(Handle spat, int type);
        [DllImport("syntacts_c")]
        public static extern void Spatializer_setWrap(Handle spat, double x, double y);
        [DllImport("syntacts_c")]
        public static extern void Spatializer_getWrap(Handle spat, ref double x, ref double y);

        [DllImport("syntacts_c")] 
        public static extern bool Spatializer_createGrid(Handle spat, int rows, int cols);
        [DllImport("syntacts_c")]
        public static extern void Spatializer_clear(Handle spat);
        [DllImport("syntacts_c")]
        public static extern void Spatializer_remove(Handle spat, int channel);
        [DllImport("syntacts_c")]
        public static extern int Spatializer_getChannelCount(Handle spat);
        [DllImport("syntacts_c")]
        public static extern bool Spatializer_hasChannel(Handle spat, int channel);

        [DllImport("syntacts_c")]
        public static extern void Spatializer_play(Handle spat, Handle signal);
        [DllImport("syntacts_c")]
        public static extern void Spatializer_stop(Handle spat);
        [DllImport("syntacts_c")]
        public static extern void Spatializer_setVolume(Handle spat, double volume);
        [DllImport("syntacts_c")]
        public static extern double Spatializer_getVolume(Handle spat);
        [DllImport("syntacts_c")]
        public static extern void Spatializer_setPitch(Handle spat, double pitch);
        [DllImport("syntacts_c")]
        public static extern double Spatializer_getPitch(Handle spat);

        [DllImport("syntacts_c")]
        public static extern void Spatializer_autoUpdate(Handle spat, bool enable);
        [DllImport("syntacts_c")]
        public static extern void Spatializer_update(Handle spat);

        [DllImport("syntacts_c")]
        public static extern void Signal_delete(Handle signal);
        [DllImport("syntacts_c")]
        public static extern bool Signal_valid(Handle signal);
        [DllImport("syntacts_c")]
        public static extern double Signal_sample(Handle signal, double t);
        [DllImport("syntacts_c")]
        public static extern double Signal_length(Handle signal);
        [DllImport("syntacts_c")]
        public static extern void Signal_setGain(Handle signal, double gain);
        [DllImport("syntacts_c")]
        public static extern double Signal_getGain(Handle signal);
        [DllImport("syntacts_c")]
        public static extern void Signal_setBias(Handle signal, double bias);
        [DllImport("syntacts_c")]
        public static extern double Signal_getBias(Handle signal);
        [DllImport("syntacts_c")]
        public static extern int Signal_count();

        [DllImport("syntacts_c")]
        public static extern Handle Mul_SigFlt(Handle lhs, double rhs);
        [DllImport("syntacts_c")]
        public static extern Handle Mul_FltSig(double lhs, Handle rhs);
        [DllImport("syntacts_c")]
        public static extern Handle Add_SigFlt(Handle lhs, double rhs);
        [DllImport("syntacts_c")]
        public static extern Handle Add_FltSig(double lhs, Handle rhs);
        [DllImport("syntacts_c")]
        public static extern Handle Sub_SigFlt(Handle lhs, double rhs);
        [DllImport("syntacts_c")]
        public static extern Handle Sub_FltSig(double lhs, Handle rhs);
        [DllImport("syntacts_c")]
        public static extern Handle Neg_Sig(Handle signal);

        [DllImport("syntacts_c")]
        public static extern Handle Sequence_create();
        [DllImport("syntacts_c")]
        public static extern double Sequence_getHead(Handle handle);
        [DllImport("syntacts_c")]
        public static extern void Sequence_setHead(Handle handle, double head);
        [DllImport("syntacts_c")]
        public static extern void Sequence_pushFlt(Handle handle, double t);
        [DllImport("syntacts_c")]
        public static extern void Sequence_pushSig(Handle handle, Handle signal);
        [DllImport("syntacts_c")]
        public static extern void Sequence_pushSeq(Handle handle, Handle sequence);
        [DllImport("syntacts_c")]
        public static extern void Sequence_insertSig(Handle handle, Handle signal, double t);
        [DllImport("syntacts_c")]
        public static extern void Sequence_insertSeq(Handle handle, Handle sequence, double t);
        [DllImport("syntacts_c")]
        public static extern void Sequence_clear(Handle handle);

        [DllImport("syntacts_c")]
        public static extern Handle Sequence_SigSig(Handle lhs, Handle rhs);
        [DllImport("syntacts_c")]
        public static extern Handle Sequence_SigFlt(Handle lhs, double rhs);
        [DllImport("syntacts_c")]
        public static extern Handle Sequence_FltSig(double lhs, Handle rhs);
        [DllImport("syntacts_c")]
        public static extern void Sequence_SeqFlt(Handle lhs, double rhs);
        [DllImport("syntacts_c")]
        public static extern void Sequence_SeqSig(Handle lhs, Handle rhs);
        [DllImport("syntacts_c")]
        public static extern void Sequence_SeqSeq(Handle lhs, Handle rhs);

        [DllImport("syntacts_c")]
        public static extern Handle Product_create(Handle lhs, Handle rhs);
        [DllImport("syntacts_c")]
        public static extern Handle Sum_create(Handle lhs, Handle rhs);

        [DllImport("syntacts_c")]
        public static extern Handle Time_create();
        [DllImport("syntacts_c")]
        public static extern Handle Scalar_create(double value);
        [DllImport("syntacts_c")]
        public static extern Handle Ramp_create1(double initial, double rate);
        [DllImport("syntacts_c")]
        public static extern Handle Ramp_create2(double initial, double final, double duration);
        [DllImport("syntacts_c")]
        public static extern Handle Noise_create();
        [DllImport("syntacts_c", CallingConvention = CallingConvention.Cdecl)]
        public static extern Handle Expression_create(string expr);
        [DllImport("syntacts_c")]
        public static extern Handle Samples_create(float[] samples, int nSamples, double sampleRate);

        [DllImport("syntacts_c")]
        public static extern Handle Repeater_create(Handle signal, int repetitions, double delay);
        [DllImport("syntacts_c")]
        public static extern Handle Stretcher_create(Handle signal, double factor);
        [DllImport("syntacts_c")]
        public static extern Handle Reverser_create(Handle signal);

        [DllImport("syntacts_c")]
        public static extern Handle Envelope_create(double duration, double amp);
        [DllImport("syntacts_c")]
        public static extern Handle ASR_create(double a, double s, double r, double amp);
        [DllImport("syntacts_c")]
        public static extern Handle ADSR_create(double a, double d, double s, double r, double amp1, double amp2);
        [DllImport("syntacts_c")]
        public static extern Handle ExponentialDecay_create(double amplitude, double decay);

        [DllImport("syntacts_c")]
        public static extern Handle Sine_create1(Handle x);
        [DllImport("syntacts_c")]
        public static extern Handle Sine_create2(double hertz);
        [DllImport("syntacts_c")]
        public static extern Handle Sine_create3(double initial, double rate);
        [DllImport("syntacts_c")]
        public static extern Handle Sine_create4(double hertz, Handle modulation, double index);

        [DllImport("syntacts_c")]
        public static extern Handle Square_create1(Handle x);
        [DllImport("syntacts_c")]
        public static extern Handle Square_create2(double hertz);
        [DllImport("syntacts_c")]
        public static extern Handle Square_create3(double initial, double rate);
        [DllImport("syntacts_c")]
        public static extern Handle Square_create4(double hertz, Handle modulation, double index);

        [DllImport("syntacts_c")]
        public static extern Handle Saw_create1(Handle x);
        [DllImport("syntacts_c")]
        public static extern Handle Saw_create2(double hertz);
        [DllImport("syntacts_c")]
        public static extern Handle Saw_create3(double initial, double rate);
        [DllImport("syntacts_c")]
        public static extern Handle Saw_create4(double hertz, Handle modulation, double index);

        [DllImport("syntacts_c")]
        public static extern Handle Triangle_create1(Handle x);
        [DllImport("syntacts_c")]
        public static extern Handle Triangle_create2(double hertz);
        [DllImport("syntacts_c")]
        public static extern Handle Triangle_create3(double initial, double rate);
        [DllImport("syntacts_c")]
        public static extern Handle Triangle_create4(double hertz, Handle modulation, double index);

        [DllImport("syntacts_c")]
        public static extern Handle Pwm_create(double frequency, double dutyCycle);

        [DllImport("syntacts_c", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool Library_saveSignal(Handle signal, string name);
        [DllImport("syntacts_c", CallingConvention = CallingConvention.Cdecl)]
        public static extern Handle Library_loadSignal(string name);

        [DllImport("syntacts_c", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool Library_deleteSignal(string name);
        [DllImport("syntacts_c", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool Library_exportSignal(Handle signal, string filePath, int format, int sampleRate, double maxLength);
        [DllImport("syntacts_c", CallingConvention = CallingConvention.Cdecl)]
        public static extern Handle Library_importSignal(string filePath, int format, int sampleRate);

        [DllImport("syntacts_c")]
        public static extern int Debug_sigMapSize();

    }
}

// https://docs.microsoft.com/en-us/dotnet/standard/garbage-collection/implementing-dispose
// https://michaelscodingspot.com/ways-to-cause-memory-leaks-in-dotnet/