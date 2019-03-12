#pragma once
#include <functional>

namespace Func {
    
    template<typename T>
    auto ConditionallyAssign(std::function<bool(T, T)> condition, T& base, T& comparedAgainst) {
        if (condition(base, comparedAgainst))
            base = comparedAgainst;
    };

    template<typename T>
    auto Smaller(const T& a, const T& b) -> bool {
        return a < b;
    }

}
