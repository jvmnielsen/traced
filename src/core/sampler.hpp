#pragma once

#include <graphics-math.hpp>

#include <random>


namespace tr {


class Sampler {
public:
    Sampler();

    auto get_random_real() -> float;
    auto get_random_in_distribution(int upperBound) -> int;
    auto cosine_sample_hemisphere() -> gm::Vec3f;
    auto get_random_2D() -> gm::Point2f;
    auto uniform_sample_triangle() -> gm::Point2f;
    auto sample_disk() -> gm::Point2f;
  

private:
    
    // to generate random numbers [0,1]
    //std::random_device m_seed;
    std::mt19937 m_gen;
    std::uniform_real_distribution<float> m_real_dist { 0.0f, 1.0f };
    //std::uniform_int_distribution<> m_intDist;
};

}

