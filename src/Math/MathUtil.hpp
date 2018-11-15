#pragma once

#include <array>

namespace Math
{
    static constexpr float  Infinity    = std::numeric_limits<float>::max();
    static constexpr float  Pi          = 3.14159265358979323846f;
    static constexpr float  Epsilon     = 1e-4f;

    template<typename T>
    T Clamp(T low, T high, T value)
    {
        return std::max(low, std::min(high, value));
    }

    float DegreeToRadian(float degree);

    bool SolveQuadratic(
            const float a,
            const float b,
            const float c,
            float& solutionOne,
            float& solutionTwo);

}
