#include "sampler.hpp"
#include "../math/math_util.hpp"

Sampler::Sampler()
        : m_gen(std::random_device()()) {
}

auto
Sampler::GetRandomReal() -> float {
    return m_realDist(m_gen);
}

auto
Sampler::GetRandomInDistribution(int upperBound) -> int {
    return static_cast<unsigned int>(GetRandomReal() * (upperBound-1));
}

auto 
Sampler::CosineSampleHemisphere() -> Vec3f {
    //Vec3f p;
    /*
    do {
        p = Vec3f{GetRandomReal(), GetRandomReal(), GetRandomReal()} * 2.0f - Vec3f{1.0f, 1.0f, 1.0f};
    } while (Dot(p, p) >= 1.0);
    */
    const auto r1 = GetRandomReal();
    const auto r2 = GetRandomReal();
    const auto z = std::sqrt(1 - r2);
    const auto phi = 2 * Math::Pi * r1;
    const auto x = std::cos(phi) * 2 * std::sqrt(r2);
    const auto y = std::sin(phi) * 2 * std::sqrt(r2);
    return Vec3f{ x, y, z };

}