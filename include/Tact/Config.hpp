// MIT License
//
// Copyright (c) 2020 Mechatronics and Haptic Interfaces Lab
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// Author(s): Evan Pezent (epezent@rice.edu)

#pragma once

// Syntacts version numbers
#define SYNTACTS_VERSION_MAJOR 1
#define SYNTACTS_VERSION_MINOR 3
#define SYNTACTS_VERSION_PATCH 0

/// The maximum number of signals that can be played in unison (polyphony) on a single channel
#define SYNTACTS_MAX_VOICES 8

/// If uncommented, Signals will use a fixed size memory pool for allocation.
/// At this time, there doesn't seem to a great deal of benifit from doing this,
/// but one day it may be be possible to reap the benifits of 
/// cache coherence by using a more efficient pool scheme.
// #define SYNTACTS_USE_POOL   

/// The size of pool memory blocks in bytes available to store Signals 
/// (only relevant if SYNTACTS_USE_POOL enabled)
// #define SYNTACTS_POOL_BLOCK_SIZE  64

/// The number of pool blocks, effectively maximum number of signals that can
/// simultaneously exist (only relevant if SYNTACTS_USE_POOL enabled)
// #define SYNTACTS_POOL_MAX_BLOCKS 1024

/// If uncommented, Signals will use shared pointers internally instead of unique pointers.
/// This will result in fewer copies and allocations, but can lead to thread safety issues
/// if you modify the parametes of Signals after they have been passed to a Session. This
/// effectively breaks the GUI as it is currently implemented, so be warned!
// #define SYNTACTS_USE_SHARED_PTR 

#ifndef SYNTACTS_STATIC
    #ifdef SYNTACTS_EXPORTS
        #define SYNTACTS_API __declspec(dllexport)
    #else
        #define SYNTACTS_API __declspec(dllimport)
    #endif

    #ifdef _MSC_VER
        #pragma warning(disable:4251)
    #endif
#else
    #define SYNTACTS_API    
#endif