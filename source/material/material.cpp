#include "material.hpp"
#include "../math/math_util.hpp"
#include "../core/intersection.hpp"

auto
Material::Sample(const Vec3f& wo, const Intersection& isect, Sampler& sampler) const -> std::tuple<Vec3f, double, Color3f> {
    
    auto wi = sampler.CosineSampleHemisphere(isect.GetOrthonormalBasis());
    if (wo.z < 0) wi *= -1; // flip to match direction
    const auto pdf = Pdf(wi, isect.GetOrthonormalBasis());
    return std::make_tuple(wi, pdf, Evaluate(wo, wi)); // called by derived class
}

auto
Material::Pdf(const Vec3f& dir, const ONB& basis) const -> float {
    //return Math::SameHemisphere(wo, wi) ? std::abs(wi.z) * Math::InvPi : 0;
    const auto cosine = Dot(Normalize(dir), basis[2]);
    if (cosine > 0) {
        return cosine / Math::Pi;
    }
    return 0;
}

auto
Material::Emitted(const Intersection& isect, const Vec3f& dir) const -> Color3f {
    return Color3f::Black();
}

auto
Matte::Evaluate(const Vec3f& wo, const Vec3f& wi) const -> Color3f {
    return m_attenuation * Math::InvPi;
}

auto
Emissive::Emitted(const Intersection& isect, const Vec3f& dir) const -> Color3f {
    return Dot(isect.GetShadingNormal(), dir) > 0 ? m_radiance : Color3f::Black();
}
