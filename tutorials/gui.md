---
layout: page
title: GUI
permalink: /tutorials/gui/
---

## Introduction

This tutorial will teach you how to setup and use the Syntacts GUI. The GUI can be thought of as a companion app to the programming API, where you can quickly prototype and design cues before implementing them in your Syntacts-driven application. Before reading this tutorial, it is recommended that you read one of the programming API tutorials so you have a basic understanding on Syntacts mechanisms and terminology. 

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

<p align="center">
    <img src="https://raw.githubusercontent.com/wiki/mahilab/Syntacts/images/tut-gui/layout.png" width="600">
</p>

# Device Bar

- The Device Bar is where you select the audio interface you wish to use. 
- Using the drop down arrows, you can choose your API, device, and sample rate.
- The second icon from the right will show a pop-up window listing all available devices. Devices denoted with a `*` are system defaults.
- The last icon will refresh the Syntacts session in the event that you need to reconnect/reboot a device.

<p align="center">
    <img src="https://raw.githubusercontent.com/wiki/mahilab/Syntacts/images/tut-gui/device_bar.gif" width="600">
</p>

# Info Bar

- The Info Bar displays relevant information and tooltips when you hover buttons and areas in the GUI.
- You can get helpful information and tips by dragging and dropping the **`?`** button over widgets.
- You can change the color theme of the GUI from the Info Bar.
- The meter displays the CPU thread load of the Syntacts session.
- Buttons for openning the Syntacts website and GitHub page are available.
- The `bug` button will display debug and diagnostics information.

<p align="center">
    <img src="https://raw.githubusercontent.com/wiki/mahilab/Syntacts/images/tut-gui/info_bar.gif" width="600">
</p>

# Palette

- The Palette lists available Signals for creating cues. Signals represent various vibration waveforms, envelopes, and processes. 
- Drag and drop Signals from the Palette into slots on the Designer widget.

# Designer

- The Designer widget offers a node based design were you can drag and drop any number of Signals from the Palette into slots.
- The resulting waveform is the **product** of each slotted Signal, and is displayed in the Visualizer widget.
- When a Signal is slotted into the Designer, a variety of context dependent controls will be presented.
- For numeric controls, you can double click for keyboard entry, or click and drag the control from left to right to change the value.
- Some Signal nodes accept other Signals or Library items as their inputs (e.g. Sum, Repeater, etc.).
- To delete a cue, simply click the `X` on the Signal bar.

# Visualizer

- The Visualizer displays a 2D graph of the current Signal.
- The displayed Signal is context dependent and changes depending on what tab you are in, or if you hovering a Library item.
- A horizontal scrollbar at the bottom of the Visualizer allows you to zoom in on the Signal.
- The duration of the Signal in seconds is displayed in the bottom-right. Infinitely long Signals display "inf".

<p align="center">
    <img src="https://raw.githubusercontent.com/wiki/mahilab/Syntacts/images/tut-gui/palette.gif" width="600">
</p>

# Library

- Once you've created a Signal to your liking, you can save it to the global Syntacts Library.
- Saved Signals can be loaded from any of the programming APIs.
- Buttons at the bottom of the widget allow you to delete, overwrite, and export Library items to `.tact`, `.wav`, `.json`, and `.csv` file formats.
- If you right click a Library item, a context menu with various actions will be displayed.

<p align="center">
    <img src="https://raw.githubusercontent.com/wiki/mahilab/Syntacts/images/tut-gui/library.gif" width="600">
</p>

# Player

- The Player widget is where you can play Signals on the currently selected Device.
- Clicking the main Play button will play the current Signal on all available channels.
- Click a channel number button will play the current Signal on only that channel.
- Right clicking either the Play or channel number buttons will stop the channel.
- You can drag Signals from the Library onto buttons to play them.
- Sliders for volume (V column) and pitch (P column) are available. Right clicking these will toggle them on/off.

<p align="center">
    <img src="https://raw.githubusercontent.com/wiki/mahilab/Syntacts/images/tut-gui/player.gif" width="600">
</p>

# Sequencer

- The Sequencer widget can be used to concatenate multiple Signals to create a Sequence.
- Add or remove tracks with the **`+`** and **`-`** buttons.
- Drag Signals from the Library onto the **`+`** button or into an unoccupied track.
- Drag slotted Signals horizontally to change their start time.
- You can also adjust the starting time of each Signal as well as the gain and bias using the Time, Gain, and Bias toggles.
- You can play the sequenced Signal on the Player, or save it to the Library.

<p align="center">
    <img src="https://raw.githubusercontent.com/wiki/mahilab/Syntacts/images/tut-gui/sequencer.gif" width="600">
</p>

# Spatializer

- The Spatializer widget allows you to create "continous" 1D or 2D arrays from your device's channels. 
- Channels in the Spatializer will have their volumn scaled based on their proximity to the target zone and its roll-off method.
- You can drag channels from the Player onto the grid, or change the grid dimensions and automatically fill it (click twice to change between row or column major).
- Channels consumed by the Spatializer will be highlighted and disabled in the Player.
- You can slot a Signal to be spatialized by dragging from the Library.
- The target location can be changed by right-click dragging in the grid area.
- The target radius can be changed with the mouse wheel, and the roll-off method can be changed via a dropdown menu.

<p align="center">
    <img src="https://raw.githubusercontent.com/wiki/mahilab/Syntacts/images/tut-gui/spatializer.gif" width="600">
</p>
