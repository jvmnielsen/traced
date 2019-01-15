#include "intersection.hpp"
#include "../math/math_util.hpp"

Intersection::Intersection(
    Point3f     point,
    Point2f     uvCoord,
    Normal3f    geometricNormal,
    Normal3f    shadingNormal,
    Mesh*       mesh)
    : m_point(std::move(point))
    , m_uv(std::move(uvCoord))
    , m_mesh(mesh)
    , m_geometricNormal(std::move(geometricNormal))
    , m_shadingNormal(std::move(shadingNormal)) {
}

auto
Intersection::UpdateRayToSampleDir(Rayf& rayToUpdate, const SamplingInfo& info) const -> void {
    rayToUpdate = Rayf{ m_point, info.toEye };
}

Point3f Intersection::OffsetShadingPoint() const {
    return m_point + m_shadingNormal * Math::Epsilon;
}

Point3f Intersection::OffsetGeometricPoint() const {
    return m_point + m_geometricNormal * Math::Epsilon;
}

auto
Intersection::NewThroughput(const Color3f& currentThroughput, SamplingInfo& info, Sampler& sampler) -> Color3f {

    m_material->Sample(info, sampler);
    if (info.pdf == 0.0f)
        return Color3f{0.0f};
    return currentThroughput * m_material->Evaluate(info) / info.pdf;
}

auto
Intersection::GetTangent() const -> const Normal3f& {
    return m_tangent;
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
Intersection::IsSpecular() const -> bool {
    return false;
}

