#pragma once

namespace tfx {

/// Makes any TactoFX primitive
template <typename T, typename ...Args>
Ptr<T> make(Args ... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template <typename T, typename ...Args>
void Cue::chain(Args... args) {
    auto g = std::make_shared<T>(std::forward<Args>(args)...);
    chain(std::move(g));
}

}