#pragma once

#include "../core/sampler.hpp"
#include "../imaging/color3.hpp"
#include "../math/onb.hpp"


class Intersection;

class Material {
public:
    Material() = default;

    virtual auto sample(const Vec3f& wo, const Intersection& isect, Sampler& sampler) const -> std::tuple<Vec3f, FLOAT, Color3f>;
    virtual auto evaluate(const Vec3f& wo, const Vec3f& wi, const Intersection& isect) const -> Color3f = 0;
    virtual auto pdf(const Vec3f& dir, const Intersection& isect) const -> FLOAT;
    virtual auto emitted(const Intersection& isect, const Vec3f& dir) const -> Color3f;
};

class Matte : public Material 
{
public:

    explicit Matte(Color3f attenuation = Color3f{0.18}) : m_attenuation(attenuation) {}

    auto evaluate(const Vec3f& wo, const Vec3f& wi, const Intersection& isect) const -> Color3f override;

private:
    Color3f m_attenuation;
};

class Emissive : public Matte
{
public:
    auto emitted(const Intersection& isect, const Vec3f& dir) const -> Color3f override;

    Color3f m_radiance = Color3f{1.0};

};


class Glossy : public Material
{
public:
    auto sample(const Vec3f& wo, const Intersection& isect, Sampler& sampler) const -> std::tuple<Vec3f, FLOAT, Color3f> override;
    auto evaluate(const Vec3f& wo, const Vec3f& wi, const Intersection& isect) const -> Color3f override;
    auto pdf(const Vec3f& dir, const Intersection& isect) const -> FLOAT override;

private:
    const Color3f m_attenuation = Color3f{0.1f, 0.2, 0.6f};
    FLOAT m_ks = 0.7;
    FLOAT m_exp = 100.0f;


};