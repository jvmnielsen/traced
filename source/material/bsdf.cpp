//
// Created by Jacob Vesti Moeslund Nielsen on 19/10/2018.
//

#include "bsdf.hpp"
#include "../math/math_util.hpp"

Color3f Lambertian::Evaluate(
        const Vec3f& wi,
        const Vec3f& wo) const
{
    return m_colorMatte / Math::Pi;
}

Color3f Phong::Evaluate(
        const Vec3f& wi,
        const Vec3f& wo) const
{
    const Vec3f wh = (wi + wo) / 2;
    return (m_colorMatte + m_colorGlossy * ((m_smoothness + 8.0f)
                * powf(std::max(0.0f, wh.DotProduct(m_normal)), m_smoothness) / 8.0f)) / Math::Pi;
}