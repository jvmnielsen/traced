#include "intersection.hpp"
//#include "../math/math_util.hpp"
#include "../geometry/mesh.hpp"

using namespace tr;
// using namespace gm;

Intersection::Intersection(
    gm::Point3f point,
    gm::Point2f uv_coord,
    gm::Vec3f geometric_normal,
    gm::Vec3f shading_normal)
    : m_point(std::move(point)),
      m_uv(std::move(uv_coord)),
      m_geometric_normal(std::move(geometric_normal)),
      //m_shadingNormal(std::move(shadingNormal)),
      //m_tangent(std::move(tangent)) {
      m_shading_basis(shading_normal) {
}

auto Intersection::offset_point() const -> gm::Point3f {
    return m_point + m_geometric_normal * gm::constants::epsilon;
}

auto Intersection::point() const -> gm::Point3f const& {
    return m_point;
}

auto Intersection::geometric_normal() const -> gm::Vec3f const& {
    return m_geometric_normal;
};

auto Intersection::shading_normal() const -> gm::Vec3f const& {
    return m_shading_basis.w();
}

auto Intersection::is_specular() const -> bool {
    return false;
}

auto Intersection::sample_material(gm::Vec3f const& wo, Sampler& sampler) const -> std::tuple<gm::Vec3f, FLOAT, gm::Color3f> 
{
    return m_material->sample(wo, *this, sampler);
}

auto 
Intersection::emitted(gm::Vec3f const & dir) const -> Color3f {
    return m_material->emitted(*this, dir);
}

auto 
Intersection::material_pdf(gm::Vec3f const& wi) const -> FLOAT {
    return m_material->pdf(wi, *this);
}

auto Intersection::evaluate_material(gm::Vec3f const & wo, gm::Vec3f const& wi) const -> gm::Color3f {
    return m_material->evaluate(wo, wi, *this);
}

auto Intersection::ray_towards(gm::Vec3f const& dir) const -> Rayf {
    return {m_point, dir};
}

auto Intersection::SetMeshAndMaterial(Mesh const* mesh, Material const* material) -> void {
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
    const auto cosSamplePoint = sampler.cosine_sample_hemisphere();
    const auto& transform = m_mesh->GetTransform();
    const auto& inverse = transform.m_inverse;

    const auto originalPoint = inverse.Multiply(m_point);
    const auto offsetFromOrig = originalPoint - Point3f{0};
    //const auto offsetSample = cosSamplePoint + offsetFromOrig;
    //const auto transformedSample = transform(offsetSample);
    //return Normalize(transformedSample - m_point);
    return Normal3f{0,0,0};
} */
