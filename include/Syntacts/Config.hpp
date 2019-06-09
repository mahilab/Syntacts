#pragma once

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