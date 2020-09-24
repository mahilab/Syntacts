---
layout: page
title: Unity API
permalink: /tutorials/unity/
---

## Introduction
In this tutorial, we will learn how to integrate Syntacts into Unity projects. Since Unity uses C# as its scripting language, programming Syntacts in Unity is no different than programming in pure C#. As such, this tutorial will not cover the C# API - refer to the official [C# tutorial](tutorials/cs.md) for that. Instead, we will discuss important details and tips that are specific to Unity.

## Requirements

- [Unity Engine](https://unity.com/) (2019.3.7.1f or newer)
- latest Syntacts [release](https://github.com/mahilab/Syntacts/releases)

## Basic Demo

The Syntacts release folder contains a Unity demo you may wish to explore. Navigate to `unity/SyntactsDemo/Assets/Demo` and open `Demo.unity`. When Unity finishes loading, press the Play button in the Editor. You should see a ball bouncing on the screen, and a `Play Library Signal` in the top-left corner. You should also be able to hear various sound effects. The demo is actually using Syntacts to generate these sounds in the same way we might use Syntacts to generate haptic vibrations. The only difference is that we have chosen base frequencies in the hearing range. Although this demo is quite simple, three key features are on display:

1. **Discrete Syntacts Signals** - when the ball collides with the floor, a procedural Signal is generated and played. The Signal is generated in the  `BouncyBall.cs` component, and its parameters can be changed from  Unity Editor.  
2. **Continous Syntacts Signals** - the ball emits a continous Signal, also defined in `BouncyBall.cs`. The pitch of the Signal is altered in realtime based on the ball's height above the ground.
3. **Loading Syntacts Signals** - the `Play Library Signal` loads and plays a Signal from the global Syntacts Library when it is pressed. The Singal to be loaded is determiend from the string `signalName` found inside of the `LibraryButton.cs` component. The string can be changed from the Editor before or during Play.

![Demo](https://raw.githubusercontent.com/wiki/mahilab/Syntacts/images/tut-unity/ball.gif)


## Starting a New Project
