#pragma once
#include <Tact/Config.hpp>

#include <cereal/types/memory.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/access.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/functional.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/utility.hpp>

///////////////////////////////////////////////////////////////////////////////

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
