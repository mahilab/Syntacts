#pragma once

///////////////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////////

namespace cereal {
    class access;
}

#define TACT_SERIALIZABLE friend class cereal::access; \
                          template <class Archive> \
                          void serialize(Archive& archive) {} 

#define TACT_SERIALIZE(...) friend class cereal::access; \
                            template <class Archive> \
                            void serialize(Archive& archive) { \
                                archive(__VA_ARGS__); \
                            }
                            
#define TACT_PARENT(T) ::cereal::make_nvp(#T, cereal::base_class<T>(this))

#define TACT_MEMBER(T) ::cereal::make_nvp(#T, T)

///////////////////////////////////////////////////////////////////////////////

#define TACT_CURVE(T) struct T { \
                          double operator()(double t) const; \
                          template <class Archive> void serialize(Archive& archive) {} \
                      };

///////////////////////////////////////////////////////////////////////////////

