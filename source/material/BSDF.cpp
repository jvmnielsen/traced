//
// Created by Jacob Vesti Moeslund Nielsen on 19/10/2018.
//

#include "bsdf.hpp"
#include "../math/math_util.hpp"
#include "../core/intersection.hpp"

auto 
BxDF::Pdf(const Normal3f& wo, const Normal3f& wi) const -> float {
    return Math::SameHemisphere(wo, wi) ? std::abs(wi.z) * Math::InvPi : 0;
}

auto 
BxDF::Sample(const Normal3f& wo, Vec3f& wi, float& pdf, Sampler& sampler) const -> Color3f {
    // Cosine-sample the hemisphere, flipping the direction if necessary
    wi = sampler.CosineSampleHemisphere();
    if (wo.z < 0) wi.z *= -1;
    pdf = Pdf(wo, wi);
   
    return Evaluate(wo, wi); // this is being called by a derived class
}


BSDF::BSDF(const Intersection& isect, std::vector<std::unique_ptr<BxDF>> bxdfs)
    : m_bxdfs(std::move(bxdfs)),
      m_geometricNormal(isect.GetGeometricNormal()),
    m_localAxis({isect.GetTangent(), Cross(isect.GetShadingNormal(), isect.GetTangent()), isect.GetShadingNormal()}) {
}

auto 
BSDF::LocalToWorld(const Vec3f& v) const -> Vec3f {
    return Vec3f{ m_localAxis[0].x * v.x + m_localAxis[1].x * v.y + m_localAxis[2].x * v.z,
                  m_localAxis[0].y * v.x + m_localAxis[1].y * v.y + m_localAxis[2].y * v.z,
                  m_localAxis[0].z * v.x + m_localAxis[1].z * v.y + m_localAxis[2].z * v.z };
}

auto 
BSDF::WorldToLocal(const Vec3f& v) const -> Vec3f {
    // http://www.pbr-book.org/3ed-2018/Materials/BSDFs.html
    return Vec3f{ Dot(v, m_localAxis[0]), Dot(v, m_localAxis[1]), Dot(v, m_localAxis[2]) };
}

auto 
BSDF::Evaluate(const Vec3f& wo, const Vec3f& wi) const -> Color3f {
    Vec3f woLocal = WorldToLocal(wo), wiLocal = WorldToLocal(wi);
    // figure out why this is
    if (woLocal.z == 0) return Color3f{0.0f};
    // is wo in the same hemisphere as wi
    const auto doesReflect = Dot(wiLocal, m_geometricNormal) * Dot(woLocal, m_geometricNormal) > 0;
    Color3f radiance{0.0f};
    for (const auto& bxdf : m_bxdfs) {
        // sanity check angle between the two
        if ((doesReflect && bxdf->m_type == BxDFType::Reflection) || (!doesReflect && bxdf->m_type == BxDFType::Transmission)) {
            radiance += bxdf->Evaluate(woLocal, wiLocal);
        }
    }
}

auto 
BSDF::Pdf(const Vec3f& wo, const Vec3f& wi) const -> float {
    //if (m_bxdfs.size() == 0) return 0.0f; TODO: make assertion
    Vec3f woLocal = WorldToLocal(wo), wiLocal = WorldToLocal(wi);
    if (woLocal.z == 0) return 0.0f;
    auto pdf = 0.0f;
    for (const auto& bxdf : m_bxdfs) {
        pdf += bxdf->Pdf(woLocal, wiLocal);
    }
    pdf /= m_bxdfs.size();
    return pdf;
}

auto 
BSDF::Sample(const Vec3f& wo, Vec3f& wi, float& pdf, Sampler& sampler) const -> Color3f {
    Vec3f woLocal = WorldToLocal(wo), wiLocal;
    pdf = 0.0f;
    auto radiance = m_bxdfs[0]->Sample(woLocal, wiLocal, pdf, sampler);
    wi = wiLocal;
    // TODO: finish up function
    return radiance;
}

auto 
BSDF::GetType() const -> BxDFType {
    // todo: expand so it includes all
    return m_bxdfs[0]->m_type;
}

auto 
Lambertian::Evaluate(const Vec3f& wo, const Vec3f& wi) const -> Color3f {
    return m_radiance * Math::InvPi;
}

/*
Color3f Phong::Evaluate(
        const Vec3f& wi,
        const Vec3f& wo) const
{
    const Vec3f wh = (wi + wo) / 2;
    return (m_radiance + m_colorGlossy * ((m_smoothness + 8.0f)
                * powf(std::max(0.0f, wh.DotProduct(m_normal)), m_smoothness) / 8.0f)) / Math::Pi;
}*/