#pragma once
#include <random>
#include "../math/vec3.hpp"

class Sampler {
public:
    Sampler();

    auto GetRandomReal() -> float;
    auto GetRandomInDistribution(std::uniform_int_distribution<>& dist) -> int;
    auto CosineSampleHemisphere() -> Vec3f;

private:
    // to generate random numbers [0,1]
    //std::random_device m_seed;
    std::mt19937 m_gen;
    std::uniform_real_distribution<float> m_realdDist { 0.0f, 1.0f };
    std::uniform_int_distribution<> m_intDist;
};

