using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using System;
using System.Runtime.InteropServices;

using Handle = System.IntPtr;

public class Syntacts : MonoBehaviour
{

    public string cueName;

    Handle session;

    // class ASR {
    //     public ASR(float a, float s, float r) { handle = ASR_create(a,s,r); }
    //     ~ASR() { Envelope_delete(handle); }
    //     private Handle handle;    
    // }

    // Start is called before the first frame update
    void Start()
    {
        session = Session_create();
        Session_open(session, 8, 2);
    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetKeyDown(KeyCode.M)) {
            print("Signals: " + Signal_count());
        }

        if (Input.GetKeyDown(KeyCode.Space)) {
            var sin = Sine_create(440);
            var asr = ASR_create(1,3,1);
            var cue = Cue_create();
            Cue_setEnvelope(cue, asr);
            Cue_chain(cue, sin);
            Session_play(session, 0, cue);
            Session_play(session, 1, cue);
            Signal_delete(sin);
            Signal_delete(asr);
            Signal_delete(cue);
        }

        if (Input.GetKeyDown(KeyCode.L)) {
            var cue = Library_loadCue(cueName);
            if (cue != Handle.Zero) {
                Session_play(session, 0, cue);
                Session_play(session, 1, cue);
                Signal_delete(cue);
            }
            else {
                print("Cue could not be loaded!");
            }
        }
    }

    void OnDestroy() 
    {
        Session_close(session);
        Session_delete(session);
    }

    void DebugCounts() {
        
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