#include "sampler.hpp"

#include <graphics-math.hpp>

using namespace gm;
using namespace tr;

Sampler::Sampler()
        : m_gen(std::random_device()()) {
}

auto
Sampler::get_random_real() -> float {
    return m_real_dist(m_gen);
}

auto
Sampler::get_random_in_distribution(int upperBound) -> int {
    return static_cast<unsigned int>(get_random_real() * (upperBound - 1));
}

auto
Sampler::get_random_2D() -> Point2f {
    return {get_random_real(), get_random_real() };
}

auto
Sampler::sample_disk() -> Point2f {
    const auto randPoint = get_random_2D();
    // map to [-1,1]
    const auto offset = Point2f{ 2.0f * randPoint.x - 1.0f, 2.0f * randPoint.y - 1.0f };

    if (offset.x == 0.0 && offset.y == 0.0)
        return { 0.0, 0.0 };

    FLOAT theta, r;
    if (std::abs(offset.x) > std::abs(offset.y)) {
        r = offset.x;
        theta = Math::Constants::Pi / 4 * (offset.y / offset.x); // Todo: precalc pi values
    } else {
        r = offset.y;
        theta = Math::Constants::Pi / 2 - Math::Constants::Pi / 4 * (offset.x / offset.y);
    }
    return { std::cos(theta) * r, std::sin(theta) * r };

}

auto 
Sampler::cosine_sample_hemisphere() -> Vec3f {
    
    //const auto p = sample_disk();
    //FLOAT z = std::sqrt(std::max((FLOAT)0, 1 - p.x * p.x - p.y * p.y));
    //return Normalize(Vec3f{ p.x, p.y, z });

    const auto r1 = get_random_real();
    const auto r2 = get_random_real();
    
    const auto z = std::sqrt(1 - r2);
    const auto phi = 2 * Math::Constants::Pi * r1;
    const auto x = std::cos(phi) * 2 * std::sqrt(r2);
    const auto y = std::sin(phi) * 2 * std::sqrt(r2);
    return { x, y, z };

    //return basis.ConvertToLocal(sampled); */

    //return sampled.x * onb[0] + sampled.y * onb[1] + sampled.z * onb[2];
}

auto
Sampler::uniform_sample_triangle() -> Point2f {
    const auto p = get_random_2D();
    FLOAT su0 = std::sqrt(p[0]);
    return { 1 - su0, p[1] * su0 };
}