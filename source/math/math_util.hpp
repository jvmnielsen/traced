#pragma once

#include <array>

namespace Math
{
    static constexpr float  Infinity    = std::numeric_limits<float>::max();
    static constexpr float  NegInfinity = std::numeric_limits<float>::min();
    static constexpr float  Pi          = 3.14159265358979323846f;
    static constexpr float  Epsilon     = 1e-5f;

    template<typename T>
    T Clamp(T low, T high, T value)
    {
        return std::max(low, std::min(high, value));
    }

    inline auto PowerHeuristic(int nf, float fPdf, int ng, float gPdf) -> float {
        float f = nf * fPdf, g = ng * gPdf;
        return (f * f) / (f * f + g * g);
    }

    float DegreeToRadian(float degree);

    // Solve quadratic equation
    // Smallest solution is returned in solutionOne
    bool SolveQuadratic(
            const float a,
            const float b,
            const float c,
            float& solutionOne,
            float& solutionTwo);

}
