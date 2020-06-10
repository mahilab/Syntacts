---
layout: page
title: Python API
permalink: /python/
---

## Introduction

This tutorial will teach you how to setup and use the Syntacts Python API. 
You should have first built and installed the Syntacts library to your system. 
For this tutorial we will use IDLE to edit and manage our project. 
However, the process should be similar if you are using another IDE such as PyCharm.

## Requirements

- [Python](https://www.python.org/downloads/) interpreter

## Basic Setup

- If you haven't already, [build and install](building.md) Syntacts.
- Install [Python](https://www.python.org/downloads/) on your device if it is not already installed.
- Create a new script in the same directory as `syntacts.py` and the dII.
- In the script, always include the following commands at the beginning:

```python
from syntacts import *
from time import sleep
from math import sin
from math import pi
```

## API Overview

# Session

- Using Syntacts begins with creating a `Session`, or an audio-context that manages communication with an output device. 
- Devices available for opening can be enumerated and queried for information using a `Session`: 

```python
session = Session()

for dev in session.available_devices:
    print("Index:       ", dev.index)
    print("Name:        ", dev.name)
    print("Max Channels:", dev.max_channels)
    print("API:         ", dev.api_name)
```

- Devices can be opened using the `open` function. 
- The no-argument overload will open the system default audio device (usually the main speakers). 
- Non-default devices can be opened by passing a device index argument or the device name and preferred API:

```python
// open default device
session.open();   
session.close();  
// open device by index
session.open(22); 
session.close();  
// open device by name and preferred API
session.open("MOTU Pro Audio", API::ASIO);
session.close();
```

- Device indices represent the combination of a device and an API it supports. Therefore, one device may have multiple indices each with a different API. 
- You can retrieve your device's index and/or name by iterating as in the above example, or by opening the [Syntacts GUI](gui.md) and viewing the information there.

> **Warning:** Device indices are NOT persistent. They may change when devices are power cycled, plugged/unplugged, or when other devices are connected to the PC. If you need a persistent method of opening a device, use the **name + API** version of `open`.

|Relevant Header(s)|Relevant Examples(s)|
|---|---|
|[Session.hpp](https://github.com/mahilab/Syntacts/blob/master/include/Tact/Session.hpp)|[example_devices.py](https://github.com/mahilab/Syntacts/blob/master/python/example_devices.py)|

# Signals

- Vibration waveforms are represented by one or more Signals. You can create Signals of different behaviors and lengths.
- Sine, Square, Saw, and Triangle classes are available in Syntacts. These implement typical oscillators with normalized amplitude and infinite duration. 
- Basic oscillators can be created by calling their constructors:

```python
sin = Sine(10)      # 10 Hz Sine wave
sqr = Square(250)   # 250 Hz Square wave
saw = Saw(175)      # 175 Hz Saw wave
tri = Triangle(440) # 440 Hz Triangle wave (audible and ok for speakers)
```

- Oscillators alone have an *infinite* duration or length and a constant amplitude. 
- You can use the Envelope, ASR, and ADSR (Attack, (Decay), Sustain, Release) Signals to define amplitude modifiers with *finite* duration:

```python
# This is a basic envelope that specifies amplitude (0.9), and duration (0.5 sec)
bas = Envelope(0.9, 0.5)
# This is an attack (0.1 sec), sustain (0.1 sec), release (0.1 sec) envelope.  
asr = ASR(0.1, 0.1, 0.1)
```

- Signals can be mixed using basic arithmetic. Multiplying and adding Signals can be thought of as an element-wise operation between two vectors.
    - Multiplying two Signals creates a new Signal of duration equal to the    shortest operand.  
    - Adding two Signals creates a new Signal of duration equal to the longest operand.
- Gain and bias can also be applied to Signals with scalar operands.
- Below are basic examples of mixing the Signals from above:

```python
sig1 = sqr * sin  # duration is infinite
sig2 = sig1 * asr # duration is 0.3 seconds
sig3 = 0.5 * (sqr + sin) * asr
```

- Signals created in the above examples:
![Signals](https://raw.githubusercontent.com/wiki/mahilab/Syntacts/images/sigs.png)

- Complete signals can be made and mixed in-place as well.

```python
# 250 Hz square wave amplitude modulated with a 20 Hz sine wave and ASR envelope.
sig4 = Square(250) * Sine(20) * ASR(1,1,1)
```

- Once you have created a complete Signal, it can be played on the Session.

```python
session.play(0, sig1) # plays sig 1 on channel 0
sleep(3)              # sig1 plays for 3 seconds
session.stop(0)       # stop sig1

session.play(1, sig2) # plays sig2 on channel 1
sleep(sig2.length)    # sig2 plays for its length of 0.3 seconds
                      # Do not need to stop sig2 because it is finite
                       
session.play_all(sig3) # plays sig3 on all channels
sleep(sig3.length)
```

> **Note:** If you want to evaluate Syntacts without tactor hardware, make sure the frequency is high enough to be audible for speakers (e.g. 440 Hz). Low frequencies can potentially damage speakers! For this reason, most of the Syntacts examples are coded with frequencies in the hearing range. If you are actually outputting to a tactor, use frequencies appropriate for it (e.g. 150-250 Hz).

|Relevant Header(s)|Relevant Examples(s)|
|---|---|
|[Signal.hpp](https://github.com/mahilab/Syntacts/blob/master/include/Tact/Signal.hpp), [Oscillator.hpp](https://github.com/mahilab/Syntacts/blob/master/include/Tact/Oscillator.hpp), [Envelope.hpp](https://github.com/mahilab/Syntacts/blob/master/include/Tact/Envelope.hpp), [General.hpp](https://github.com/mahilab/Syntacts/blob/master/include/Tact/General.hpp)|[example_basic.py](https://github.com/mahilab/Syntacts/blob/master/python/example_basic.py)

# Sequences

- Multiple Signals can be ordered in time using Sequences.
- You can concatenate Signals using the insertion, or left-shift, operator `<<`.
- Delay and pause are achieved through the insertion of positive scalar operands.
- Negative scalar operands move the insertion points backwards in time, allowing for the overlay or fading of Signals into each other.
- Below is a basic example of creating Sequences:

```python
sigA = Sine(440) * ASR(1,1,1);      # create 3 second Signal
sigB = Square(440) * ADSR(1,1,1,1); # create 4 second Signal

# 7 second Sequence with sigA before sigB
sig4 = sigA << sigB; 
# 1 sec delay and 2 sec pause, 10 sec Sequence
sig5 = 1 << sigA << 2 << sigB; 
# 1 sec fade/overlay between sigA and sigB, 6 sec sequence
sig6 = sigA << -1 << sigB; 
```

- Sequenced signals created above:
![Sequences](https://raw.githubusercontent.com/wiki/mahilab/Syntacts/images/seq.png)

- Sequences can also be concatenated:

```python
# Sequence of sig4, sig5, and sig6. Note this will also modify sig4.
sig7 = sig4 << sig5 << sig6; 
# Play sig7 on channel 0 for its length of 23 seconds
session.play(0, sig7);
sleep(sig7.length);
```

- You can also insert Signals into an existing Sequence timeline:

```python
# Insert 1 s of noise starts at the 4 second mark of sig5
sig5.insert(Noise() * Envelope(1), 4); 
# Play sig5 on channel 0
session.play(0, sig5); 
sleep(sig5.length)
```

|Relevant Header(s)|Relevant Examples(s)|
|---|---|
|[Sequence.hpp](https://github.com/mahilab/Syntacts/blob/master/include/Tact/Sequence.hpp)|[example_sequences.cpy](https://github.com/mahilab/Syntacts/blob/master/python/example_sequences.py)|

# Spatializers

*Coming Soon*

# Library

*Coming Soon*
