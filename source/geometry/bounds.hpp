#pragma once
#include "../math/point3.hpp"


class Bounds {

    Point3f m_lower;
    Point3f m_upper;

public:

    //Bounds() = default;

    Bounds(Point3f lower, Point3f upper) : m_lower(std::move(lower)), m_upper(std::move(upper)) {}
    auto lower() const -> Point3f { return m_lower; }
    auto upper() const -> Point3f { return m_upper; }


    auto overlaps(const Bounds& other) const -> bool;
    auto IsInside(const Point3f& point) const -> bool;

    auto center() const -> Point3f;

    auto Diagonal()         const->Vec3f;
    auto SurfaceArea()      const->FLOAT;
    auto CalculateCenter()  const->Point3f;
    auto axis_of_max_extent()    const -> int;

    auto operator[](const std::size_t i) const -> Point3f;
    auto operator[](const std::size_t i) -> Point3f&;
};

auto bounds_union(const Bounds& b1, const Bounds& b2) -> Bounds {
    return {elementwise_min(b1.lower(), b2.lower()), elementwise_max(b1.upper(), b2.upper())};
}

auto point_union(const Bounds& b, const Point3f& p) -> Bounds {
    return {elementwise_min(b.lower(), p), elementwise_max(b.upper(), p)};
}