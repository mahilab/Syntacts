---
layout: page
title: GUI
permalink: /tutorials/gui/
---

## Introduction

This tutorial will teach you how to setup and use the Syntacts GUI. The GUI can be thought of as a companion app to the programming API, where you can quickly prototype and design cues before implementing them in your Syntacts-driven application. 

![GUI](https://raw.githubusercontent.com/wiki/mahilab/Syntacts/images/syntacts_gui.png)

## Requirements

- Latest [Syntacts Release](https://github.com/mahilab/Syntacts/releases)

## Basic Setup

- Download the [latest pre-compiled Syntacts binaries](https://github.com/mahilab/Syntacts/releases) for your system and extract the files. 
- Run the GUI executable `syntacts-gui` in the top level directory. 
- On Windows, you may receive a “Windows protected your PC” screen. Click “More info”, then “Run anyway”.
- On macOS, the executable `syntacts-gui` may not run, and it will instead open the binary code. If this occurs, open a Terminal and run the following command in the directory of the Syntacts binaries. After this, you will be able to successfully run `syntacts-gui`.

```shell
> sudo chmod 777 syntacts-gui
```

## GUI Widgets Overview

The GUI is composed of five main widget areas: `Device Bar`, `Info Bar`, `Palette/Library`, `Designer/Sequencer/Spatializer`, and `Player`. The functionality of each widget is broken down in the following sections.

![GUI](https://raw.githubusercontent.com/wiki/mahilab/Syntacts/images/tut-gui/layout.png)


# Device Bar

- The Device Bar is where you select the audio interface you wish to use. 
- Using the drop down arrows, you can choose your API, device, and sample rate.
- The second icon from the right will show a pop-up window listing all available devices. Devices denoted with a `*` are system defaults.
- The last icon will refresh the Syntacts session in the event that you need to reconnect/reboot a device.

![GUI](https://raw.githubusercontent.com/wiki/mahilab/Syntacts/images/tut-gui/device_bar.gif)

# Info Bar

- The Status and Info Bar is located at the bottom-right of the GUI.
- When you hover your mouse over different areas/panels of the GUI, the Info Bar states the function of that component.

# Palette/Library

- The Palette and Library panels are located on the left side of the GUI.
- The Palette contains tools to create different Signals. Signals represent various vibration waveforms. 
- Using the Palette, you can use different types of oscillators, envelopes, and processes to create your desired cue.
  - Sine, Square, Saw, and Triangle classes are available in Syntacts. These implement typical oscillators with normalized amplitude and infinite duration.
  - You can use the Envelope, ASR, and ADSR (Attack, (Decay), Sustain, Release) Signals to define amplitude modifiers with finite duration.
- The Library allows you to save and load created Signals.

# Player

- The Player is located on the right side of the GUI.
- This is used to play Signals on different channels.
- Using the Player, you can also change the volume and pitch by moving the V and P toggles.

# Designer

- The Designer is the first tab located in the center of the GUI.
- This is where you can drag Signals to create your desired cue.
- You can also change the frequency of Signals by using your mouse to move the frequency toggle.

# Sequencer

- The Sequencer is the second tab located in the center of the GUI.
- This can be used to concatenate multiple Signals to create a Sequence.
- You can drag Signals from the Library into the Sequencer and order the Signals to create your desired Sequence.
- You can also adjust the starting time of each Signal as well as the gain and bias using the Time, Gain, and Bias toggles.

# Spatializer

- The Spatializer is the third tab located in the center of the GUI.
- This maps multiple channels to a normalized (0 to 1) continuous 1D or 2D spatial representation.
- You can create a grid with a specific number of divisions to match the physical layout of a tactor array.

## Using the GUI: A Demo

This will run you through an example to teach you how to use the various components of the GUI.

# Designing a Cue

- We will begin by designing a cue using the Palette and the Designer.
- To design a cue, drag a Signal from the Palette into the Designer.
- You can change the frequency of the cue by dragging the frequency toggle or double clicking the frequency bar. 
    - If you want to evaluate Syntacts without tactor hardware, make sure the frequency is high enough to be audible for speakers (e.g. 440 Hz). Low frequencies can potentially damage speakers! For this reason, most of the Syntacts examples are coded with frequencies in the hearing range. If you are actually outputting to a tactor, use frequencies appropriate for it (e.g. 150-250 Hz).
- To delete a cue, simply click the `X` on the Signal bar.

*Demo:*

# Saving a Cue to the Library

- You can save a designed cue in the Library.
- After creating a cue, type the desired name of the cue into the library. Click the `+` button to save the cue under that name.

*Demo:*


# Mixing Signals

- Signals can be mixed using basic arithmetic. Multiplying and adding Signals can be thought of as an element-wise operation between two vectors.
  - Multiplying two Signals creates a new Signal of duration equal to the shortest operand.
  - Adding two Signals creates a new Signal of duration equal to the longest operand.
- The default process for mixing Signals in the GUI is multiplication.
- You can drag different Processes from the Palette into the Designer to create custom mixed Signals.
- You can also mix custom Signals together by dragging Signals from the Library into the Designer.

*Demo:*

# Sequencing Cues from the Library

- Signals can be sequenced by dragging Signals from the Library into the Sequencer.
- You can drag Signals directly onto the track or use the `+` button to place Signals immediately after each other.
- Use the `-` button to delete a Signal from the Sequence.
- You can move the cues backwards and forwards on the track to determine their position in time.

*Demo:*

# Using the Spatializer

- You can change the spatial layout of how a Signal is played using the Spatializer.
- Start by dragging the desired Signal from the Library onto the Signal bar on the right side of the Spatializer.
- You can create a grid representing the physical layout of your tactor array by changing the number of X and Y divisions.
  - To make the grid 1-Dimensional, click the grid button next to the divisions.
- Position the channels on the grid using the `Fill` button and dragging the channel numbers to the desired locations.
- You can then set a virtual target coordinate and radius to play and blend multiple tactors at once. Do this by right-mouse dragging the target around the grid or by using the Position and Radius toggles on the right side of the Spatializer.
- The volume of channels is interpolated according to a specified roll-off law (ie. linear, logarithmic, etc.) based on their proximity to the target location using. You can change this using the Roll-Off drop-down bar.

*Demo:*
