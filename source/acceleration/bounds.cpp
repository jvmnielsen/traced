#include "bounds.hpp"

auto
Bounds::Diagonal() const -> Vec3f {
    return {m_upper - m_lower};
}

auto
Bounds::SurfaceArea() const -> FLOAT {
    const auto diagonal = Diagonal();
    return 2.0 * (diagonal.x() * diagonal.y() + diagonal.x() * diagonal.z() + diagonal.y() * diagonal.z());
}


auto
Bounds::CalculateCenter() const -> Point3f {
    return m_lower + Diagonal() * 0.5f;
}


auto
Bounds::axis_of_max_extent() const -> int {
    const auto diagonal = Diagonal();
    if (diagonal.x() > diagonal.y() && diagonal.x() > diagonal.z())
        return 0;

    if (diagonal.y() > diagonal.z())
        return 1;

    return 2;
}


auto Bounds::IsInside(const Point3f& point) const -> bool {

    /*
    const auto isInsideBoundsForAxis = [this, p] (int axis) {
        return p[axis] >= LowerBound()[axis] && p[axis] <= UpperBound()[axis];
    };

    for (int axis = 0; axis < 3; ++axis)
        if (!isInsideBoundsForAxis(axis))
            return false;

    */
    return true;
}

auto
Bounds::overlaps(const Bounds& other) const ->  bool {
    return     m_upper.x() >= other.lower().x() && m_lower.x() <= other.upper().x()
        && m_upper.y() >= other.lower().y() && m_lower.y() <= other.upper().y()
        && m_upper.z() >= other.lower().z() && m_lower.z() <= other.upper().z();
}

auto 
Bounds::center() const -> Point3f {
    return (m_lower + m_upper) / 2;
}

auto 
Bounds::operator[](const std::size_t i) const -> Point3f {
    return (&m_lower)[i];
}

auto 
Bounds::operator[](const std::size_t i) -> Point3f& {
    return (&m_lower)[i];
}