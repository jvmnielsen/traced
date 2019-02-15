#include "intersection.hpp"
#include "../math/math_util.hpp"
#include "../geometry/mesh.hpp"


Intersection::Intersection(
    Point3f     point,
    Point2f     uvCoord,
    Normal3f    geometricNormal,
    Normal3f    shadingNormal)
    : m_point(std::move(point))
    , m_uv(std::move(uvCoord))
    , m_geometricNormal(std::move(geometricNormal))
    , m_shadingNormal(std::move(shadingNormal)) 
    , m_orthonormal(shadingNormal) {  
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

auto
Intersection::GetTransformedSampledVec(Sampler& sampler) const -> Vec3f {
    const auto cosSamplePoint = sampler.CosineSampleHemisphere();

    const auto& transform = m_mesh->GetTransform();

    const auto originalPoint = transform.Inverse(m_point);
    const auto offsetFromOrig = originalPoint - Point3f{0};
    const auto offsetSample = cosSamplePoint + offsetFromOrig;
    const auto transformedSample = transform(offsetSample);
    return transformedSample - m_point;

}