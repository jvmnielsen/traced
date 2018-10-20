//
// Created by Jacob Vesti Moeslund Nielsen on 19/10/2018.
//

#include "BSDF.h"

Color3f Lambertian::EvaluateFiniteScatteringDensity(
        const Vec3f& wi,
        const Vec3f& wo) const
{
    return m_colorMatte / M_PI;
}

Color3f Phong::EvaluateFiniteScatteringDensity(
        const Vec3f& wi,
        const Vec3f& wo) const
{
    const Vec3f wh = (wi + wo) / 2;
    return (m_colorMatte + m_colorGlossy * ((m_smoothness + 8.0f)
                * powf(std::max(0.0f, wh.DotProduct(m_normal)), m_smoothness) / 8.0f)) / M_PI;
}