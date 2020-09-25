---
layout: page
title: MATLAB
permalink: /tutorials/matlab/
---

## Introduction
Although Syntacts does not yet officially provide a wrapper for MATLAB, it is still possible to load and use the raw Syntacts C library. This tutorial will demonstrate how to set this up and basic usage. For a comprehensive guide to Syntact's API, refer to the [C++ API tutorial](/tutorials/cpp). The interface will not be exactly the same since C does not have classes, but the names of the exported C functions are such that you should be able to figure things out (e.g. `Session.play(...)` in C++ will be `Session_play(...)` in C). 

## Requirements

- MATLAB
- compatible C/C++ compiler
- latest [Syntacts Release](https://github.com/mahilab/Syntacts/releases)

## Setting Up the MATLAB Compiler

To import the Syntacts library into MATLAB, you first need to have installed a compatible C/C++ compiler. This [table](https://www.mathworks.com/support/requirements/supported-compilers.html) lists the various compilers compatible with each MATLAB release. 

Unless you decide to build the Syntacts C library from source using your own compiler, you need to also ensure that your compiler is compatible with compiler that was used to build the Syntacts Release:

- On Windows, we use MSVC to build Syntacts. Versions 2015, 2017, and 2019 should work. MinGW *may* work. Feel free to try.
- On macOS, we use XCode (i.e. Apple Clang) to build Syntacts. Install the latest version of XCode from the Apple Store.

For this example, we will use MATLAB 2020b and MSVC 2019 on Windows 10.

## Loading Syntacts into MATLAB

- From the `c` folder in the latest Syntacts Release, extract `syntacts.h` and `syntacts.dll` (or `syntacts.dylib` on macOS) to a location of your choice. 
- Open MATLAB and set the working directory to the location containing the header file and library.
- The following code can be used to load Syntacts:
    ```matlab
    loadlibrary('syntacts')
    ```
- You can list the available functions with:
    ```matlab
    libfunctions('syntacts')
    ```
    ```
    ADSR_create                       Session_open1                     
    ASR_create                        Session_open2                     
    Add_FltSig                        Session_open3   
    ...
    ```

## Using Syntacts from MATLAB

Once the library is loaded, you can create a Syntacts Session and open a device:

```matlab
s = calllib('syntacts','Session_create');

calllib('syntacts','Session_open1',s);
```

With an open device, you can proceed to create and play Signals:

```matlab
% equivalent to Signal = Sine(440) * ASR(1,1,1,1) in C++
sine = calllib('syntacts','Sine_create2',440);
asr  = calllib('syntacts','ASR_create',1,1,1,1);
sig  = calllib('syntacts','Product_create',sine,asr);
% play Signal on channel 0 (left speaker will be audible)
calllib('syntacts','Session_play',s,0,sig);
% close device
calllib('syntacts','Session_close',s)
```

MATLAB doesn't know how to free the resources that Syntacts creates, so we must do so explicitly to avoid memory leaks:

```matlab
% free Signals
calllib('syntacts','Signal_delete',sine);
calllib('syntacts','Signal_delete',asr);
calllib('syntacts','Signal_delete',sig);
% free Session
calllib('syntacts','Session_delete',s);
```

Finally, when you are done using Syntacts, don't forget to unload the library:
```matlab
unloadlibrary('syntacts')
```

## Wrappers

As you can see, while it is possible to use Syntacts from MATLAB, doing so is quite cumbersome. Therefore, we recommend that you create wrapper classes or functions around the Syntacts library features you plan to use. For example:

```matlab
% Syntacts.m
classdef Syntacts < handle
properties
    session
end
methods
    % constructor
    function obj = Syntacts(dev_idx)
        loadlibrary('syntacts','syntacts.h');
        obj.session = calllib('syntacts','Session_create');
        calllib('syntacts','Session_open2',obj.session,dev_idx);
    end
    % destructor
    function delete(obj)
        calllib('syntacts','Session_close',obj.session);
        calllib('syntacts','Session_delete',obj.session);
        unloadlibrary('syntacts');
    end
    % play sine wave Signal
    function play(obj,ch,freq,amp,dur)
        sine = calllib('syntacts','Sine_create2',freq);
        env  = calllib('syntacts','Envelope_create',dur,amp);
        sig  = calllib('syntacts','Product_create',sine,env);
        calllib('syntacts','Session_play',obj.session,ch,sig);
        calllib('syntacts','Signal_delete',sine);
        calllib('syntacts','Signal_delete',env);
        calllib('syntacts','Signal_delete',sig);
    end
end
end
```

```matlab
S = Syntacts(6);    % make a Syntacts Session with device 6
S.play(0,250,1,1);  % play a Signal on channel 0
S.play(1,175,0.5,1) % play a Signal on channel 1
clear S             % resources will be automatically freed
```

Eventually we hope to support MATLAB in an official capacity using wrapper objects similar to that presented here. If this is something you would like to help with, feel free to contribute a Pull Request on the GitHub repository! 