#pragma once

#ifndef TFX_STATIC
    #ifdef TFX_EXPORTS
        #define TFX_API __declspec(dllexport)
    #else
        #define TFX_API __declspec(dllimport)
    #endif

    #ifdef _MSC_VER
        #pragma warning(disable:4251)
    #endif

#else

    #define TFX_API
    
#endif