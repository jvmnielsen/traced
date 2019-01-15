#include "material.hpp"
#include "../math/normal3.hpp"
#include "../math/math_util.hpp"
#include "../core/intersection.hpp"

auto
Material::Emitted(const Intersection& atLight, const SamplingInfo& info) const -> Color3f {
    return Color3f{ 0.0f };
}

auto
Material::Pdf(SamplingInfo& info) const -> void {
    // we may have to change vectors into local coord system
    info.pdf = Math::SameHemisphere(info.toLight, info.toEye) ?
            std::abs(info.toEye.z) * Math::InvPi : 0;
}

auto
Matte::Sample(SamplingInfo& info, Sampler& sampler) const -> void {
    // Cosine-sample the hemisphere, flipping the direction if necessary
    info.toEye = sampler.CosineSampleHemisphere();
    if (info.toLight.z < 0) info.toEye.z *= -1;
    Pdf(info);
}

auto
Matte::Evaluate(const SamplingInfo& info) const -> Color3f {
    return m_albedo * Math::InvPi;
}



auto
Emissive::Emitted(const Intersection& atLight, const SamplingInfo& info) const -> Color3f {
    return Dot(atLight.GetGeometricNormal(), info.toEye) > 0 ? m_radiance : Color3f{0.0f};
}
