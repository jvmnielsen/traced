#pragma once
#include <random>
#include "../math/vec3.hpp"
//#include "../math/onb.hpp"
#include "../math/point3.hpp"


class Sampler {
public:
    Sampler();

    auto GetRandomReal() -> float;
    auto GetRandomInDistribution(int upperBound) -> int;
    auto CosineSampleHemisphere() -> Point3f;

private:
    // to generate random numbers [0,1]
    //std::random_device m_seed;
    std::mt19937 m_gen;
    std::uniform_real_distribution<float> m_realDist { 0.0f, 1.0f };
    //std::uniform_int_distribution<> m_intDist;
};

