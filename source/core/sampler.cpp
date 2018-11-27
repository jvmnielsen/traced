#include "sampler.hpp"

Sampler::Sampler()
        : m_gen(std::random_device()()) {
}

auto
Sampler::GetRandomReal() -> float {
    return m_realdDist(m_gen);
}

auto
Sampler::GetRandomInDistribution(std::uniform_int_distribution<>& dist) -> int {
    return dist(m_gen);
}