#include "material.hpp"
#include "../core/intersection.hpp"
#include <cmath>

using namespace tr;
using namespace gm;

auto Material::sample(gm::Vec3f const& wo, Intersection const& isect, Sampler& sampler) const -> std::tuple<Vec3f, FLOAT, Color3f> {

    //const auto wo = isect.get_shading_basis().WorldToLocal(worldWo);
    //if (wo.z == 0) return
    auto wi = isect.get_shading_basis().convert_to_local(sampler.cosine_sample_hemisphere());
    //if (wo.z() < 0) wi = -wi; // flip to match direction
    const auto pdf = Material::pdf(wi, isect);
    //const auto wiWorld = isect.get_shading_basis().LocalToWorld(wi);
    return std::make_tuple(wi, pdf, evaluate(wo, wi, isect)); // called by derived class
}

auto Material::pdf(gm::Vec3f const& dir, Intersection const& isect) const -> FLOAT {
    /*
    const auto cosine = dot(dir, isect.shading_normal());
    if (cosine > 0)
        return cosine / Math::Constants::Pi;
    return 0 ; */
    return dot(isect.shading_normal(), dir) / gm::constants::pi;
   
}

auto Material::emitted(Intersection const& isect, gm::Vec3f const& dir) const -> gm::Color3f {
    return gm::Color3f::black();
}

auto Matte::evaluate(Vec3f const& wo, Vec3f const& wi, Intersection const& isect) const -> Color3f {
    //return same_hemisphere(wo, wi) ? m_attenuation * Math::Constants::Pi : Color3f::Black();
    return m_attenuation * gm::constants::pi;
}

auto Emissive::emitted(Intersection const& isect, gm::Vec3f const& dir) const -> gm::Color3f {
    return gm::dot(isect.shading_normal(), dir) > 0 ? m_radiance : gm::Color3f::black();
}




auto Glossy::sample(gm::Vec3f const& wo, Intersection const& isect, Sampler& sampler) const -> std::tuple<gm::Vec3f, FLOAT, gm::Color3f> {
    auto const n = (-wo + 2.0f * isect.shading_normal() * dot(isect.shading_normal(), wo)).normalise();
    const auto onb = gm::ONB{n};
    const auto v = sampler.cosine_sample_hemisphere();
    auto wi = onb.convert_to_local(v);
    if (dot(isect.shading_normal(), wi) < 0)
        wi = -v.x * onb.u() -v.y * onb.v() + v.z * onb.w();

    const auto phong_lobe = std::pow(dot(onb.w(), wi), m_exp);
    const auto pdf = phong_lobe * dot(isect.shading_normal(), wi);
    const auto color = m_ks * m_attenuation * phong_lobe;

    return {wi, pdf, color};
}

auto Glossy::evaluate(gm::Vec3f const& wo, Vec3f const& wi, Intersection const& isect) const -> gm::Color3f
{
    gm::Color3f color = gm::Color3f::black();
    const auto n_dot_wi = gm::dot(isect.shading_normal(), wi);
    gm::Vec3f r{-wi + 2.0f * isect.shading_normal() * n_dot_wi};
    const auto r_dot_wo = dot(r, wo);

    if (r_dot_wo > 0.0f)
        color = Color3f(m_ks * std::pow(r_dot_wo, m_exp));

    return color;
}

auto Glossy::pdf(Vec3f const& wi, Intersection const& isect) const -> FLOAT {
    const auto phong_lobe = std::pow(dot(isect.shading_normal(), wi), m_exp);
    return phong_lobe * dot(isect.shading_normal(), wi);
}

