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
    SerializedProperty api;
    SerializedProperty name;

    bool showDeviceInfo = true;
    bool showAvailable = false;

    Dictionary<API, List<Device>> availableDevices;

    public virtual void OnEnable() {
        openMode = serializedObject.FindProperty("openMode");
        index = serializedObject.FindProperty("index");
        channelCount = serializedObject.FindProperty("channelCount");
        sampleRate = serializedObject.FindProperty("sampleRate");
        api = serializedObject.FindProperty("api");
        name = serializedObject.FindProperty("name");

        Session temp = new Session();
        availableDevices = new Dictionary<API, List<Device>>();
        foreach (Device dev in temp.availableDevices)
        {
            if (!availableDevices.ContainsKey(dev.api))
                availableDevices.Add(dev.api, new List<Device>());
            availableDevices[dev.api].Add(dev);
        }
        temp.Dispose();
    }

    public override void OnInspectorGUI() {

        SyntactsHub script = (SyntactsHub)target;

        serializedObject.Update();
        EditorGUI.BeginChangeCheck();
        EditorGUILayout.PropertyField(openMode, new GUIContent("Open Mode", "Determines how Devices will be opened."));

        if (openMode.enumValueIndex == (int)SyntactsHub.OpenMode.ByIndex)
            EditorGUILayout.PropertyField(index, new GUIContent("Device Index"));
        else if (openMode.enumValueIndex == (int)SyntactsHub.OpenMode.ByAPI)
            EditorGUILayout.PropertyField(api, new GUIContent("Device API"));
        else if (openMode.enumValueIndex == (int)SyntactsHub.OpenMode.Custom) {
            EditorGUILayout.PropertyField(index, new GUIContent("Device Index"));
            EditorGUILayout.PropertyField(channelCount, new GUIContent("Channel Count"));
            EditorGUILayout.PropertyField(sampleRate, new GUIContent("Sample Rate"));
        }
        else if (openMode.enumValueIndex == (int)SyntactsHub.OpenMode.ByName) {
            EditorGUILayout.PropertyField(name, new GUIContent("Device Name"));
            EditorGUILayout.PropertyField(api,  new GUIContent("Device API"));
        }

        if (EditorGUI.EndChangeCheck())
            serializedObject.ApplyModifiedProperties();


        if (script.session != null && script.session.isOpen) {
             showDeviceInfo = EditorGUILayout.BeginFoldoutHeaderGroup(showDeviceInfo, "Current Device");
             if (showDeviceInfo) {
                EditorGUILayout.LabelField("Index:", script.device.index.ToString());
                EditorGUILayout.LabelField("Name:",  script.device.name.ToString());
                EditorGUILayout.LabelField("Default:", script.device.isDefault.ToString());
                EditorGUILayout.LabelField("API:", script.device.apiName.ToString());
                EditorGUILayout.LabelField("API Default:", script.device.isDefault.ToString());
                EditorGUILayout.LabelField("Max Channels:", script.device.maxChannels.ToString());
             }
             EditorGUILayout.EndFoldoutHeaderGroup();
        }

        showAvailable = EditorGUILayout.BeginFoldoutHeaderGroup(showAvailable, "Available Devices");
        if (showAvailable) {
            foreach (var pair in availableDevices) {
                EditorGUILayout.LabelField(pair.Key.ToString(), EditorStyles.boldLabel);
                foreach (Device dev in pair.Value)
                    EditorGUILayout.LabelField(dev.index.ToString(), dev.name);
            }
        }        
        EditorGUILayout.EndFoldoutHeaderGroup();

        // if (GUILayout.Button("Open Session")) {

            

        // }
        // if (GUILayout.Button("Close Session")) {

        // }

        
    }
}
