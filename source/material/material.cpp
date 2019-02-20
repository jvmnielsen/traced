#include "material.hpp"
#include "../core/intersection.hpp"

auto
Material::Sample(const Normal3f& wo, const Intersection& isect, Sampler& sampler) const -> std::tuple<Normal3f, FLOAT, Color3f> {

    const auto localWo = isect.GetShadingBasis().WorldToLocal(wo);
    auto wi = sampler.CosineSampleHemisphere();
    if (wo.z < 0) wi = -wi; // flip to match direction
    const auto pdf = Pdf(wo, wi);
    return std::make_tuple(isect.GetShadingBasis().LocalToWorld(wi), pdf, Evaluate(wo, wi)); // called by derived class
}

auto
Material::Pdf(const Normal3f& wo, const Normal3f& wi) const -> FLOAT {
    return SameHemisphere(wo, wi) ? std::abs(wi.z) * Math::InvPi : 0;
    //return Dot(wi.w(), dir) / Math::Pi;
    /*
    const auto cosine = Dot(Normalize(dir), wi[2]);
    if (cosine > 0) {
        return cosine / Math::Pi;
    }
    return 0; */
}

auto
Material::Emitted(const Intersection& isect, const Normal3f& dir) const -> Color3f {
    return Color3f::Black();
}

auto
Matte::Evaluate(const Normal3f& wo, const Normal3f& wi) const -> Color3f {
    return m_attenuation * Math::InvPi;
}

auto
Emissive::Emitted(const Intersection& isect, const Normal3f& dir) const -> Color3f {
    return Dot(isect.GetGeometricNormal(), dir) > 0 ? m_radiance : Color3f::Black();
}
