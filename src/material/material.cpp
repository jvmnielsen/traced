#include "material.hpp"
#include "../core/intersection.hpp"
#include <cmath>

using namespace tr;
using namespace gm;

auto Material::sample(
        gm::Vec3f const& wo,
        Intersection const& isect,
        Sampler& sampler
    ) const -> std::tuple<Vec3f, FLOAT, Color3f> {

    auto wi = isect.get_shading_basis().convert_to_local(sampler.cosine_sample_hemisphere());
    const auto pdf = Material::pdf(wi, isect);
    return std::make_tuple(wi, pdf, evaluate(wo, wi, isect)); // called by derived class
}

auto Material::pdf(gm::Vec3f const& dir, Intersection const& isect) const -> FLOAT {
    return dot(isect.shading_normal(), dir) / gm::constants::pi;
}

auto Material::emitted(Intersection const& isect, gm::Vec3f const& dir) const -> gm::Color3f {
    return gm::Color3f::black();
}

auto Matte::evaluate(Vec3f const& wo, Vec3f const& wi, Intersection const& isect) const -> Color3f {
    return m_attenuation * gm::constants::pi;
}

auto Emissive::emitted(Intersection const& isect, gm::Vec3f const& dir) const -> gm::Color3f {
    return gm::dot(isect.shading_normal(), dir) > 0 ? m_radiance : gm::Color3f::black();
}

auto Glossy::sample(
        gm::Vec3f const& wo,
        Intersection const& isect,
        Sampler& sampler
    ) const -> std::tuple<gm::Vec3f, FLOAT, gm::Color3f> {

    auto const n = (-wo + 2.0f * isect.shading_normal() * dot(isect.shading_normal(), wo)).normalise();
    auto const onb = gm::ONB{n};
    auto const v = sampler.cosine_sample_hemisphere();
    auto wi = onb.convert_to_local(v);
    if (dot(isect.shading_normal(), wi) < 0)
        wi = -v.x * onb.u() -v.y * onb.v() + v.z * onb.w();

    auto const phong_lobe = std::pow(dot(onb.w(), wi), m_exp);
    auto const pdf = phong_lobe * dot(isect.shading_normal(), wi);
    auto const color = m_ks * m_attenuation * phong_lobe;

    return {wi, pdf, color};
}

auto Glossy::evaluate(
        gm::Vec3f const& wo,
        Vec3f const& wi,
        Intersection const& isect
    ) const -> gm::Color3f {

    gm::Color3f color = gm::Color3f::black();
    auto const n_dot_wi = gm::dot(isect.shading_normal(), wi);
    gm::Vec3f r{-wi + 2.0f * isect.shading_normal() * n_dot_wi};
    auto const r_dot_wo = dot(r, wo);

    if (r_dot_wo > 0.0f)
        color = Color3f(m_ks * std::pow(r_dot_wo, m_exp));

    return color;
}

auto Glossy::pdf(Vec3f const& wi, Intersection const& isect) const -> FLOAT {
    auto const phong_lobe = std::pow(dot(isect.shading_normal(), wi), m_exp);
    return phong_lobe * dot(isect.shading_normal(), wi);
}

