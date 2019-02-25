#pragma once

namespace tfx {

/// Makes any TactoFX primitive
template <typename T, typename ...Args>
auto make(Args ... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template <typename T, typename ...Args>
void Cue::addGenerator(Args... args) {
    auto g = std::make_shared<T>(std::forward<Args>(args)...);
    addGenerator(std::move(g));
}

}