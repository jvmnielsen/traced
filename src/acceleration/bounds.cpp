#include "bounds.hpp"

using namespace tr;
using namespace gm; 

auto Bounds::calculate_diagonal() const -> Vec3f {
    return {m_upper - m_lower};
}

auto Bounds::surface_area() const -> FLOAT {
    auto const diagonal = calculate_diagonal();
    return static_cast<FLOAT>(2) * (diagonal.x * diagonal.y + diagonal.x * diagonal.z + diagonal.y * diagonal.z);
}

auto Bounds::calculate_center() const -> Point3f {
    return m_lower + 0.5f * calculate_diagonal();
}

auto Bounds::axis_of_max_extent() const -> int {
    const auto diagonal = calculate_diagonal();
    if (diagonal.x > diagonal.y && diagonal.x > diagonal.z)
        return 0;

    if (diagonal.y > diagonal.z)
        return 1;

    return 2;
}

auto Bounds::overlaps(const Bounds& other) const ->  bool {

    for (int axis = 0; axis < 3; ++axis) {
        if (m_lower[axis] > other.upper()[axis] || other.lower()[axis] > m_upper[axis])
            return false;
    }
    return true;
}

auto Bounds::center() const -> Point3f {
    return (m_lower + m_upper) / 2;
}

auto Bounds::operator[](const std::size_t i) const -> Point3f {
    return (&m_lower)[i];
}

auto Bounds::operator[](const std::size_t i) -> Point3f& {
    return (&m_lower)[i];
}