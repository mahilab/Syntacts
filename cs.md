---
layout: page
title: C# API
permalink: /cs/
---

## Introduction
This tutorial will teach you how to setup and use the Syntacts C# API. For this purpose, a C# IDE such as Visual Studio is required.
 
## Requirements

- C# compiler such as .NET

## Basic Setup

- Download the [latest pre-compiled Syntacts binaries](https://github.com/mahilab/Syntacts/releases) for your system. 
- Extract `syntacts-c.dll` and add it to each project folder that you want to run. 

## Building

- Navigate to the csharp directory of the latest release of Syntacts
- Build the Syntacts library from the command line and then run the required project.

```shell
  > cd csharp/Syntacts
  > dotnet build
  > cd ../examples/example_basic
  > dotnet run
```

## API Overview

# Session

- Using Syntacts begins with creating a `Session`, or an audio-context that manages communication with an output device. 
- Devices available for opening can be enumerated and queried for information using a `Session`: 

```cs
Session session = new Session();
        
foreach (Device dev in session.availableDevices) {
    Console.WriteLine("");
    Console.WriteLine("Index:        {0}", dev.index);
    Console.WriteLine("Name:         {0}", dev.name); 
    Console.WriteLine("API:          {0}", dev.apiName); 
    Console.WriteLine("Max Channels: {0}", dev.maxChannels); 
}
```

- Devices can be opened using the `Open` function. 
- The no-argument overload will open the system default audio device (usually the main speakers). 
- Non-default devices can be opened by passing a device index argument or the device name and preferred API:

```cs
// open default device
session.Open();   
session.Close();  
// open device by index
session.Open(22); 
session.Close();  
// open device by name and preferred API
session.Open("MOTU Pro Audio", API::ASIO);
session.Close();
```

- Device indices represent the combination of a device and an API it supports. Therefore, one device may have multiple indices each with a different API. 
- You can retrieve your device's index and/or name by iterating as in the above example, or by opening the [Syntacts GUI](gui.md) and viewing the information there.

> **Warning:** Device indices are NOT persistent. They may change when devices are power cycled, plugged/unplugged, or when other devices are connected to the PC. If you need a persistent method of opening a device, use the **name + API** version of `open`.

|Relevant Examples(s)|
|---|
|[example_devices.cs](https://github.com/mahilab/Syntacts/blob/master/csharp/examples/example_devices/example_devices.cs)|

# Signals

- Vibration waveforms are represented by one or more Signals. You can create Signals of different behaviors and lengths.
- Sine, Square, Saw, and Triangle classes are available in Syntacts. These implement typical oscillators with normalized amplitude and infinite duration. 
- Basic oscillators can be created by calling their constructors:

```cs
Signal sin = new Sine(10);      // 10 Hz Sine wave
Signal sqr = new Square(250);   // 250 Hz Square wave
Signal saw = new Saw(175);      // 175 Hz Saw wave
Signal tri = new Triangle(440); // 440 Hz Triangle wave (audible and ok for speakers)
```
- Oscillators alone have an *infinite* duration or length and a constant amplitude. 
- You can use the Envelope, ASR, and ADSR (Attack, (Decay), Sustain, Release) Signals to define amplitude modifiers with *finite* duration:

```cs
// This is a basic envelope that specifies amplitude (0.9), and duration (0.5 sec)
Signal bas = new Envelope(0.9, 0.5);
// This is an attack (1 sec), sustain (3 sec), release (1 sec) envelope. The sustain amplitude is 1.0. 
Signal asr = new ASR(1, 2, 1, 1.0);
```

- Signals can be mixed using basic arithmetic. Multiplying and adding Signals can be thought of as an element-wise operation between two vectors.
    - Multiplying two Signals creates a new Signal of duration equal to the    shortest operand.  
    - Adding two Signals creates a new Signal of duration equal to the longest operand.
- Gain and bias can also be applied to Signals with scalar operands.
- Below are basic examples of mixing the Signals from above:

```cs
Signal sig1 = sqr * sin;  // duration is infinite
Signal sig2 = sig1 * asr; // duration is 0.3 seconds
Signal sig3 = 0.5 * (sqr + sin) * asr;
```
- Signals created in the above examples:
![Signals](https://raw.githubusercontent.com/wiki/mahilab/Syntacts/images/sigs.png)

- Complete signals can be made and mixed in-place as well.

```cs
// 250 Hz square wave amplitude modulated with a 20 Hz sine wave and ASR envelope.
Signal sig4 = new Square(250) * new Sine(20) * new ASR(1,1,1);
```

- Once you have created a complete Signal, it can be played on the Session.

```cs
session.Play(0, sig1); // plays sig 1 on channel 0
Sleep(3);              // sig1 plays for 3 seconds
session.Stop(0)        // stop sig1

session.Play(1, sig2); // plays sig2 on channel 1
Sleep(sig2.length);    // sig2 plays for its length of 0.3 seconds
                       // Do not need to stop sig2 because it is finite

session.PlayAll(sig3)  // plays sig3 on all channels
Sleep(sig3.length)
```

> **Note:** If you want to evaluate Syntacts without tactor hardware, make sure the frequency is high enough to be audible for speakers (e.g. 440 Hz). Low frequencies can potentially damage speakers! For this reason, most of the Syntacts examples are coded with frequencies in the hearing range. If you are actually outputting to a tactor, use frequencies appropriate for it (e.g. 150-250 Hz).

|Relevant Examples(s)|
|---|
|[example_basic.cs](https://github.com/mahilab/Syntacts/blob/master/csharp/examples/example_basic/example_basic.cs)

# Sequences

- Multiple Signals can be ordered in time using Sequences.
- You can concatenate Signals using Push().
- Delay and pause are achieved through the insertion of positive scalar operands.
- Negative scalar operands move the insertion points backwards in time, allowing for the overlay or fading of Signals into each other.
- Below is a basic example of creating Sequences:

```cs
Signal sigA = new Sine(440) * new ASR(1,1,1);      // create 3 second Signal
Signal sigB = new Square(440) * new ADSR(1,1,1,1); // create 4 second Signal

// 7 second Sequence with sigA before sigB
Sequence sig4 = new Sequence();
sig4.Push(sigA).Push(sigB); 
// 1 sec delay and 2 sec pause, 10 sec Sequence
Sequence sig5 = new Sequence();
sig5.Push(1).Push(sigA).Push(2).Push(sigB); 
// 1 sec fade/overlay between sigA and sigB, 6 sec sequence
Sequence sig6 = new Sequence();
sig6.Push(sigA).Push(-1).Push(sigB); 
```

- Sequenced signals created above:
![Sequences](https://raw.githubusercontent.com/wiki/mahilab/Syntacts/images/seq.png)

- Sequences can also be concatenated:

```cs
// Sequence of sig4, sig5, and sig6. Note this will also modify sig4.
Sequence sig7 = new Sequence();
sig7.Push(sig4).Push(sig5).Push(sig6); 
// Play sig7 on channel 0 for its length of 23 seconds
session.Play(0, sig7)
Sleep(sig7.length)
```

- You can also insert Signals into an existing Sequence timeline:

```cs
// insert 1 s of noise starts at the 4 second mark of sig5
sig5.Insert(new Noise() * new Envelope(1), 4); 
// Play sig5 on channel 0
session.PlayAll(signal5);
Sleep(signal5.length);
```

|Relevant Examples(s)|
|---|
|[example_sequences.cs](https://github.com/mahilab/Syntacts/blob/master/csharp/examples/example_sequences/example_sequences.cs)|

# Spatializers

- Spatializers map multiple channels to a normalized (0 to 1) continuous 1D or 2D spatial representation.
- For example, you can configure a virtual grid to match the physical layout of a tactor array.
- You can then set a virtual target coordinate and radius to play and blend multiple tactors at once.
- Only channels within a target radius are played.
- The volume of channels is interpolated according to a specified roll-off law (Linear=0, Smoothstep=1, Smootherstep=2, Smootheststep=3, Logarithmic=4, Exponential=5) based on their proximity to the target location using. The roll-off law can be changed with `rollOff`.
- Below is an example of creating a spatializer: 

```cs
Spatializer spatial = new Spatializer(session);     // create 2D Spatializer

spatial.CreateGrid(4,6);                            // Grid of 4 rows x 6 cols
spatial.SetPosition(18, new Point(0.1,0.8));        // move channel 18 by x = 0.1, y = 0.8
spatial.radius = 0.3;                               // effect radius
spatial.rollOff = 0;                                // set roll off method (Linear=0)
spatial.target = new Point(0.2, 0.1);               // target location
spatial.Play(sig1);                                 // play Signal
sleep(3);                                           // wait 3 seconds while the Signal plays
```

- To create sweeping motions with tactile arrays, you can move the target location in a `while` or `for` loop along a predescribed path.
- Master volume and pitch of the Spatializer can also be modified using `volume` and `pitch`.

```cs
while (condition) {
    spatial.target = new Point(x,y); 
    spatial.volume = v;
    spatial.pitch = p;
}
```

- The Spatializer created in the examples above:
![Spatializers](https://raw.githubusercontent.com/wiki/mahilab/Syntacts/images/spatial.png)

- Channel positions can be set as uniform grids (as above) or individually using `SetPosition`.
- Below is an example of custom channel positioning:

```cs
int chs = session.channelCount;
// set up position of channels, evenly distributed
double spc = 1.0 / (chs - 1);
for (int i = 0; i < chs; ++i)
    spatial.SetPosition(i, new Point(i * spc, 0));
```

|Relevant Examples(s)|
|---|
|[example_spatializer.cs](https://github.com/mahilab/Syntacts/blob/master/csharp/examples/example_spatializer/example_spatializer.cs)|

# Library

- Signals can be saved to a disk and reloaded at a later time using Library.
- In Syntacts, the default file format is a binary representation of the serialized Signal. 
    - This saves only the parameters needed to reconstruct the Signal at runtime instead of saving all individual audio samples.
    - Using this format creates smaller files which can be loaded more quickly on the fly than other audio file formats.
- To save and import in the default location (i.e. APPDATA/Syntacts/Library), use the functions `SaveSignal` and `LoadSignal`:
    
```cs
// Make a Signal to save/export, and a blank Signal to import in to
Signal save = new Sine(440) * new ASR(1,1,1);
Signal loaded;

// Syntacts Binary Format (Default Location, i.e. APPDATA/Syntacts/Library)
Library.SaveSignal(save, "out");
Library.LoadSignal(out loaded, "out");
```

- Signals can be saved and imported in other file locations using the functions `ExportSignal` and `ImportSignal`.
- You can customize the file location by specifying the file path. This is changed using quotation marks: `"file path"`.
- Files can be saved with a relative or absolute path:

```cs
// Binary format saved in a custom location
// Export/import with a relative path
Library.ExportSignal(save, "relative/folder/out.sig");
Library.ImportSignal(out loaded, "relative/folder/out.sig");

// Export/import with an absolute path
Library.ExportSignal(save, "/absolute/folder/out.sig");
Library.ImportSignal(out loaded, "/absolute/folder/out.sig");
```

- Syntacts can also export and import JSON, WAV, and AIFF file formats to use with existing haptic libraries. CSV and TXT files can only be exported.
- To use the above file formats, simply change the file format in your path to your desired file type:

```cs
// JSON format
Library.ExportSignal(save, "folder/out.json");
Library.ImportSignal(out loaded, "folder/out.json");

// WAV format - can use .wav, .WAV, or .wave
Library.ExportSignal(save, "folder/out.wav");
Library.ImportSignal(out loaded, "folder/out.wav");

// AIFF format - can use .aiff, .AIFF, or .aifc
Library.ExportSignal(save, "folder/out.aiff");
Library.ImportSignal(out loaded, "folder/out.aiff");

// CSV format (import not yet supported)
Library.ExportSignal(save, "folder/out.csv");

// TXT format (import not yet supported)
Library.ExportSignal(save, "folder/out.txt");
```

|Relevant Examples(s)|
|---|
|[example_library.cs](https://github.com/mahilab/Syntacts/blob/master/csharp/examples/example_library/example_library.cs)|
