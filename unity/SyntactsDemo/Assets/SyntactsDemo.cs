using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using System;
using System.Runtime.InteropServices;

using Syntacts;

public class SyntactsDemo : MonoBehaviour
{

    public string cueName;

    Session session;

    // Start is called before the first frame update
    void Start()
    {
        session = new Session();
        session.Open(3,1);
        Signal signal = new Signal();
    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetKeyDown(KeyCode.Space)) {
            Cue cue = new Cue();
            cue.SetEnvelope(new ASR(1,3,1));
            cue.Chain(new Sine(440));
            if (Input.GetKey(KeyCode.LeftControl))
                cue.Chain(new Sine(20));
            session.Play(0, cue);
        }

        if (Input.GetKeyDown(KeyCode.C)) {
            print(Signal.Count());
        }
    }

    void OnDestroy() 
    {
        session.Close();
        session.Dispose();
    }

}