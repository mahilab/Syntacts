#pragma once

///////////////////////////////////////////////////////////////////////////////
// SERIALIZATION MACROS FOR CEREAL LIBRARY
///////////////////////////////////////////////////////////////////////////////

namespace cereal {
    class access;
}

#define SERIALIZE(...) friend class cereal::access; \
                       template <class Archive> \
                       void serialize(Archive& archive) { \
                           archive(__VA_ARGS__); \
                       }
#define PARENT(T) ::cereal::make_nvp(#T, cereal::base_class<T>(this))
#define MEMBER(T) ::cereal::make_nvp(#T, T)