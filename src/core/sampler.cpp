#include "sampler.hpp"

#include <graphics-math.hpp>

using namespace gm;
using namespace tr;

Sampler::Sampler()
    : m_gen(std::random_device()()) {
}

auto Sampler::get_random_real() -> float {
    return m_real_dist(m_gen);
}

auto Sampler::get_random_in_distribution(int upperBound) -> int {
    return static_cast<int>(get_random_real() * (upperBound - 1));
}

auto Sampler::get_random_2D() -> Point2f {
    return {get_random_real(), get_random_real() };
}

auto Sampler::sample_disk() -> Point2f {
    auto const rand_point = get_random_2D();
    // map to [-1,1]
    auto const offset = Point2f{2.0f * rand_point.x - 1.0f, 2.0f * rand_point.y - 1.0f };
    if (offset.x == 0.0 && offset.y == 0.0)
        return { 0.0, 0.0 };

    FLOAT theta, r;
    if (std::abs(offset.x) > std::abs(offset.y)) {
        r = offset.x;
        theta = gm::constants::pi / 4 * (offset.y / offset.x);
    } else {
        r = offset.y;
        theta = gm::constants::pi / 2 - gm::constants::pi / 4 * (offset.x / offset.y);
    }
    return { std::cos(theta) * r, std::sin(theta) * r };
}

auto Sampler::cosine_sample_hemisphere() -> Vec3f {
    auto const r1 = get_random_real();
    auto const r2 = get_random_real();

    auto const z = std::sqrt(1 - r2);
    auto const phi = 2 * gm::constants::pi * r1;
    auto const x = std::cos(phi) * 2 * std::sqrt(r2);
    auto const y = std::sin(phi) * 2 * std::sqrt(r2);
    return { x, y, z };
}

auto Sampler::uniform_sample_triangle() -> Point2f {
    auto const p = get_random_2D();
    FLOAT su0 = std::sqrt(p[0]);
    return { 1 - su0, p[1] * su0 };
}