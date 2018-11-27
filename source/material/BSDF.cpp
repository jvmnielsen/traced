//
// Created by Jacob Vesti Moeslund Nielsen on 19/10/2018.
//

#include "bsdf.hpp"
#include "../math/math_util.hpp"

auto
Lambertian::Evaluate(const Vec3f& wo, const Vec3f& wi) const -> Color3f {

}

auto
Lambertian::Pdf(const Normal3f& wo, const Normal3f& wi) const -> float {

}

auto
Lambertian::Sample(const Normal3f& wo, Normal3f& wi, float& pdf) const -> Color3f {

}


/*
Color3f Phong::Evaluate(
        const Vec3f& wi,
        const Vec3f& wo) const
{
    const Vec3f wh = (wi + wo) / 2;
    return (m_colorMatte + m_colorGlossy * ((m_smoothness + 8.0f)
                * powf(std::max(0.0f, wh.DotProduct(m_normal)), m_smoothness) / 8.0f)) / Math::Pi;
}*/