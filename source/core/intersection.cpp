#include "intersection.hpp"
#include "../math/math_util.hpp"
#include "../geometry/mesh.hpp"


Intersection::Intersection(
    Point3f     point,
    Point2f     uv_coord,
    Vec3f    geometric_normal,
    Vec3f    shading_normal)
    : m_point(std::move(point)),
      m_uv(std::move(uv_coord)),
      m_geometric_normal(std::move(geometric_normal)),
      //m_shadingNormal(std::move(shadingNormal)),
      //m_tangent(std::move(tangent)) {
      m_shading_basis(shading_normal) {
}

Point3f Intersection::offset_point() const {
    return m_point + m_geometric_normal * Math::Constants::Epsilon;
}

auto
Intersection::point() const -> const Point3f& {
    return m_point;
}

auto
Intersection::geometric_normal() const -> const Vec3f&
{
    return m_geometric_normal;
};

auto
Intersection::shading_normal() const -> const Vec3f&
{
    return m_shading_basis.w();
}

auto
Intersection::IsSpecular() const -> bool {
    return false;
}

auto 
Intersection::sample_material(const Vec3f& wo, Sampler& sampler) const -> std::tuple<Vec3f, FLOAT, Color3f> 
{
    return m_material->Sample(wo, *this, sampler);
}

auto 
Intersection::emitted(const Vec3f& dir) const -> Color3f {
    return m_material->Emitted(*this, dir);
}

auto 
Intersection::material_pdf(const Vec3f& wi) const -> FLOAT {
    return m_material->Pdf(wi, *this);
}

auto 
Intersection::evaluate_material(const Vec3f& wo, const Vec3f& wi) const -> Color3f {
    return m_material->Evaluate(wo, wi);
}

auto 
Intersection::ray_towards(const Vec3f& dir) const -> Rayf {
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
