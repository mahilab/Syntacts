using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Syntacts;

[DisallowMultipleComponent]
public class SyntactsHub : MonoBehaviour
{
    /// The Syntacts session maintained by this SyntactsHub
    public Session session;
    public Device device {
        get { return currentDevice; }
    }

    public enum OpenMode {
        Default  = 0,
        ByIndex  = 1,
        ByName   = 2,
        ByAPI    = 3,
        Custom   = 4
    };
 
    [SerializeField]
    private OpenMode openMode = OpenMode.Default;
    [SerializeField]
    int index = 0;  
    [SerializeField]
    int channelCount = 2;   
    [SerializeField]
    int sampleRate = 44100; 

#if UNITY_STANDALONE_WIN || UNITY_EDITOR_WIN
    [SerializeField]     
    API deviceApi = API.MME;
#elif UNITY_STANDALONE_OSX || UNITY_EDITOR_OSX 
    [SerializeField]     
    API deviceApi = API.CoreAudio;
#else
    [SerializeField]     
    API deviceApi = API.Unknown;
#endif

    [SerializeField]
    string deviceName = "";

    private Device currentDevice;

    void Awake() {
        currentDevice = null;
        session = new Session();
        int result = -1;
        if (openMode == OpenMode.Default)
            result = session.Open();
        else if (openMode == OpenMode.ByAPI)
            result = session.Open(deviceApi);
        else if (openMode == OpenMode.ByIndex)
            result = session.Open(index);
        else if (openMode == OpenMode.Custom) 
            result = session.Open(index, channelCount, sampleRate);
        else if (openMode == OpenMode.ByName) {
            foreach (Device dev in session.availableDevices) {
                if (dev.name == deviceName && (deviceApi == API.Unknown || dev.api == deviceApi)) {
                    result = session.Open(dev.index);
                    break;
                }
            }
        }
        if (result != 0)
            Debug.LogError("<b>[Syntacts]</b> Failed to open Device (Error code: " + result.ToString() + ")");
        else {
            currentDevice = session.currentDevice;
            Debug.Log("<b>[Syntacts]</b> Opened Device " + currentDevice.index.ToString() + ": " + currentDevice.name);
        }
    }

    void OnApplicationQuit() {
        if (session != null && session.isOpen) {
            int result = session.Close();
            if (result != 0)
                Debug.LogError("<b>[Syntacts]</b> Failed to close Device (Error code: " + result.ToString() + ")");
            else
                Debug.Log("<b>[Syntacts]</b> Closed Device");
            session.Dispose();
        }
    }
}
