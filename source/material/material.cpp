#include "material.hpp"
#include "../math/math_util.hpp"
#include "../core/intersection.hpp"

auto
Material::Sample(const Vec3f& wo, Vec3f& wi, float& pdf, const Intersection& isect, Sampler& sampler) const -> Color3f {
    
    wi = sampler.CosineSampleHemisphere(isect.GetOrthonormalBasis());
    if (wo.z < 0) wi *= -1; // flip to match direction
    pdf = Pdf(wi, isect.GetOrthonormalBasis());
    return Evaluate(wo, wi); // called by derived class
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
Material::Emitted(const Vec3f& normalAtLight, const Vec3f& dir, float distanceSquared) const -> Color3f {
    return Color3f{0.0f};
}

auto
Matte::Evaluate(const Vec3f& wo, const Vec3f& wi) const -> Color3f {
    return m_attenuation * Math::InvPi;
}

auto
Emissive::Emitted(const Vec3f& normalAtLight, const Vec3f& dir, float distanceSquared) const -> Color3f {
    //return Dot(normalAtLight, dir) > 0 ? Color3f{100.0f} / distanceSquared : Color3f{0.0f};


    Color3f m_radiance = Color3f{1.0f};

    if (Dot(normalAtLight, dir) > 0) {

        //return m_radiance;
        Color3f light = m_radiance / distanceSquared;
        if (light.r > m_radiance.r || light.g > m_radiance.g || light.g > m_radiance.g) {
            return m_radiance;
        }
        return light;
        
            
    }
    return Color3f{0.0f};
}
