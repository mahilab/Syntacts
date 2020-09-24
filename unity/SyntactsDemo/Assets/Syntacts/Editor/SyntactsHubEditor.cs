using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;
using System.Text;
using Syntacts;

[CustomEditor(typeof(SyntactsHub))]
public class SyntactsHubEditor : Editor
{
    SerializedProperty openMode;
    SerializedProperty index;
    SerializedProperty channelCount;
    SerializedProperty sampleRate;
    SerializedProperty deviceApi;
    SerializedProperty deviceName;

    bool showDeviceInfo = true;
    bool showAvailable = false;
    bool showChannelInfo = false;
    bool showDebug = false;

    GUIStyle italicLabel;
    GUIStyle errorLabel;

    Dictionary<API, List<Device>> availableDevices;

    string version;
    bool asio;

    public virtual void OnEnable()
    {
        openMode = serializedObject.FindProperty("openMode");
        index = serializedObject.FindProperty("index");
        channelCount = serializedObject.FindProperty("channelCount");
        sampleRate = serializedObject.FindProperty("sampleRate");
        deviceApi = serializedObject.FindProperty("deviceApi");
        deviceName = serializedObject.FindProperty("deviceName");

        italicLabel = new GUIStyle();
        italicLabel.fontStyle = FontStyle.Italic;
        errorLabel = new GUIStyle();
        errorLabel.fontStyle = FontStyle.Bold;
        errorLabel.normal.textColor = Color.red;

        Session temp = new Session();
        availableDevices = new Dictionary<API, List<Device>>();
        foreach (Device dev in temp.availableDevices)
        {
            if (!availableDevices.ContainsKey(dev.api))
                availableDevices.Add(dev.api, new List<Device>());
            availableDevices[dev.api].Add(dev);
        }
        temp.Dispose();

        version = Syntacts.Version.text;
        asio = Syntacts.Version.asio;
    }

    public override void OnInspectorGUI()
    {
        SyntactsHub script = (SyntactsHub)target;

        serializedObject.Update();

        EditorGUI.BeginDisabledGroup(Application.isPlaying);
            
        EditorGUI.BeginChangeCheck();
        EditorGUILayout.PropertyField(openMode, new GUIContent("Open Mode", "Determines how Devices will be opened."));

        if (openMode.enumValueIndex == (int)SyntactsHub.OpenMode.ByIndex)
            EditorGUILayout.PropertyField(index, new GUIContent("Device Index"));
        else if (openMode.enumValueIndex == (int)SyntactsHub.OpenMode.ByAPI)
            EditorGUILayout.PropertyField(deviceApi, new GUIContent("Device API"));
        else if (openMode.enumValueIndex == (int)SyntactsHub.OpenMode.Custom)
        {
            EditorGUILayout.PropertyField(index, new GUIContent("Device Index"));
            EditorGUILayout.PropertyField(channelCount, new GUIContent("Channel Count"));
            EditorGUILayout.PropertyField(sampleRate, new GUIContent("Sample Rate"));
        }
        else if (openMode.enumValueIndex == (int)SyntactsHub.OpenMode.ByName)
        {
            EditorGUILayout.PropertyField(deviceName, new GUIContent("Device Name"));
            EditorGUILayout.PropertyField(deviceApi, new GUIContent("Device API"));
        }

        if (EditorGUI.EndChangeCheck())
            serializedObject.ApplyModifiedProperties();
        
        EditorGUI.EndDisabledGroup();


        showDeviceInfo = EditorGUILayout.BeginFoldoutHeaderGroup(showDeviceInfo, "Device");
        if (showDeviceInfo)
        {

            if (!Application.isPlaying) {
                EditorGUI.BeginDisabledGroup(true);
                EditorGUILayout.LabelField("Only available when the Application is running.", italicLabel);
                EditorGUI.EndDisabledGroup();
            }
            else if (script.session != null && script.session.isOpen)
            {
                EditorGUILayout.LabelField("Index:", script.device.index.ToString());
                EditorGUILayout.LabelField("Name:", script.device.name.ToString());
                EditorGUILayout.LabelField("Default:", script.device.isDefault.ToString());
                EditorGUILayout.LabelField("API:", script.device.apiName.ToString());
                EditorGUILayout.LabelField("API Default:", script.device.isDefault.ToString());
                EditorGUILayout.LabelField("Max Channels:", script.device.maxChannels.ToString());
                EditorGUILayout.LabelField("Channels:", script.session.channelCount.ToString());
                EditorGUILayout.LabelField("Sample Rate:", script.session.sampleRate.ToString());
                EditorGUILayout.LabelField("CPU Load:", Mathf.Round((float)script.session.cpuLoad * 100).ToString()+"%");
            }
            else {
                EditorGUILayout.LabelField("Failed to open Device!",errorLabel);
            }
        }
        EditorGUILayout.EndFoldoutHeaderGroup();

        showChannelInfo = EditorGUILayout.BeginFoldoutHeaderGroup(showChannelInfo, "Channels");
        if (showChannelInfo) {
            // EditorGUI.BeginDisabledGroup(true);
            if (!Application.isPlaying) {
                EditorGUI.BeginDisabledGroup(true);
                EditorGUILayout.LabelField("Only available when the Application is running.", italicLabel);
                EditorGUI.EndDisabledGroup();
            }
            else if (script.session != null && script.session.isOpen) {
                int count = script.session.channelCount;
                for (int i = 0; i < count; ++i) {
                    EditorGUILayout.LabelField("Channel " + i.ToString(), EditorStyles.miniBoldLabel);
                    EditorGUI.BeginChangeCheck();
                    float vol = EditorGUILayout.Slider("Volume", (float)script.session.GetVolume(i), 0, 1); 
                    if (EditorGUI.EndChangeCheck())
                        script.session.SetVolume(i, vol);

                    EditorGUI.BeginChangeCheck();
                    float pitch = EditorGUILayout.Slider("Pitch",  (float)script.session.GetPitch(i), 0, 10);
                    if (EditorGUI.EndChangeCheck())
                        script.session.SetPitch(i, pitch);
                    
                    EditorGUILayout.Slider("Level", (float)script.session.GetLevel(i), 0, 1);
                }
            }
            else {
                EditorGUILayout.LabelField("Failed to open Device!",errorLabel);
            }
            // EditorGUI.EndDisabledGroup();
        }
        EditorGUILayout.EndFoldoutHeaderGroup();
        

        showAvailable = EditorGUILayout.BeginFoldoutHeaderGroup(showAvailable, "Available Devices");
        if (showAvailable)
        {
            foreach (var pair in availableDevices)
            {
                EditorGUILayout.LabelField(pair.Key.ToString(), EditorStyles.boldLabel);
                foreach (Device dev in pair.Value)
                    EditorGUILayout.LabelField(dev.index.ToString(), dev.name);
            }
        }
        EditorGUILayout.EndFoldoutHeaderGroup();


        showDebug = EditorGUILayout.BeginFoldoutHeaderGroup(showDebug, "Debug");
        if (showDebug) {
            EditorGUILayout.LabelField("Syntacts Version:", version);
            EditorGUILayout.LabelField("ASIO Support:", asio.ToString());
            EditorGUILayout.LabelField("Session Count:", Syntacts.Session.count.ToString());
            EditorGUILayout.LabelField("Signal Count:", Syntacts.Signal.count.ToString());
        }
        EditorGUILayout.EndFoldoutHeaderGroup();
    }

    public override bool RequiresConstantRepaint() {
        return true;
    }
}
