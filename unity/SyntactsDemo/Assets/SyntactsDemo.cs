using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using Syntacts; // use Syntacts namespace

public class SyntactsDemo : MonoBehaviour
{
    [Header("Device Initialization")]
    [Tooltip("Determine which device index to use through the Syntacts GUI or by running devices.exe")]
    public int deviceIndex = 0;

    [Header("Device Monitor (FYI Only)")]
    public bool open = false;
    public bool valid = false;
    public int channelCount = 0;
    public double sampleRate = 0;
    public double cpuLoad = 0;
    public int count;

    [Header("Library Playback")]
    public int channel;
    [Range(0, 1)]
    public float volume = 1;
    [Range(0.01f,10)]
    public float pitch = 1;
    [Tooltip("Enter a Library Signal name and press L to load and play it")]
    public string librarySignal = "mySignal";

    public Session session;

    void Awake() {
        session = new Session();
        session.Open(deviceIndex);
        channelCount = session.channelCount;
        sampleRate = session.sampleRate;

        // make default library signal so demo works out of box
        Signal mySignal = new Sine(440, 1000) * new ADSR(.1,.1,.1,.1);
        Library.SaveSignal(mySignal, "mySignal");
    }

    void Update() {
        // update monitor 
        open = session.open;
        valid = session.valid;
        cpuLoad = session.cpuLoad;
        count = Session.count;

        if (channel < channelCount)
        {
            session.SetVolume(channel, volume);
            session.SetPitch(channel, pitch);
        }

        if (Input.GetKeyDown(KeyCode.L)) {
            Signal libSig;
            if (channel < channelCount) {
                if (Library.LoadSignal(out libSig, librarySignal)) {                    
                    session.Play(channel, libSig);                    
                }
                else
                    print("Failed to load Signal " + librarySignal);
            }
            else 
                print("Channel " + channel.ToString() + " exceeds this Device's channel count");
        }
    }

    void OnDestroy() 
    {
        session.Close();
        session.Dispose();
    }
}