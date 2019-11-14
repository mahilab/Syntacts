#pragma once

#include <memory>

namespace tact {

    /// Smart pointer type used throughout the Syntacts library
    template <typename T>
    using Ptr = std::shared_ptr<T>;

    /// Shorthand for std::make_shared<T>(...)
    template <typename T, typename ...Args>
    inline Ptr<T> create(Args... args) {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    /// Shorthand for std::move(...)
    using std::move;

}