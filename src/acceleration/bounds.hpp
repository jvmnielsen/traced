#pragma once
//#include "../math/point3.hpp"
//#include "../geometry/mesh.hpp"

#include <graphics-math.hpp>

namespace tr {

class Bounds {

    gm::Point3f m_lower;
    gm::Point3f m_upper;

public:
    Bounds(gm::Point3f lower, gm::Point3f upper) : m_lower(std::move(lower)), m_upper(std::move(upper)) {}

    auto lower() const -> gm::Point3f { return m_lower; }
    auto upper() const -> gm::Point3f { return m_upper; }

    auto overlaps(Bounds const& other) const -> bool;
    auto inside(gm::Point3f const& point) const -> bool;

    auto center() const -> gm::Point3f;

    auto Diagonal() const-> gm::Vec3f;
    auto SurfaceArea() const-> FLOAT;
    auto CalculateCenter() const-> gm::Point3f;
    auto axis_of_max_extent() const -> int;

    auto operator[](const std::size_t i) const -> gm::Point3f;
    auto operator[](const std::size_t i) -> gm::Point3f&;
};

auto inline bounds_union(Bounds const& b1, Bounds const& b2) -> Bounds {
    return {elementwise_min(b1.lower(), b2.lower()), elementwise_max(b1.upper(), b2.upper())};
}

auto inline point_union(Bounds const& b, gm::Point3f const& p) -> Bounds {
    return {elementwise_min(b.lower(), p), elementwise_max(b.upper(), p)};
}

}