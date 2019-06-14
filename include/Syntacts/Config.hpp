#pragma once

constexpr int   SAMPLE_RATE = 48000;
constexpr float SAMPLE_LENGTH = 1.0f / SAMPLE_RATE;

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