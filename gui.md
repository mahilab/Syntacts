---
layout: page
title: GUI
permalink: /gui/
---

## Introduction

This tutorial will teach you how to setup and use the Syntacts GUI. This will allow you to create, visualize, and sequence cues for your desired use.

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

## GUI Panel Overview

This will provide an overview of the GUI's panels and a basic guide about their uses. For more detailed information on how to use different components of the GUI, simply drag the help button (the question mark in the bottom right corner) over the area of interest.

# Device Bar

- The Device Bar is located at the top-center of the GUI.
- Using the drop down arrows, you can choose your API, device, and sample rate.

# Status and Info Bar

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
- You can combine Signals using different processes. Signals can be mixed using basic arithmetic. Multiplying and adding Signals can be thought of as an element-wise operation between two vectors.
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
- Drag channels onto the grid to position them as desired.
- You can then set a virtual target coordinate and radius to play and blend multiple tactors at once.
- The volume of channels is interpolated according to a specified roll-off law (ie. linear, logarithmic, etc.) based on their proximity to the target location using.

## Using the GUI: A Demo

This will run you through an example to teach you how to use the various components of the GUI.

# Designing a Cue

- We will begin by designing a cue using the Paletter and the Designer.
- First, drag a Sine cue from the Palette into the Designer.
- Change the frequency of the Sine cue to 10 Hz.
  - You can change the frequency of the cue by dragging the frequency toggle. 
    - If you want to evaluate Syntacts without tactor hardware, make sure the frequency is high enough to be audible for speakers (e.g. 440 Hz). Low frequencies can potentially damage speakers! For this reason, most of the Syntacts examples are coded with frequencies in the hearing range. If you are actually outputting to a tactor, use frequencies appropriate for it (e.g. 150-250 Hz).
- Next, drag a Square cue from the Palette into the Designer below the Sine cue.
- Change the frequency of the Square cue to 250 Hz.
- This should have created a Signal of infinite duration that looks like repeated vertical ovals.

*To be continued*
