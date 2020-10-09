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
    using Handle = System.IntPtr; // = C void*

    ///////////////////////////////////////////////////////////////////////////
    // VERSION
    //////////////////////////////////////////////////////////////////////////

    /// <summary>Containts Syntacts version information.</summary>
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

    /// <summary>Lists all supported audio device drivers APIs.</summary>
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

    /// <summary>Contains information about a specific audio device.</summary>
    public class Device {

        public int index;             ///< device index
        public string name;           ///< device name
        public bool isDefault;        ///< is this the default device?
        public API api;               ///< device API index
        public string apiName;        ///< device API name
        public bool isApiDefault;     ///< is this the default device for its API?
        public int maxChannels;       ///< maximum number of output channels
        public int[] sampleRates;     ///< supported sample rates
        public int defaultSampleRate; ///< the device's default sample rate

        /// <summary>Internal constructor.</summary>
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

    /// <summary>Encapsulates a Syntacts device Session.</summary>
    public class Session : IDisposable
    {
        /// <summary>Constructor.</summary>
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

        /// <summary>Opens the default device with its max channels and default sample rate.</summary>
        public int Open()
        {
            return Dll.Session_open1(handle);
        }

        /// <summary>Opens the default device for a specific API with its max channels and default sample rate.</summary>
        public int Open(API api) {
            return Dll.Session_open4(handle, (int)api);
        }

        /// <summary>Opens a specific device by name and API with its max channels and default sample rate.</summary>
        public int Open(string name, API api) {
            return Dll.Session_open5(handle, name, (int)api);
        }

        /// <summary>Opens a specific device by index with its max channels and default sample rate.</summary>
        public int Open(int index)
        {
            return Dll.Session_open2(handle, index);
        }

        /// <summary>Opens a specific device by index with a specified number of channels and sample rate.</summary>
        public int Open(int index, int channelCount, double sampleRate = 0)
        {
            return Dll.Session_open3(handle, index, channelCount, sampleRate);
        }

        /// <summary>Opens a specific device with its max channels and default sample rate.</summary>
        public int Open(Device device)
        {
            return Dll.Session_open2(handle, device.index);
        }

        /// <summary>Opens a specific device with a specified number of channels and sample rate.</summary>
        public int Open(Device device, int channelCount, double sampleRate = 0)
        {
            return Dll.Session_open3(handle, device.index, channelCount, sampleRate);
        }

        /// <summary>Closes the currently opened device.</summary>
        public int Close()
        {
            return Dll.Session_close(handle);
        }

        /// <summary>Plays a signal on the specified channel of the current device.</summary>
        public int Play(int channel, Signal signal)
        {
            return Dll.Session_play(handle, channel, signal.handle);
        }

        /// <summary>Plays a signal on all available channels of the current device.</summary>
        public int PlayAll(Signal signal)
        {
            return Dll.Session_playAll(handle, signal.handle);
        }

        /// <summary>Returns true if a signal is playing on the specified channel.</summar>
        public bool IsPlaying(int channel) {
            return Dll.Session_isPlaying(handle, channel);
        }

        /// <summary>Stops playing signals on the specified channel of the current device.</summary>
        public int Stop(int channel)
        {
            return Dll.Session_stop(handle, channel);
        }

        /// <summary>Stops playing signals on all channels.</summary>
        public int StopAll()
        {
            return Dll.Session_stopAll(handle);
        }

        /// <summary>Pauses playing signals on the specified channel of the current device.</summary>
        public int Pause(int channel)
        {
            return Dll.Session_pause(handle, channel);
        }

        /// <summary> Pauses playing signals on all channels.</summary>
        public int PauseAll()
        {
            return Dll.Session_pauseAll(handle);
        }

        /// <summary>Returns true if the specified channel is in a paused state.</summar>
        public bool IsPaused(int channel) {
            return Dll.Session_isPaused(handle, channel);
        }

        /// <summary>Resumes playing signals on the specified channel of the current device.</summary>
        public int Resume(int channel)
        {
            return Dll.Session_resume(handle, channel);
        }

        /// <summary>Resumes playing signals on all channels.</summary>
        public int ResumeAll()
        {
            return Dll.Session_resumeAll(handle);
        }

        /// <summary>Sets the volume on the specified channel of the current device.</summary>
        public int SetVolume(int channel, double volume)
        {
            return Dll.Session_setVolume(handle, channel, volume);
        }

        /// <summary>Gets the volume on the specified channel of the current device.</summary>
        public double GetVolume(int channel)
        {
            return Dll.Session_getVolume(handle, channel);
        }

        /// <summary>Sets the pitch on the specified channel of the current device.</summary>
        public int SetPitch(int channel, double pitch)
        {
            return Dll.Session_setPitch(handle, channel, pitch);
        }

        /// <summary>Gets the pitch on the specified channel of the current device.</summary>
        public double GetPitch(int channel)
        {
            return Dll.Session_getPitch(handle, channel);
        }

        /// <summary>Gets the max output level between 0 and 1 for the most recent buffer (useful for visualizations).</summary>
        public double GetLevel(int channel) {
            return Dll.Session_getLevel(handle, channel);
        }

        /// <summary>Gets info for the currently opened device.</summary>
        public Device currentDevice
        {
            get { return new Device(handle, Dll.Session_getCurrentDevice(handle)); }
        }

        /// <summary>Gets info for the default device.</summary>
        public Device defaultDevice
        {
            get { return new Device(handle, Dll.Session_getDefaultDevice(handle)); }
        }

        /// <summary>List of all available devices.</summary>
        public Device[] availableDevices;

        /// <summary>True if a device is open, false otherwise.</summary>
        public bool isOpen
        {
            get { return Dll.Session_isOpen(handle); }
        }

        /// <summary>The number of available channels (0 if not open).</summary>
        public int channelCount
        {
            get { return Dll.Session_getChannelCount(handle); }
        }

        /// <summary>The current sampling rate in Hz.</summary>
        public double sampleRate
        {
            get { return Dll.Session_getSampleRate(handle); }
        }

        /// <summary>The CPU core load (0 to 1) of the Session.</summary>
        public double cpuLoad
        {
            get { return Dll.Session_getCpuLoad(handle); }
        }

        /// <summary>True if the Session Handle is currently valid.</summary>
        public bool valid
        {
            get { return Dll.Session_valid(handle); }
        }

        /// <summary>The number of currently open Sessions.</summary>
        public static int count
        {
            get { return Dll.Session_count(); }
        }

        /// <summary>Finalizer.</summary>
        ~Session()
        {
            Dispose(false);
        }

        /// <summary>Manually frees unmanaged Session resources.</summary>
        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        /// <summary>Manually frees unmanaged Session resources (internal).</summary>
        protected virtual void Dispose(bool disposing)
        {
            if (handle != Handle.Zero)
            {
                Dll.Session_delete(handle);
                handle = Handle.Zero;
            }
        }

        /// <summary>Pointer to unmanaged Session.</summary>
        public Handle handle = Handle.Zero;
    }

    ///////////////////////////////////////////////////////////////////////////
    // SPATIALIZER
    ///////////////////////////////////////////////////////////////////////////

    /// <summary>A 2D Spatializer point.</summary>
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
    public struct Point {
        public Point(double x, double y) { this.x = x; this.y = y; }
        public double x;
        public double y;
    }

    /// <summary>Types of supported interpolatin curves for Spatializer dropoff.</summary>
    public enum Curve {
        Linear        = 0,
        Smoothstep    = 1,
        Smootherstep  = 2,
        Smootheststep = 3,
        Lograthmic    = 4,
        Exponential   = 5
    }

    /// <summary>Syntacts Spatializer interface.</summary>
    public class Spatializer : IDisposable {

        /// <summary>Constructor.</summary>
        public Spatializer(Session session = null) {
            if (session != null)
                this.handle = Dll.Spatializer_create(session.handle);
            else
                this.handle = Dll.Spatializer_create(Handle.Zero);
        }

        /// <summary>Binds this Spatializer to a Session.</summary>
        public void Bind(Session session) {
            Dll.Spatializer_bind(handle, session.handle);
        }

        /// <summary>Unbinds the Spatializer from its current Session.</summary>
        public void Unbind() {
            Dll.Spatializer_unbind(handle);
        }

        /// <summary>Set the position of a channel. The channel will be added if it's not already in the Spatializer.</summary>
        public void SetPosition(int channel, double x, double y) {
            Dll.Spatializer_setPosition(handle, channel, x, y);
        }

        /// <summary>Set the position of a channel. The channel will be added if it's not already in the Spatializer.</summary>
        public void SetPosition(int channel, Point p) {
            Dll.Spatializer_setPosition(handle, channel, p.x, p.y);
        }

        /// <summary>Gets the position of a channel if it is in the Spatializer.</summary>
        public Point GetPosition(int channel) {
            Point p = new Point();
            Dll.Spatializer_getPosition(handle, channel, ref p.x, ref p.y);
            return p;
        }

        /// <summary>Quickly create a grid of channels.</summary>
        public bool CreateGrid(int rows, int cols) {
            return Dll.Spatializer_createGrid(handle, rows, cols);
        }

        /// <summary>Remove all channels from the Spatializer.</summary>
        public void Clear() {
            Dll.Spatializer_clear(handle);
        }

        /// <summary>Remove a channel from the Spatializer.</summary>
        public void Remove(int channel) {
            Dll.Spatializer_remove(handle, channel);
        }

        /// <summary>Returns true if a channel is in the Spatializer.</summary>
        public bool HasChannel(int channel) {
            return Dll.Spatializer_hasChannel(handle, channel);
        }

        /// <summary>Play a Signal on the Spatializer.</summary>
        public void Play(Signal signal) {
            Dll.Spatializer_play(handle, signal.handle);
        }

        /// <summary>Stop all Signals playing in the Spatializer.</summary>
        public void Stop() {
            Dll.Spatializer_stop(handle);
        }

        /// <summary>Explicitly update the pitch/volume of all channels in the Spatializer.</summary>
        public void Update() {
            Dll.Spatializer_update(handle);
        }

        /// <summary>Finalizer.</summary>
        ~Spatializer()
        {
            Dispose(false);
        }

        /// <summary>Manually frees unmanaged Spatializer resources.</summary>
        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        /// <summary>Manually frees unmanaged Spatializer resources (internal).</summary>
        protected virtual void Dispose(bool disposing)
        {
            if (handle != Handle.Zero)
            {
                Dll.Spatializer_delete(handle);
                handle = Handle.Zero;
            }
        }

        /// <summary>The Spatializer target position.</summary>
        public Point target {
            get
            {
                Point p = new Point();
                Dll.Spatializer_getTarget(handle, ref p.x, ref p.y);
                return p;
            }
            set { Dll.Spatializer_setTarget(handle, value.x, value.y);}
        }

        /// <summary>The Spatializer target radius.</summary>
        public double radius {
            get { return Dll.Spatializer_getRadius(handle); }
            set { Dll.Spatializer_setRadius(handle, value); }
        }

        /// <summary>The Spatializer target roll-off method.</summary>
        public Curve rollOff {
            get { return (Curve)Dll.Spatializer_getRollOff(handle); }
            set { Dll.Spatializer_setRollOff(handle, (int)value); }
        }

        /// <summary>Spatializer wrapping intervale for x and y axes. A value of 0 disables wrapping on that axis.</summary>
        public Point wrapInterval {
            get
            {
                Point p = new Point();
                Dll.Spatializer_getWrap(handle, ref p.x, ref p.y);
                return p;
            }
            set { Dll.Spatializer_setWrap(handle, value.x, value.y); }
        }

        /// <summary>The number of channels in the Spatializer.</summary>
        public int channelCount {
            get { return Dll.Spatializer_getChannelCount(handle); }
        }

        /// <summary>The global volume of the Spatializer.</summary>
        public double volume {
            get { return Dll.Spatializer_getVolume(handle); }
            set { Dll.Spatializer_setVolume(handle, value); }
        }

        /// <summary>The global pitch of the Spatializer</summary>
        public double pitch {
            get { return Dll.Spatializer_getPitch(handle); }
            set { Dll.Spatializer_setPitch(handle, value); }
        }

        /// <summary>Enable/disable automatic updating of pitch/volume of all channels when Spatializer target or channels change (enabled by default).</summary>
        public bool autoUpdate {
            set { Dll.Spatializer_autoUpdate(handle, value); }
        }

        /// <summary>True if the Spatializer Handle is currently valid.</summary>
        public bool valid
        {
            get { return Dll.Spatializer_valid(handle); }
        }

        /// <summary>Pointer to unmanaged Spatializer.</summary>
        public Handle handle = Handle.Zero;
    }

    ///////////////////////////////////////////////////////////////////////////
    // SIGNAL
    ///////////////////////////////////////////////////////////////////////////

    /// <summary>An object that returns time variant samples for a length of time.</summary>
    public class Signal : IDisposable
    {
        /// <summary>Default constructor.</summary>
        public Signal()
        { }

        /// <summary>Constructor (internal).</summary>
        public Signal(Handle handle)
        {
            this.handle = handle;
        }

        /// <summary>Samples the Signal at time t in seconds.</summary>
        public double Sample(double t)
        {
            return Dll.Signal_sample(handle, t);
        }

        /// <summary>The length of the Signal in seconds or infinity.</summary>
        public double length
        {
            get { return Dll.Signal_length(handle); }
        }

        /// <summary>The Signal will be scaled by this amount when sampled.</summary>
        public double gain
        {
            set { Dll.Signal_setGain(handle, value); }
            get { return Dll.Signal_getGain(handle); }
        }

        /// <summary>The Signal will be offset by this amount when sampled.</summary>
        public double bias
        {
            set { Dll.Signal_setBias(handle, value); }
            get { return Dll.Signal_getBias(handle); }
        }

        /// <summary>True if the Signal Handle is valid.</summary>
        public bool valid
        {
            get { return Dll.Signal_valid(handle); }
        }

        /// <summary>The current count of Signals allocated in this process.</summary>
        public static int count
        {
            get { return Dll.Signal_count(); }
        }

        /// <summary>Finalizer.</summary>
        ~Signal()
        {
            Dispose(false);
        }

        /// <summary>Manually frees unmanaged Signal resources.</summary>
        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        /// <summary>Manually frees unmanaged Signal resources (internal).</summary>
        protected virtual void Dispose(bool disposing)
        {
            if (handle != Handle.Zero)
            {
                Dll.Signal_delete(handle);
            }
        }

        /// <summary>Multiply two Signals.</summary>
        public static Signal operator *(Signal lhs, Signal rhs)
        {
            return new Product(lhs, rhs);
        }

        /// <summary>Multiply a Signal and a scalar.</summary>
        public static Signal operator *(Signal lhs, double rhs)
        {
            return new Signal(Dll.Mul_SigFlt(lhs.handle, rhs));
        }

        /// <summary>Multiply a scalar and a Signal.</summary>
        public static Signal operator *(double lhs, Signal rhs)
        {
            return new Signal(Dll.Mul_FltSig(lhs, rhs.handle));
        }

        /// <summary>Add two Signals.</summary>
        public static Signal operator +(Signal lhs, Signal rhs)
        {
            return new Sum(lhs, rhs);
        }

        /// <summary>Add a Signal and a scalar.</summary>
        public static Signal operator +(Signal lhs, double rhs)
        {
            return new Signal(Dll.Add_SigFlt(lhs.handle, rhs));
        }

        /// <summary>Add a scalar and a Signal.</summary>
        public static Signal operator +(double lhs, Signal rhs)
        {
            return new Signal(Dll.Add_FltSig(lhs, rhs.handle));
        }

        /// <summary>Subtract a scalar from a Signal.</summary>
        public static Signal operator -(Signal lhs, double rhs)
        {
            return new Signal(Dll.Sub_SigFlt(lhs.handle, rhs));
        }

        /// <summary>Subtract a Signal from a scalar.</summary>
        public static Signal operator -(double lhs, Signal rhs)
        {
            return new Signal(Dll.Sub_FltSig(lhs, rhs.handle));
        }

        /// <summary>Negate a Signal.</summary>
        public static Signal operator -(Signal signal) {
            return new Signal(Dll.Neg_Sig(signal.handle));
        }

        /// <summary>Pointer to unmanaged Signal.</summary>
        internal Handle handle = Handle.Zero;
    }

    ///////////////////////////////////////////////////////////////////////////
    // OPERATORS
    ///////////////////////////////////////////////////////////////////////////

    /// <summary>A Signal which is the sum or difference of two other signals.</summary>
    public class Sum : Signal
    {
        public Sum(Signal lhs, Signal rhs) :
            base(Dll.Sum_create(lhs.handle, rhs.handle))
        { }
    }

    /// <summary>A Signal which is the product of two other signals.</summary>
    public class Product : Signal
    {
        public Product(Signal lhs, Signal rhs) :
            base(Dll.Product_create(lhs.handle, rhs.handle))
        { }
    }

    ///////////////////////////////////////////////////////////////////////////
    // SEQUENCE
    ///////////////////////////////////////////////////////////////////////////

    /// <summary>A Signal formed by the sequencing of other Signals at various time points.</summary>
    public class Sequence : Signal {

        /// <summary>Default constructor.</summary>
        public Sequence() :
            base(Dll.Sequence_create())
        { }

        /// <summary>Constructor (internal).</summary>
        public Sequence(Handle handle)
        {
            this.handle = handle;
        }

        /// <summary>Moves the insertion head forward/backward by t.</summary>
        public Sequence Push(double t) {
            Dll.Sequence_pushFlt(handle, t);
            return this;
        }

        /// <summary>Pushes a Signal at the head position and then moves the head forward.</summary>
        public Sequence Push(Signal signal) {
            Dll.Sequence_pushSig(handle, signal.handle);
            return this;
        }

        /// <summary>Pushes another Sequence at the head position and then moves the head forward.</summary>
        public Sequence Push(Sequence sequence) {
            Dll.Sequence_pushSeq(handle, sequence.handle);
            return this;
        }

        /// <summary>Inserts a Signal at position t in this Sequence but does NOT move head.</summary>
        public Sequence Insert(Signal signal, double t) {
            Dll.Sequence_insertSig(handle, signal.handle, t);
            return this;
        }

        /// <summary>Inserts another Sequence at position t in this Sequence but does NOT move head.</summary>
        public Sequence Insert(Sequence sequence, double t) {
            Dll.Sequence_insertSeq(handle, sequence.handle, t);
            return this;
        }

        /// <summary>The current insertion head position/time.</summary>
        public double head {
            get { return Dll.Sequence_getHead(handle); }
            set { Dll.Sequence_setHead(handle, value); }
        }

        /// <summary>Creates a new Sequence from two Signals (a.k.a "<<" in C++ and Python APIs).</summary>
        public static Sequence Combine(Signal lhs, Signal rhs) {
            return new Sequence(Dll.Sequence_SigSig(lhs.handle,rhs.handle));
        }

        /// <summary>Creates a new Sequence from a Signal and moves the head forward (a.k.a "<<" in C++ and Python APIs).</summary>
        public static Sequence Combine(Signal lhs, double rhs) {
            return new Sequence(Dll.Sequence_SigFlt(lhs.handle,rhs));
        }

        /// <summary>Creates a new Sequence, moves the head forward, then inserts a Signal (a.k.a "<<" in C++ and Python APIs).</summary>
        public static Sequence Combine(double lhs, Signal rhs) {
            return new Sequence(Dll.Sequence_FltSig(lhs,rhs.handle));
        }
    }

    ///////////////////////////////////////////////////////////////////////////
    // GENERAL
    ///////////////////////////////////////////////////////////////////////////

    /// <summary>A signal that simple returns the time passed to it.</summary>
    public class Time : Signal {
        public Time() :
            base(Dll.Time_create())
        { }
    }

    /// <summary>A Signal that emits a constant value over time.</summary>
    public class Scalar : Signal {
        public Scalar(double value) :
            base(Dll.Scalar_create(value))
        { }
    }

    /// <summary>A Signal that increases or decreases over time.</summary>
    public class Ramp : Signal {
        public Ramp(double initial, double rate) :
            base(Dll.Ramp_create1(initial, rate))
        { }

        public Ramp(double initial, double final, double duration) :
            base(Dll.Ramp_create2(initial, final, duration))
        { }
    }

    /// <summary>A signal that generates white noise.</summary>
    public class Noise : Signal {
        public Noise() :
            base(Dll.Noise_create())
        { }
    }

    /// <summary>A signal that returns the evaluation of an expression f(t).</summary>
    public class Expression : Signal {
        public Expression(string expr) :
            base(Dll.Expression_create(expr))
        { }
    }

    /// <summary>A Signal defined by an array of recorded samples (used internally for Library.ImportSignal).</summary>
    public class Samples : Signal {
        public Samples(float[] samples, double sampleRate) :
            base(Dll.Samples_create(samples, samples.Length, sampleRate))
        { }
    }

    ///////////////////////////////////////////////////////////////////////////
    // PROCESS
    ///////////////////////////////////////////////////////////////////////////

    /// <summary>A Signal which repeats another Signal for a number of repetitions.</summary>
    public class Repeater : Signal
    {
        public Repeater(Signal signal, int repetitions, double delay) :
            base(Dll.Repeater_create(signal.handle, repetitions, delay))
        { }
    }

    /// <summary>A Signal which stretches or compresses another Signal temporally by a factor.</summary>
    public class Stretcher : Signal
    {
        public Stretcher(Signal signal, double factor) :
            base(Dll.Stretcher_create(signal.handle, factor))
        { }
    }

    /// <summary>A Signal which reverses the input Signal.</summary>
    public class Reverser : Signal
    {
        public Reverser(Signal signal) :
            base(Dll.Reverser_create(signal.handle))
        { }
    }

    ///////////////////////////////////////////////////////////////////////////
    // ENVELOPE
    ///////////////////////////////////////////////////////////////////////////

    /// <summary>A basic envelope, providing a duration and constant amplitude.</summary>
    public class Envelope : Signal
    {
        public Envelope(double duration, double amplitude = 1) :
            base(Dll.Envelope_create(duration, amplitude))
        { }
    }

    /// <summary>Attack-Sustain-Release Envelope.</summary>
    public class ASR : Signal
    {
        public ASR(double a, double s, double r, double amplitude = 1) :
            base(Dll.ASR_create(a, s, r, amplitude))
        { }
    }

    /// <summary>Attack-Decay-Sustain-Release Envelope.</summary>
    public class ADSR : Signal
    {
        public ADSR(double a, double d, double s, double r, double amplitude1 = 1, double amplitude2 = 0.5) :
            base(Dll.ADSR_create(a,d,s,r,amplitude1,amplitude2))
        { }
    }

    /// <summary>Exponential decay according to the law y = A*e^(-Bt).</summary>
    public class ExponentialDecay : Signal {
        public ExponentialDecay(double amplitude = 1, double decay = 6.907755) :
            base(Dll.ExponentialDecay_create(amplitude,decay))
        { }
    }

    /// <summary>Constructs an Envelope from a Signal.</summary>
    public class SignalEnvelope : Signal {
        public SignalEnvelope(Signal signal, double duration = 1, double amplitude = 1) :
            base(Dll.SignalEnvelope_create(signal.handle, duration, amplitude))
        { }
    }

    // TODO: KeyedEnvelope, ASR/ADSR curves

    ///////////////////////////////////////////////////////////////////////////
    // OSCILLATOR
    ///////////////////////////////////////////////////////////////////////////

    /// <summary>A sine wave Oscillator.</summary>
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

    /// <summary>A square wave Oscillator.</summary>
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

    /// <summary>A saw wave Oscillator.</summary>
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

    /// <summary>A triangle wave Oscillator.</summary>
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

    /// <summary>A PWM square wave with adjustable frequency and duty cycle.</summary>
    public class Pwm : Signal
    {
        public Pwm(double frequency, double dutyCycle) :
            base(Dll.Pwm_create(frequency, dutyCycle))
        { }
    }

    ///////////////////////////////////////////////////////////////////////////
    // LIBRARY
    ///////////////////////////////////////////////////////////////////////////

    /// <summary>Formats used for exporting/importing Signals.<summary>
    public enum FileFormat {
        Auto = 0, ///< automatic detection from file path extension
        SIG = 1,  ///< Syntacts file form
        WAV = 2,  ///< WAV audio file format
        AIFF = 3, ///< AIFF audio file format
        CSV = 4,  ///< comman-separated-value format,
        JSON = 5  ///< human readable serialized format
    }

    /// <summary>Contains Syntacts Library functions.<summary>
    public class Library
    {
        /// <summary>Saves a serialized Signal to the global Syntacts Signal library.<summary>
        public static bool SaveSignal(Signal signal, string name) {
            return Dll.Library_saveSignal(signal.handle, name);
        }

        /// <summary>Loads a serialized Signal from the global Syntacts Signal library.<summary>
        public static bool LoadSignal(out Signal signal, string name)
        {
            signal = new Signal(Dll.Library_loadSignal(name));
            if (signal.handle == Handle.Zero)
                return false;
            return true;
        }

        /// <summary>Erases a Signal from the global Syntacts Signal library if it exists.<summary>
        public static bool DeleteSignal(string name) {
            return Dll.Library_deleteSignal(name);
        }

        /// <summary>Saves a Signal as a specified file format.<summary>
        public static bool ExportSignal(Signal signal, string filePath, FileFormat format = FileFormat.Auto, int sampleRate = 48000, double maxLength = 60) {
            return Dll.Library_exportSignal(signal.handle, filePath, (int)format, sampleRate, maxLength);
        }

        /// <summary>Imports a Signal of a specific file format.<summary>
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
        public static extern int Spatializer_getRollOff(Handle spat);
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
        public static extern Handle SignalEnvelope_create(Handle signal, double duration, double amplitude);

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

// RESOURCES (INTERNAL):

// https://docs.microsoft.com/en-us/dotnet/standard/garbage-collection/implementing-dispose
// https://michaelscodingspot.com/ways-to-cause-memory-leaks-in-dotnet/