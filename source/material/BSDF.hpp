//
// Created by Jacob Vesti Moeslund Nielsen on 19/10/2018.
//
#pragma once

#include "../imaging/color3.hpp"
#include "../math/vec3.hpp"
#include "../math/normal3.hpp"
#include <vector>
#include "../core/intersection.hpp"
#include "../core/sampler.hpp"

//class Intersection;

//class Sampler;

enum BxDFType { Reflection = 1 << 0, Transmission = 1 << 1, Diffuse = 1 << 2, Glossy = 1 << 3, Specular = 1 << 4 };

class BxDF {
public:

    BxDF(BxDFType type) : m_type(type) { }

    virtual auto Evaluate(const Vec3f& wo, const Vec3f& wi) const ->Color3f = 0;
    virtual auto Pdf(const Normal3f& wo, const Normal3f& wi) const -> float;
    virtual auto Sample(const Normal3f& wo, Vec3f& wi, float& pdf, Sampler& sampler) const -> Color3f;

    BxDFType m_type;
};

class BSDF {
public:
    BSDF() = default;
    BSDF(const Intersection& isect, std::vector<std::unique_ptr<BxDF>> bxdfs);

    auto LocalToWorld(const Vec3f& v) const -> Vec3f;
    auto WorldToLocal(const Vec3f& v) const -> Vec3f;

    auto Evaluate(const Vec3f& wo, const Vec3f& wi) const -> Color3f;
    auto Pdf(const Normal3f& wo, const Normal3f& wi) const -> float;
    auto Sample(const Normal3f& wo, Normal3f& wi, float& pdf, Sampler& sampler) const -> Color3f;

private:
    std::vector<std::unique_ptr<BxDF>> m_bxdfs;
    const Normal3f m_geometricNormal;
    //const Vec3f m_shadingTangent;
    std::array<const Normal3f, 3> m_localAxis;
};



class Lambertian : public BxDF
{
public:
    Lambertian(const Color3f& radiance = Color3f{0.2f}) : BxDF(BxDFType(Reflection | Diffuse)), m_radiance(radiance) { }

    auto Evaluate(const Vec3f& wo, const Vec3f& wi) const -> Color3f override;
    //auto Pdf(const Normal3f& wo, const Normal3f& wi) const -> float override;
    //auto Sample(const Normal3f& wo, Normal3f& wi, float& pdf) const -> Color3f;

    Color3f m_radiance;
};


/*
class Phong : public BSDF
{
public:

     Phong(
            const Color3f& colorMatte,
            const Color3f& colorGlossy,
            float smoothness,
            const Vec3f& normal)
            : m_radiance(colorMatte)
            , m_colorGlossy(colorGlossy)
            , m_smoothness(smoothness)
            , m_normal(normal)
            { }

    Color3f Evaluate(
            const Vec3f& wi,
            const Vec3f& wo) const override;

    Color3f m_radiance;
    Color3f m_colorGlossy;
    float   m_smoothness;
    Vec3f   m_normal;
};
*/
