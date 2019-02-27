#pragma once

#include <array>
//#include "normal3.hpp"
#include <optional>

#define FLOAT double

namespace Math {

    static constexpr FLOAT  Infinity    = std::numeric_limits<FLOAT>::max();
    static constexpr FLOAT  NegInfinity = std::numeric_limits<FLOAT>::min();
    static constexpr FLOAT  Pi          = 3.14159265358979323846;
    static constexpr FLOAT  InvPi       = 0.31830988618379067154;
    static constexpr FLOAT  Epsilon     = 1e-5;

    template<typename T>
    T Clamp(T low, T high, T value) {
        return std::max(low, std::min(high, value));
    }

    inline auto PowerHeuristic(int nf, FLOAT fPdf, int ng, FLOAT gPdf) -> float {
        const auto f = nf * fPdf, g = ng * gPdf;
        return (f * f) / (f * f + g * g);
    }

    template<typename T>
    auto DegreeToRadian(T degree) -> T {
        return degree * Pi / 180.0;
    }

    // Returns in order smallest to largest solution
    template<typename T>
    auto SolveQuadratic(T a, T b, T c) -> std::optional<std::tuple<T, T>> {
       
        const auto discr = b * b - 4 * a * c;

        if (discr < 0) return std::nullopt;

        if (discr == 0) {
            const auto simpleSolu = -0.5 * b / a;
            return std::make_tuple(simpleSolu, simpleSolu);
        }
          
        const auto q = (b > 0) ? -0.5 * (b + sqrt(discr)) : -0.5 * (b - sqrt(discr));

        const auto solutionOne = q / a;
        const auto solutionTwo = c / q;

        if (solutionOne > solutionTwo)
            std::swap(solutionOne, solutionTwo);

        return std::make_tuple(solutionOne, solutionTwo);
    }

    template<typename T>
    auto Lerp(T t, T v1, T v2) -> T {
        return (1 - t) * v1 + t * v2;
    }

    template<typename T>
    auto Square(T a) -> T {
        return a * a;
    }

}
