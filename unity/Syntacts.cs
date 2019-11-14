using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using System;
using System.Runtime.InteropServices;

using Handle = System.IntPtr;

public class Syntacts : MonoBehaviour
{
    Handle session;

    class ASR {
        public ASR(float a, float s, float r) { handle = ASR_create(a,s,r); }
        ~ASR() { Envelope_delete(handle); }
        private Handle handle;    
    }

    // Start is called before the first frame update
    void Start()
    {
        session = Session_create();
        print(session);
        Session_open(session, 16, 20);
        DebugMaps();
        ASR asr = new ASR(1,2,1);
        DebugMaps();
    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetKeyDown(KeyCode.M)) {
            DebugMaps();
        }

        if (Input.GetKeyDown(KeyCode.Space)) {
            var sin = SineWave_create(440);
            var asr = ASR_create(1,3,1);
            var cue = Cue_create(sin, asr);
            Session_play(session, 0, cue);
            Oscillator_delete(sin);
            Envelope_delete(asr);
            Cue_delete(cue);
        }
    }

    void OnDestroy() 
    {
        Session_close(session);
        Session_delete(session);
    }

    void DebugMaps() {
        print("Cue: " + Debug_cueMapSize());
        print("Osc: " + Debug_oscMapSize());
        print("Env: " + Debug_envMapSize());
    }


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
    public static extern Handle Envelope_create(float duration);
    [DllImport("syntacts-plugin")]
    public static extern void Envelope_delete(Handle env);
    [DllImport("syntacts-plugin")]
    public static extern Handle ASR_create(float a, float s, float r);
    [DllImport("syntacts-plugin")]
    public static extern Handle ADSR_create(float a, float d, float s, float r);

    [DllImport("syntacts-plugin")]
    public static extern void Oscillator_delete(Handle osc);
    [DllImport("syntacts-plugin")]
    public static extern Handle SineWave_create(float frequency);
    [DllImport("syntacts-plugin")]
    public static extern Handle SquareWave_create(float frequency);

    [DllImport("syntacts-plugin")]
    public static extern Handle Cue_create(Handle osc, Handle env);
    [DllImport("syntacts-plugin")]
    public static extern void Cue_delete(Handle cue);

    [DllImport("syntacts-plugin")]
    public static extern int Debug_oscMapSize();
    [DllImport("syntacts-plugin")]
    public static extern int Debug_envMapSize();
    [DllImport("syntacts-plugin")]
    public static extern int Debug_cueMapSize();
}
