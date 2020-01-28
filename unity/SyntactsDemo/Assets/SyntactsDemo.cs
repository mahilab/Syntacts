using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using Syntacts; // use Syntacts namespace

public class SyntactsDemo : MonoBehaviour
{
    [Header("Device Initialization")]
    public int deviceIndex = 0;

    [Header("Device Info")]
    public bool open = false;
    public bool valid = false;
    public int channelCount = 0;


    [Header("Playback")]
    public string librarySignal = "mySignal";
    [Range(0, 1)]
    public float volume = 1;
    [Range(0.01f,10)]
    public float pitch = 1;

    Session session;

    void Awake() {
        session = new Session();
        session.Open(deviceIndex);
        channelCount = session.ChannelCount;
    }

    void Update() {
        open = session.IsOpen();
        valid = session.Valid;
    }

    void OnDestroy() 
    {
        session.Close();
        session.Dispose();
    }
}