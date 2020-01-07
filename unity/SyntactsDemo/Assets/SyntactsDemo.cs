using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using System;
using System.Runtime.InteropServices;

using Syntacts;

public class SyntactsDemo : MonoBehaviour
{

    public int device;
    public int channeCount;
    public int channel = 0;
    public string signalName;
    public bool open;

    public AnimationCurve curve;

    [Range(0,10)]
    public float pitch = 1;

    Session session;

    // Start is called before the first frame update
    void Start()
    {
        session = new Session();
        session.Open(device, channeCount);
    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetKeyDown(KeyCode.Space)) {
            var sine = new Sine(440);
            var mod  = new Sine(10);
            var adsr = new ADSR(1,1,1,1);
            var prod = sine * mod;
            session.Play(channel, prod);
        }

        if (Input.GetKeyDown(KeyCode.X))
            session.Stop(channel);

        if (Input.GetKeyDown(KeyCode.C)) {
            print(Signal.Count());
        }

        if (Input.GetKeyDown(KeyCode.L)) {
            var sig = Library.LoadSignal(signalName);
            session.Play(channel, sig);
        }

        session.SetPitch(channel, pitch);

        open = session.IsOpen();
    }

    void OnDestroy() 
    {
        session.Close();
        session.Dispose();
    }

}