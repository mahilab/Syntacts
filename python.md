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

