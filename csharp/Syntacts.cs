using System;
using System.Runtime.InteropServices;

namespace Syntacts
{
    using Handle = System.IntPtr; // C void*

    ///////////////////////////////////////////////////////////////////////////
    // SESSION
    //////////////////////////////////////////////////////////////////////////

    class Session : IDisposable
    {
        public Session()
        {
            handle = Dll.Session_create();
        }

        public int Open()
        {
            return Dll.Session_open1(handle);
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

        public bool IsOpen()
        {
            return Dll.Session_isOpen(handle);
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

        public int ChannelCount
        {
            get { return Dll.Session_getChannelCount(handle); }
        }

        public double SampleRate
        {
            get { return Dll.Session_getSampleRate(handle); }
        }

        public double CpuLoad
        {
            get { return Dll.Session_getCpuLoad(handle); }
        }

        public static int Count
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
    // SIGNAL
    ///////////////////////////////////////////////////////////////////////////

    class Signal : IDisposable
    {

        public Signal(Handle handle)
        {
            this.handle = handle;
        }

        public double Sample(double t)
        {
            return Dll.Signal_sample(handle, t);
        }

        public double Length
        {
            get { return Dll.Signal_length(handle); }
        }

        public double Gain
        {
            set { Dll.Signal_setGain(handle, value); }
            get { return Dll.Signal_getGain(handle); }
        }

        public double Bias
        {
            set { Dll.Signal_setBias(handle, value); }
            get { return Dll.Signal_getBias(handle); }
        }

        public bool Valid
        {
            get { return Dll.Signal_valid(handle); }
        }

        public static int Count
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

    class Product : Signal
    {
        public Product(Signal lhs, Signal rhs) :
            base(Dll.Product_create(lhs.handle, rhs.handle))
        { }
    }

    class Sum : Signal
    {
        public Sum(Signal lhs, Signal rhs) :
            base(Dll.Sum_create(lhs.handle, rhs.handle))
        { }
    }

    ///////////////////////////////////////////////////////////////////////////
    // GENERAL
    ///////////////////////////////////////////////////////////////////////////

    // TODO

    ///////////////////////////////////////////////////////////////////////////
    // PROCESS
    ///////////////////////////////////////////////////////////////////////////

    // TODO

    ///////////////////////////////////////////////////////////////////////////
    // ENVELOPE
    ///////////////////////////////////////////////////////////////////////////

    // class Envelope : Signal
    // {
    //     public Envelope(double duration)
    //     {
    //         handle = Dll.Envelope_create(duration);
    //     }
    // }

    class ASR : Signal
    {
        public ASR(double a, double s, double r, double amplitude = 1) :
            base(Dll.ASR_create(a, s, r, amplitude))
        { }
    }

    // class ADSR : Signal
    // {
    //     public ADSR(double a, double d, double s, double r)
    //     {
    //         handle = Dll.ADSR_create(a, d, s, r);
    //     }
    // }

    ///////////////////////////////////////////////////////////////////////////
    // OSCILLATOR
    ///////////////////////////////////////////////////////////////////////////

    class Sine : Signal
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

    ///////////////////////////////////////////////////////////////////////////
    // LIBRARY
    ///////////////////////////////////////////////////////////////////////////


    // class Library
    // {
    //     public static Signal LoadSignal(string name)
    //     {
    //         Signal sig = new Signal();
    //         sig.handle = Dll.Library_loadSignal(name);
    //         return sig;
    //     }
    // }

    class Dll
    {
        [DllImport("syntacts-c")]
        public static extern Handle Session_create();
        [DllImport("syntacts-c")]
        public static extern void Session_delete(Handle session);
        [DllImport("syntacts-c")]
        public static extern int Session_open1(Handle session);
        [DllImport("syntacts-c")]
        public static extern int Session_open2(Handle session, int index);
        [DllImport("syntacts-c")]
        public static extern int Session_open3(Handle session, int index, int channelCount, double sampleRate);
        [DllImport("syntacts-c")]
        public static extern int Session_close(Handle session);
        [DllImport("syntacts-c")]
        public static extern bool Session_isOpen(Handle session);
        [DllImport("syntacts-c")]
        public static extern int Session_play(Handle session, int channel, Handle signal);
        [DllImport("syntacts-c")]
        public static extern int Session_playAll(Handle session, Handle signal);
        [DllImport("syntacts-c")]
        public static extern int Session_stop(Handle session, int channel);
        [DllImport("syntacts-c")]
        public static extern int Session_stopAll(Handle session);
        [DllImport("syntacts-c")]
        public static extern int Session_pause(Handle session, int channel);
        [DllImport("syntacts-c")]
        public static extern int Session_pauseAll(Handle session);
        [DllImport("syntacts-c")]
        public static extern int Session_resume(Handle session, int channel);
        [DllImport("syntacts-c")]
        public static extern int Session_resumeAll(Handle session);
        [DllImport("syntacts-c")]
        public static extern bool Session_isPlaying(Handle session, int channel);
        [DllImport("syntacts-c")]
        public static extern bool Session_isPaused(Handle session, int channel);
        [DllImport("syntacts-c")]
        public static extern int Session_setVolume(Handle session, int channel, double volume);
        [DllImport("syntacts-c")]
        public static extern double Session_getVolume(Handle session, int channel);
        [DllImport("syntacts-c")]
        public static extern int Session_setPitch(Handle session, int channel, double pitch);
        [DllImport("syntacts-c")]
        public static extern double Session_getPitch(Handle session, int channel);
        [DllImport("syntacts-c")]
        public static extern int Session_getChannelCount(Handle session);
        [DllImport("syntacts-c")]
        public static extern double Session_getSampleRate(Handle session);
        [DllImport("syntacts-c")]
        public static extern double Session_getCpuLoad(Handle session);
        [DllImport("syntacts-c")]
        public static extern int Session_count();

        [DllImport("syntacts-c")]
        public static extern bool Signal_valid(Handle signal);
        [DllImport("syntacts-c")]
        public static extern void Signal_delete(Handle signal);
        [DllImport("syntacts-c")]
        public static extern double Signal_sample(Handle signal, double t);
        [DllImport("syntacts-c")]
        public static extern double Signal_length(Handle signal);
        [DllImport("syntacts-c")]
        public static extern void Signal_setGain(Handle signal, double gain);
        [DllImport("syntacts-c")]
        public static extern double Signal_getGain(Handle signal);
        [DllImport("syntacts-c")]
        public static extern void Signal_setBias(Handle signal, double bias);
        [DllImport("syntacts-c")]
        public static extern double Signal_getBias(Handle signal);
        [DllImport("syntacts-c")]
        public static extern int Signal_count();

        [DllImport("syntacts-c")]
        public static extern Handle Mul_SigFlt(Handle lhs, double rhs);
        [DllImport("syntacts-c")]
        public static extern Handle Mul_FltSig(double lhs, Handle rhs);
        [DllImport("syntacts-c")]
        public static extern Handle Add_SigFlt(Handle lhs, double rhs);
        [DllImport("syntacts-c")]
        public static extern Handle Add_FltSig(double lhs, Handle rhs);
        [DllImport("syntacts-c")]
        public static extern Handle Sub_SigFlt(Handle lhs, double rhs);
        [DllImport("syntacts-c")]
        public static extern Handle Sub_FltSig(double lhs, Handle rhs);
        [DllImport("syntacts-c")]
        public static extern Handle Neg_Sig(Handle signal);

        [DllImport("syntacts-c")]
        public static extern Handle Product_create(Handle lhs, Handle rhs);
        [DllImport("syntacts-c")]
        public static extern Handle Sum_create(Handle lhs, Handle rhs);

        [DllImport("syntacts-c")]
        public static extern Handle Time_create();
        [DllImport("syntacts-c")]
        public static extern Handle Scalar_create(double value);
        [DllImport("syntacts-c")]
        public static extern Handle Ramp_create(double initial, double rate);
        [DllImport("syntacts-c")]
        public static extern Handle Noise_create();
        [DllImport("syntacts-c", CallingConvention = CallingConvention.Cdecl)]
        public static extern Handle Expression_create(string expr);
        [DllImport("syntacts-c")]
        public static extern Handle Samples_create(float[] samples, int nSamples, double sampleRate);

        [DllImport("syntacts-c")]
        public static extern Handle Repeater_create(Handle signal, int repetitions, double delay);
        [DllImport("syntacts-c")]
        public static extern Handle Stretcher_create(Handle signal, double factor);
        [DllImport("syntacts-c")]
        public static extern Handle Reverser_create(Handle signal);

        [DllImport("syntacts-c")]
        public static extern Handle Envelope_create(double duration, double amp);
        [DllImport("syntacts-c")]
        public static extern Handle ASR_create(double a, double s, double r, double amp);
        [DllImport("syntacts-c")]
        public static extern Handle ADSR_create(double a, double d, double s, double r, double amp1, double amp2);

        [DllImport("syntacts-c")]
        public static extern Handle Sine_create1(Handle x);
        [DllImport("syntacts-c")]
        public static extern Handle Sine_create2(double hertz);
        [DllImport("syntacts-c")]
        public static extern Handle Sine_create3(double initial, double rate);
        [DllImport("syntacts-c")]
        public static extern Handle Sine_create4(double hertz, Handle modulation, double index);

        [DllImport("syntacts-c")]
        public static extern Handle Square_create1(Handle x);
        [DllImport("syntacts-c")]
        public static extern Handle Square_create2(double hertz);
        [DllImport("syntacts-c")]
        public static extern Handle Square_create3(double initial, double rate);
        [DllImport("syntacts-c")]
        public static extern Handle Square_create4(double hertz, Handle modulation, double index);

        [DllImport("syntacts-c")]
        public static extern Handle Saw_create1(Handle x);
        [DllImport("syntacts-c")]
        public static extern Handle Saw_create2(double hertz);
        [DllImport("syntacts-c")]
        public static extern Handle Saw_create3(double initial, double rate);
        [DllImport("syntacts-c")]
        public static extern Handle Saw_create4(double hertz, Handle modulation, double index);

        [DllImport("syntacts-c")]
        public static extern Handle Triangle_create1(Handle x);
        [DllImport("syntacts-c")]
        public static extern Handle Triangle_create2(double hertz);
        [DllImport("syntacts-c")]
        public static extern Handle Triangle_create3(double initial, double rate);
        [DllImport("syntacts-c")]
        public static extern Handle Triangle_create4(double hertz, Handle modulation, double index);

        [DllImport("syntacts-c")]
        public static extern Handle Pwm_create(double frequency, double dutyCycle);


        [DllImport("syntacts-c")]
        public static extern Handle Sequence_create();
        [DllImport("syntacts-c")]
        public static extern Handle Sequence_SigSig(Handle lhs, Handle rhs);
        [DllImport("syntacts-c")]
        public static extern Handle Sequence_SigFlt(Handle lhs, double rhs);
        [DllImport("syntacts-c")]
        public static extern Handle Sequence_FltSig(double lhs, Handle rhs);
        [DllImport("syntacts-c")]
        public static extern void Sequence_SeqFlt(Handle lhs, double rhs);
        [DllImport("syntacts-c")]
        public static extern void Sequence_SeqSig(Handle lhs, Handle rhs);
        [DllImport("syntacts-c")]
        public static extern void Sequence_SeqSeq(Handle lhs, Handle rhs);

        [DllImport("syntacts-c", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool Library_saveSignal(Handle signal, string name);
        [DllImport("syntacts-c", CallingConvention = CallingConvention.Cdecl)]
        public static extern Handle Library_loadSignal(string name);

        [DllImport("syntacts-c")]
        public static extern int Debug_sigMapSize();

    }
}

// https://docs.microsoft.com/en-us/dotnet/standard/garbage-collection/implementing-dispose
// https://michaelscodingspot.com/ways-to-cause-memory-leaks-in-dotnet/