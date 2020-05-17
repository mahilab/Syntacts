using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Syntacts;

public class LibraryButton : MonoBehaviour
{
    [Tooltip("Reference to a SyntactsHub in the scene.")]
    public SyntactsHub syntacts;
    [Tooltip("Name of Library signal to load and play.")]
    public string signalName = "mySignal";
    [Tooltip("The channel the signal will be played on.")]
    public int channel = 0;

    // Start is called before the first frame update
    void Start()
    {
        // make "mySignal" so demo works initially
        Signal mySignal = new Sine(440, 500) * new ASR(0.1,0.1,0.1);
        Library.SaveSignal(mySignal, "mySignal");
    }

    void OnGUI() {
        if (GUI.Button(new Rect(10, 10, 125, 25), "Play Library Signal")) {
            Signal signal;
            if (Library.LoadSignal(out signal, signalName)) {
                syntacts.session.Play(channel, signal);
            }
            else {
                Debug.LogError("Failed to load signal: " + signalName);
            }
        }
    }
}
