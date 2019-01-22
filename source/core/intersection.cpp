#include "intersection.hpp"
#include "../math/math_util.hpp"

Intersection::Intersection(
    Point3f     point,
    Point2f     uvCoord,
    Normal3f    geometricNormal,
    Normal3f    shadingNormal)
    : m_point(std::move(point))
    , m_uv(std::move(uvCoord))
    , m_geometricNormal(std::move(geometricNormal))
    , m_shadingNormal(std::move(shadingNormal)) 
    , m_orthonormal(geometricNormal) {  
}


Point3f Intersection::OffsetShadingPoint() const {
    return m_point + m_shadingNormal * Math::Epsilon;
}

Point3f Intersection::OffsetGeometricPoint() const {
    return m_point + m_geometricNormal * Math::Epsilon;
}



auto
Intersection::GetPoint() const -> const Point3f& {
    return m_point;
}

auto
Intersection::GetGeometricNormal() const -> const Normal3f& {
    return m_geometricNormal;
};

auto
Intersection::GetShadingNormal() const -> const Normal3f& {
    return m_shadingNormal;
}

auto
Intersection::IsSpecular() const -> bool {
    return false;
}

auto
Intersection::GetOrthonormalBasis() const -> const ONB& {
    return m_orthonormal;
}