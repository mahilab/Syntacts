#pragma once

namespace tfx {

template <typename T, typename ...Args>
void Cue::chain(Args... args) {
    auto g = std::make_shared<T>(std::forward<Args>(args)...);
    chain(std::move(g));
}

}