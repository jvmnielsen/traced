#include "sampler.hpp"
#include "../math/math_util.hpp"
#include "../math/normal3.hpp"

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
Sampler::CosineSampleHemisphere(const Vec3f& normal) -> Vec3f {
    
    std::array<Vec3f, 3> onb{};

    onb[2] = normal;

    Vec3f a;
    if (normal.x > 0.9f) {
        a = Vec3f{0, 1, 0};
    } else {
        a = Vec3f{1, 0, 0};
    }

    onb[1] = Cross(onb[2], a).Normalize();
    onb[0] = Cross(onb[2], onb[1]);

    const auto r1 = GetRandomReal();
    const auto r2 = GetRandomReal();
    const auto z = std::sqrt(1 - r2);
    const auto phi = 2 * Math::Pi * r1;
    const auto x = std::cos(phi) * 2 * std::sqrt(r2);
    const auto y = std::sin(phi) * 2 * std::sqrt(r2);
    const auto sampled = Normalize(Vec3f{ x, y, z });

    return sampled.x * onb[0] + sampled.y * onb[1] + sampled.z * onb[2];
}