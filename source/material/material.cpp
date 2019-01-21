#include "material.hpp"
#include "../math/normal3.hpp"
#include "../math/math_util.hpp"
#include "../core/intersection.hpp"

auto
Material::Sample(const Vec3f& wo, Vec3f& wi, float& pdf, const Vec3f& normal, Sampler& sampler) const -> Color3f {
    
    std::array<Vec3f, 3> onb{};

    onb[2] = normal;

    Vec3f a;
    if (normal.x > 0.9f) {
        a = Vec3f{0, 1, 0};
    }
    else {
        a = Vec3f{1, 0, 0};
    }

    onb[1] = Cross(onb[2], a).Normalize();
    onb[0] = Cross(onb[2], onb[1]);

    
    wi = sampler.CosineSampleHemisphere(normal);
    if (wo.z < 0) wi *= -1; // flip to match direction
    pdf = Pdf(onb[2], wi);
    return Evaluate(wo, wi); // called by derived class
}

auto
Material::Pdf(const Vec3f& wo, const Vec3f& wi) const -> float {
    //return Math::SameHemisphere(wo, wi) ? std::abs(wi.z) * Math::InvPi : 0;
    const auto cosine = Dot(wi, wo);
    if (cosine > 0)
    {
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
    //return Dot(normalAtLight, dir) > 0 ? m_radiance / distanceSquared : Color3f{0.0f};

    if (Dot(normalAtLight, dir) > 0) {
        Color3f light = m_radiance / distanceSquared;
        if (light.r > m_radiance.r || light.g > m_radiance.g || light.g > m_radiance.g) {
            return m_radiance;
        }
        return light;
        
            
    }
    return Color3f{0.0f};
}
