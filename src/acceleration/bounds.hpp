#pragma once
#include <graphics-math.hpp>

namespace tr {

    class Bounds {
        gm::Point3f m_lower;
        gm::Point3f m_upper;
    public:
        Bounds(gm::Point3f lower, gm::Point3f upper) : m_lower(lower), m_upper(upper) {}

        [[nodiscard]] auto lower() const -> gm::Point3f { return m_lower; }
        [[nodiscard]] auto upper() const -> gm::Point3f { return m_upper; }
        [[nodiscard]] auto overlaps(Bounds const& other) const -> bool;
        [[nodiscard]] auto inside(gm::Point3f const& point) const -> bool;
        [[nodiscard]] auto center() const -> gm::Point3f;
        [[nodiscard]] auto calculate_diagonal() const-> gm::Vec3f;
        [[nodiscard]] auto surface_area() const-> FLOAT;
        [[nodiscard]] auto calculate_center() const-> gm::Point3f;
        [[nodiscard]] auto axis_of_max_extent() const -> int;

        auto operator[](std::size_t i) const -> gm::Point3f;
        auto operator[](std::size_t i) -> gm::Point3f&;
    };

    auto inline bounds_union(Bounds const& b1, Bounds const& b2) -> Bounds {
        return {elementwise_min(b1.lower(), b2.lower()), elementwise_max(b1.upper(), b2.upper())};
    }

    auto inline point_union(Bounds const& b, gm::Point3f const& p) -> Bounds {
        return {elementwise_min(b.lower(), p), elementwise_max(b.upper(), p)};
    }
}