#pragma once
#include <random>
#include "../math/point2.hpp"
#include "../math/normal3.hpp"


class Sampler {
public:
    Sampler();

    auto GetRandomReal() -> float;
    auto GetRandomInDistribution(int upperBound) -> int;
    auto CosineSampleHemisphere() -> Vec3f;
    auto GetRandom2D() -> Point2f;
    auto UniformSampleTriangle() -> Point2f;
    auto SampleDisk()->Point2f;
  

private:
    
    // to generate random numbers [0,1]
    //std::random_device m_seed;
    std::mt19937 m_gen;
    std::uniform_real_distribution<float> m_real_dist { 0.0f, 1.0f };
    //std::uniform_int_distribution<> m_intDist;
};

