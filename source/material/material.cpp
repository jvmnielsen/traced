#include "material.hpp"
#include "../core/intersection.hpp"

auto
Material::Sample(const Vec3f& wo, const Intersection& isect, Sampler& sampler) const -> std::tuple<Vec3f, FLOAT, Color3f> 
{

    //const auto wo = isect.GetShadingBasis().WorldToLocal(worldWo);
    //if (wo.z == 0) return
    auto wi = isect.GetShadingBasis().convert_to_local(sampler.CosineSampleHemisphere());
    //if (wo.z < 0) wi = -wi; // flip to match direction
    const auto pdf = Pdf(isect, wi);
    //const auto wiWorld = isect.GetShadingBasis().LocalToWorld(wi);
    return std::make_tuple(wi, pdf, Evaluate(wo, wi)); // called by derived class
}

auto
Material::Pdf(const Intersection& isect, const Vec3f& wi) const -> FLOAT {
    //return SameHemisphere(wo, wi) ? std::abs(wi.z) * Math::InvPi : 0;
    return dot(isect.get_shading_normal(), wi) / Math::Constants::Pi;
    /*
    const auto cosine = Dot(Normalize(dir), wi[2]);
    if (cosine > 0) {
        return cosine / Math::Pi;
    }
    return 0; */
}

auto
Material::Emitted(const Intersection& isect, const Vec3f& dir) const -> Color3f {
    return Color3f::Black();
}

auto
Matte::Evaluate(const Vec3f& wo, const Vec3f& wi) const -> Color3f {
    return m_attenuation * Math::Constants::Pi;
}

auto
Emissive::Emitted(const Intersection& isect, const Vec3f& dir) const -> Color3f {
    return dot(isect.get_shading_normal(), dir) > 0 ? m_radiance : Color3f::Black();
}
