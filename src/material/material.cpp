#include "material.hpp"
#include "../core/intersection.hpp"
#include <cmath>

auto
Material::sample(const Vec3f& wo, const Intersection& isect, Sampler& sampler) const -> std::tuple<Vec3f, FLOAT, Color3f>
{

    //const auto wo = isect.GetShadingBasis().WorldToLocal(worldWo);
    //if (wo.z == 0) return
    auto wi = isect.GetShadingBasis().convert_to_local(sampler.CosineSampleHemisphere());
    //if (wo.z() < 0) wi = -wi; // flip to match direction
    const auto pdf = Material::pdf(wi, isect);
    //const auto wiWorld = isect.GetShadingBasis().LocalToWorld(wi);
    return std::make_tuple(wi, pdf, evaluate(wo, wi, isect)); // called by derived class
}

auto
Material::pdf(const Vec3f& dir, const Intersection& isect) const -> FLOAT {
    /*
    const auto cosine = dot(dir, isect.shading_normal());
    if (cosine > 0)
        return cosine / Math::Constants::Pi;
    return 0 ; */
    return dot(isect.shading_normal(), dir) / Math::Constants::Pi;
   
}

auto
Material::emitted(const Intersection& isect, const Vec3f& dir) const -> Color3f {
    return Color3f::Black();
}

auto
Matte::evaluate(const Vec3f& wo, const Vec3f& wi, const Intersection& isect) const -> Color3f {
    //return same_hemisphere(wo, wi) ? m_attenuation * Math::Constants::Pi : Color3f::Black();
    return m_attenuation * Math::Constants::Pi;
}

auto
Emissive::emitted(const Intersection& isect, const Vec3f& dir) const -> Color3f {
    return dot(dir, isect.shading_normal()) > 0 ? m_radiance : Color3f::Black();
}




auto Glossy::sample(const Vec3f& wo, const Intersection& isect, Sampler& sampler) const -> std::tuple<Vec3f, FLOAT, Color3f>
{
    const auto onb = ONB{-wo + isect.shading_normal() * 2.0f * dot(wo, isect.shading_normal())};
    const auto v = sampler.CosineSampleHemisphere();
    auto wi = onb.convert_to_local(v);
    if (dot(isect.shading_normal(), wi) < 0)
        wi = -v.x() * onb.u() -v.y() * onb.v() + onb.w() * v.z();

    const auto phong_lobe = std::pow(dot(onb.w(), wi), m_exp);
    const auto pdf = phong_lobe * dot(isect.shading_normal(), wi);
    const auto color = m_ks * m_attenuation * phong_lobe;

    return {wi, pdf, color};
}

auto Glossy::evaluate(const Vec3f& wo, const Vec3f& wi, const Intersection& isect) const -> Color3f
{
    Color3f color = Color3f::Black();
    const auto n_dot_wi = dot(isect.shading_normal(), wi);
    Vec3f r{-wi + 2.0f * isect.shading_normal() * n_dot_wi};
    const auto r_dot_wo = dot(r, wo);

    if (r_dot_wo > 0.0f)
        color = Color3f{m_ks * std::pow(r_dot_wo, m_exp)};

    return color;
}

auto Glossy::pdf(const Vec3f& wi, const Intersection& isect) const -> FLOAT
{
    const auto phong_lobe = std::pow(dot(isect.shading_normal(), wi), m_exp);
    return phong_lobe * dot(isect.shading_normal(), wi);
}

