#include "intersection.hpp"
#include "../geometry/mesh.hpp"

using namespace tr;

Intersection::Intersection(
    gm::Point3f point,
    gm::Point2f uv_coord,
    gm::Normal3f geometric_normal,
    gm::Normal3f shading_normal)
    : m_point(point),
      m_uv(uv_coord),
      m_geometric_normal(geometric_normal),
      m_shading_basis(shading_normal) {
}

auto Intersection::offset_point() const -> gm::Point3f {
    return m_point + gm::constants::epsilon * m_geometric_normal;
}

auto Intersection::point() const -> gm::Point3f const& {
    return m_point;
}

auto Intersection::geometric_normal() const -> gm::Normal3f const& {
    return m_geometric_normal;
};

auto Intersection::shading_normal() const -> gm::Normal3f const& {
    return m_shading_basis.w();
}

auto Intersection::is_specular() const -> bool {
    return false;
}

auto Intersection::sample_material(gm::Vec3f const& wo, Sampler& sampler) const -> std::tuple<gm::Vec3f, FLOAT, gm::Color3f> 
{
    return m_material->sample(wo, *this, sampler);
}

auto Intersection::emitted(gm::Vec3f const & dir) const -> gm::Color3f {
    return m_material->emitted(*this, dir);
}

auto Intersection::material_pdf(gm::Vec3f const& wi) const -> FLOAT {
    return m_material->pdf(wi, *this);
}

auto Intersection::evaluate_material(gm::Vec3f const& wo, gm::Vec3f const& wi) const -> gm::Color3f {
    return m_material->evaluate(wo, wi, *this);
}

auto Intersection::set_mesh_and_material(Mesh const* mesh, Material const* material) -> void {
    m_mesh = mesh;
    m_material = material;
}
