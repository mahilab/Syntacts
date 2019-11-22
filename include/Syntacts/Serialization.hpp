#pragma once

///////////////////////////////////////////////////////////////////////////////
// SERIALIZATION MACROS FOR CEREAL LIBRARY
///////////////////////////////////////////////////////////////////////////////

namespace cereal {
    class access;
}

#define TACT_SERIALIZE(...) friend class cereal::access; \
                            template <class Archive> \
                            void serialize(Archive& archive) { \
                                archive(__VA_ARGS__); \
                            }
                            
#define TACT_PARENT(T) ::cereal::make_nvp(#T, cereal::base_class<T>(this))
#define TACT_MEMBER(T) ::cereal::make_nvp(#T, T)