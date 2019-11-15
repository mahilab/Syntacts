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

        internal Handle handle = Handle.Zero;
    }

    class EnvelopeBase : Signal {

    }

    class Envelope : EnvelopeBase
    {
        public Envelope(float duration)
        {
            handle = Dll.Envelope_create(duration);
        }
    }

    class ASR : EnvelopeBase {
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

    class Cue : Signal
    {
        public Cue() {
            handle = Dll.Cue_create();
        }

        public void SetEnvelope(EnvelopeBase env)
        {
            Dll.Cue_setEnvelope(handle, env.handle);
        }

        public void Chain(Signal sig) 
        {
            Dll.Cue_chain(handle, sig.handle);
        }

    }

    class Session : IDisposable
    {
        public Session()
        {
            handle = Dll.Session_create();
        }

        public int Open(int index, int channelCount)
        {
            return Dll.Session_open(handle, index, channelCount);
        }

        public int Close()
        {
            return Dll.Session_close(handle);
        }

        public int Play(int channel, Cue cue) {
            return Dll.Session_play(handle, channel, cue.handle);
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

        private Handle handle = Handle.Zero;

    }

    class Dll
    {

        [DllImport("syntacts-plugin")]
        public static extern Handle Session_create();
        [DllImport("syntacts-plugin")]
        public static extern void Session_delete(Handle session);
        [DllImport("syntacts-plugin")]
        public static extern int Session_open(Handle session, int index, int channelCount);
        [DllImport("syntacts-plugin")]
        public static extern int Session_close(Handle session);
        [DllImport("syntacts-plugin")]
        public static extern int Session_play(Handle session, int channel, Handle cue);
        [DllImport("syntacts-plugin")]
        public static extern int Session_stop(Handle session, int channel);
        [DllImport("syntacts-plugin")]
        public static extern int Session_pause(Handle session, int channel);
        [DllImport("syntacts-plugin")]
        public static extern int Session_resume(Handle session, int channel);
        [DllImport("syntacts-plugin")]
        public static extern int Session_setVolume(Handle session, int channel, float volume);

        [DllImport("syntacts-plugin")]
        public static extern bool Signal_valid(Handle sig);
        [DllImport("syntacts-plugin")]
        public static extern void Signal_delete(Handle sig);
        [DllImport("syntacts-plugin")]
        public static extern float Signal_sample(Handle sig, float t);
        [DllImport("syntacts-plugin")]
        public static extern int Signal_count();

        [DllImport("syntacts-plugin")]
        public static extern Handle Scalar_create(float value);
        [DllImport("syntacts-plugin")]
        public static extern Handle Ramp_create(float initial, float rate);

        [DllImport("syntacts-plugin")]
        public static extern Handle Envelope_create(float duration);
        [DllImport("syntacts-plugin")]
        public static extern Handle ASR_create(float a, float s, float r);
        [DllImport("syntacts-plugin")]
        public static extern Handle ADSR_create(float a, float d, float s, float r);

        [DllImport("syntacts-plugin")]
        public static extern Handle Sine_create(float frequency);
        [DllImport("syntacts-plugin")]
        public static extern Handle Square_create(float frequency);
        [DllImport("syntacts-plugin")]
        public static extern Handle Saw_create(float frequency);
        [DllImport("syntacts-plugin")]
        public static extern Handle Triangle_create(float frequency);

        [DllImport("syntacts-plugin")]
        public static extern Handle Cue_create();
        [DllImport("syntacts-plugin")]
        public static extern void Cue_setEnvelope(Handle cue, Handle env);
        [DllImport("syntacts-plugin")]
        public static extern void Cue_chain(Handle cue, Handle sig);

        [DllImport("syntacts-plugin", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool Library_saveCue(Handle cue, string name);
        [DllImport("syntacts-plugin", CallingConvention = CallingConvention.Cdecl)]
        public static extern Handle Library_loadCue(string name);

        [DllImport("syntacts-plugin")]
        public static extern int Debug_oscMapSize();
        [DllImport("syntacts-plugin")]
        public static extern int Debug_envMapSize();
        [DllImport("syntacts-plugin")]
        public static extern int Debug_cueMapSize();

    }
}

// https://docs.microsoft.com/en-us/dotnet/standard/garbage-collection/implementing-dispose
// https://michaelscodingspot.com/ways-to-cause-memory-leaks-in-dotnet/