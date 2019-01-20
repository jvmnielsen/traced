#pragma once

#include "../core/sampler.hpp"
#include "../imaging/color3.hpp"

struct SamplingInfo;
class Intersection;

class Material {
public:

    virtual auto Sample(const Vec3f& wo, Vec3f& wi, float& pdf, Sampler& sampler) const -> Color3f;
    virtual auto Evaluate(const Vec3f& wo, const Vec3f& wi) const                       -> Color3f = 0;
            auto Pdf(const Vec3f& wo, const Vec3f& wi) const                            -> float;
    virtual auto Emitted(const Vec3f& normalAtLight, const Vec3f& dir, float distanceSquared) const            -> Color3f;

};



class Matte : public Material 
{
public:

    auto Evaluate(const Vec3f& wo, const Vec3f& wi) const -> Color3f override;

    Color3f m_attenuation = Color3f{ 0.18f };
};

class Emissive : public Matte
{
public:

    //auto Sample(SamplingInfo& info, Sampler& sampler) const -> void override; // set pdf as well
    //auto Evaluate(const SamplingInfo& info) const -> Color3f override;
    //auto Emitted() const -> Color3f override;


    auto Emitted(const Vec3f& normalAtLight, const Vec3f& dir, float distanceSquared) const -> Color3f override;


    Color3f m_radiance = Color3f{20.5f};

};


/*

class Metal : public Material
{
public:
    Metal(const Color3f& albedo, float fuzz) : m_albedo(albedo), m_fuzz(fuzz) { }

    bool Scatter(const Rayf& rayIn, const Intersection& isect, Color3f& attenuation, Rayf& scattered) override { return true; }

    Color3f EvaluateBSDF(const Vec3f& w_o, const Vec3f& w_i) override { return Color3f{0.4f}; }

    float m_fuzz;

private:
    Color3f m_albedo;
};

class Dielectric : public Material
{
public:
    explicit Dielectric(float refractiveIndex) : m_refractiveIndex(refractiveIndex) { }

    bool Scatter(const Rayf& rayIn, const Intersection& isect, Color3f& attenuation, Rayf& scattered) override { return true; }

    Color3f EvaluateBSDF(const Vec3f& w_o, const Vec3f& w_i) override { return Color3f{0.4f}; }

private:
    float m_refractiveIndex;
};

class DiffuseLight : public Material
{
    Color3f EvaluateBSDF(const Vec3f& w_o, const Vec3f& w_i) override { return Color3f{0.4f}; }

    bool Scatter(const Rayf& rayIn, const Intersection& isect, Color3f& attenuation, Rayf& scattered) override { return false; }
    Color3f Emitted(const Point2f& uv, const Point3f& point) const override { return Color3f{40.0f}; }
};

class Emissive : public Material {
public:
    Color3f Emitted(const Point2f& uv, const Point3f& point) const override { return Color3f{40.0f}; }
};

*/