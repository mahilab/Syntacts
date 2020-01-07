using System;
using System.Runtime.InteropServices;

namespace Syntacts
{
    using Handle = System.IntPtr;

    class Signal : IDisposable
    {

        public float sample(float t) {
            return Dll.Signal_sample(handle, t);
        }

        public static int Count() {
            return Dll.Signal_count();
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

        public static Signal operator*(Signal lhs, Signal rhs) {
            return new Product(lhs, rhs);
        }

        internal Handle handle = Handle.Zero;
    }

    class Envelope : Signal
    {
        public Envelope(float duration) {
            handle = Dll.Envelope_create(duration);
        }
    }

    class ASR : Signal {
        public ASR(float a, float s, float r) {
            handle = Dll.ASR_create(a,s,r);
        }
    }

    class ADSR : Signal {
        public ADSR(float a, float d, float s, float r) {
            handle = Dll.ADSR_create(a,d,s,r);
        }
    }

    class Sine : Signal {
        public Sine(float frequency) 
        {
            handle = Dll.Sine_create(frequency);
        }
    }

    class Product : Signal {
        public Product(Signal lhs, Signal rhs) {
            handle = Dll.Product_create(lhs.handle, rhs.handle);
        }
    }

    class Sum : Signal {
        public Sum(Signal lhs, Signal rhs) {
            handle = Dll.Sum_create(lhs.handle, rhs.handle);
        }
    }

    class Session : IDisposable
    {
        public Session()
        {
            handle = Dll.Session_create();
        }

        public int Open(int index, int channelCount, double sampleRate = 0)
        {
            return Dll.Session_open(handle, index, channelCount, sampleRate);
        }

        public int Close()
        {
            return Dll.Session_close(handle);
        }

        public int Play(int channel, Signal signal) {
            return Dll.Session_play(handle, channel, signal.handle);
        }

        public int Stop(int channel)
        {
            return Dll.Session_stop(handle, channel);
        }

        public int Pause(int channel)
        {
            return Dll.Session_pause(handle, channel);
        }

        public int Resume(int channel)
        {
            return Dll.Session_resume(handle, channel);
        }

        public int SetVolume(int channel, float volume)
        {
            return Dll.Session_setVolume(handle, channel, volume);
        }

        public int SetPitch(int channel, float pitch)
        {
            return Dll.Session_setPitch(handle, channel, pitch);
        }

        public bool IsOpen() {
            return Dll.Session_isOpen(handle);
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

    class Library {
        public static Signal LoadSignal(string name) {
            Signal sig = new Signal();
            sig.handle = Dll.Library_loadSignal(name);
            return sig;
        }
    }

    class Dll
    {

        [DllImport("syntacts-c")]
        public static extern Handle Session_create();
        [DllImport("syntacts-c")]
        public static extern void Session_delete(Handle session);
        [DllImport("syntacts-c")]
        public static extern int Session_open(Handle session, int index, int channelCount, double sampeRate);
        [DllImport("syntacts-c")]
        public static extern int Session_close(Handle session);
        [DllImport("syntacts-c")]
        public static extern int Session_play(Handle session, int channel, Handle cue);
        [DllImport("syntacts-c")]
        public static extern int Session_stop(Handle session, int channel);
        [DllImport("syntacts-c")]
        public static extern int Session_pause(Handle session, int channel);
        [DllImport("syntacts-c")]
        public static extern int Session_resume(Handle session, int channel);
        [DllImport("syntacts-c")]
        public static extern int Session_setVolume(Handle session, int channel, float volume);
        [DllImport("syntacts-c")]
        public static extern int Session_setPitch(Handle session, int channel, float pitch);
        [DllImport("syntacts-c")]
        public static extern bool Session_isOpen(Handle session);
        [DllImport("syntacts-c")]
        public static extern int Session_count();

        [DllImport("syntacts-c")]
        public static extern bool Signal_valid(Handle sig);
        [DllImport("syntacts-c")]
        public static extern void Signal_delete(Handle sig);
        [DllImport("syntacts-c")]
        public static extern float Signal_sample(Handle sig, float t);
        [DllImport("syntacts-c")]
        public static extern float Signal_length(Handle sig);
        [DllImport("syntacts-c")]
        public static extern int Signal_count();

        [DllImport("syntacts-c")]
        public static extern Handle Scalar_create(float value);
        [DllImport("syntacts-c")]
        public static extern Handle Ramp_create(float initial, float rate);

        [DllImport("syntacts-c")]
        public static extern Handle Envelope_create(float duration);
        [DllImport("syntacts-c")]
        public static extern Handle ASR_create(float a, float s, float r);
        [DllImport("syntacts-c")]
        public static extern Handle ADSR_create(float a, float d, float s, float r);

        [DllImport("syntacts-c")]
        public static extern Handle Sine_create(float frequency);
        [DllImport("syntacts-c")]
        public static extern Handle Square_create(float frequency);
        [DllImport("syntacts-c")]
        public static extern Handle Saw_create(float frequency);
        [DllImport("syntacts-c")]
        public static extern Handle Triangle_create(float frequency);

        [DllImport("syntacts-c")]
        public static extern Handle Product_create(Handle lhs, Handle rhs);
        [DllImport("syntacts-c")]
        public static extern Handle Sum_create(Handle lhs, Handle rhs);


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