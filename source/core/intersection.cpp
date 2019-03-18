#include "intersection.hpp"
#include "../math/math_util.hpp"
#include "../geometry/mesh.hpp"


Intersection::Intersection(
    Point3f     point,
    Point2f     uvCoord,
    Normal3f    geometricNormal,
    Normal3f    shadingNormal)
    : m_point(std::move(point)),
      m_uv(std::move(uvCoord)),
      m_geometricNormal(std::move(geometricNormal)),
      //m_shadingNormal(std::move(shadingNormal)),
      //m_tangent(std::move(tangent)) {
      m_shadingBasis(shadingNormal) {
}

Point3f Intersection::PointOffset() const {
    return m_point + m_geometricNormal * Math::Constants::Epsilon;
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
    return m_shadingBasis.w();
}

auto
Intersection::IsSpecular() const -> bool {
    return false;
}

auto 
Intersection::SampleMaterial(const Normal3f& wo, Sampler& sampler) const -> std::tuple<Normal3f, FLOAT, Color3f> {
    return m_material->Sample(wo, *this, sampler);
}

auto 
Intersection::Emitted(const Normal3f& dir) const -> Color3f {
    return m_material->Emitted(*this, dir);
}

auto 
Intersection::MaterialPdf(const Normal3f& wi) const -> FLOAT {
    return m_material->Pdf(*this, wi);
}

auto 
Intersection::EvaluateMaterial(const Normal3f& wo, const Normal3f& wi) const -> Color3f {
    return m_material->Evaluate(wo, wi);
}

auto 
Intersection::RayTowards(const Normal3f& dir) const -> Rayf {
    return {m_point, dir};
}

auto 
Intersection::SetMeshAndMaterial(const Mesh* mesh, const Material* material) -> void {
    m_mesh = mesh;
    m_material = material;
}

/*
auto
Intersection::GetOrthonormalBasis() const -> const ONB& {
    return m_orthonormal;
}*/

/*
auto
Intersection::GetTransformedSampledVec(Sampler& sampler) const -> Normal3f {
    const auto cosSamplePoint = sampler.CosineSampleHemisphere();
    const auto& transform = m_mesh->GetTransform();
    const auto& inverse = transform.m_inverse;

    const auto originalPoint = inverse.Multiply(m_point);
    const auto offsetFromOrig = originalPoint - Point3f{0};
    //const auto offsetSample = cosSamplePoint + offsetFromOrig;
    //const auto transformedSample = transform(offsetSample);
    //return Normalize(transformedSample - m_point);
    return Normal3f{0,0,0};
} */
