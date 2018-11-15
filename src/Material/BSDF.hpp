//
// Created by Jacob Vesti Moeslund Nielsen on 19/10/2018.
//

#ifndef RAYTRACER_BSDF_H
#define RAYTRACER_BSDF_H
#include "../Imaging/Color3.hpp"
#include "../Math/Vec3.hpp"


class BSDF
{
    virtual Color3f Evaluate(
        const Vec3f& wi,
        const Vec3f& wo) const = 0;

};

class Lambertian : public BSDF
{
public:
    explicit Lambertian(const Color3f& colorMatte = Color3f{0.2f}) : m_colorMatte(colorMatte) { }

    Color3f Evaluate(
            const Vec3f& wi,
            const Vec3f& wo) const override;

    Color3f m_colorMatte;
};

class Phong : public BSDF
{
public:

     Phong(
            const Color3f& colorMatte,
            const Color3f& colorGlossy,
            float smoothness,
            const Vec3f& normal)
            : m_colorMatte(colorMatte)
            , m_colorGlossy(colorGlossy)
            , m_smoothness(smoothness)
            , m_normal(normal)
            { }

    Color3f Evaluate(
            const Vec3f& wi,
            const Vec3f& wo) const override;

    Color3f m_colorMatte;
    Color3f m_colorGlossy;
    float   m_smoothness;
    Vec3f   m_normal;
};

#endif //RAYTRACER_BSDF_H
