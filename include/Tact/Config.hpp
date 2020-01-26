#pragma once

// Syntacts version numbers
#define SYNTACTS_VERSION_MAJOR 1
#define SYNTACTS_VERSION_MINOR 0
#define SYNTACTS_VERSION_PATCH 0

/// If uncommented, Signals will use a fixed size memory pool for allocation.
/// At this, there doesn't seem to a great deal of benifit from doing this,
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