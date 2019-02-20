#pragma once

#include "../core/sampler.hpp"
#include "../imaging/color3.hpp"
#include "../math/onb.hpp"


class Intersection;

class Material {
public:
    Material() = default;

    virtual auto Sample(const Normal3f& wo, const Intersection& isect, Sampler& sampler) const -> std::tuple<Normal3f, FLOAT, Color3f>;
    virtual auto Evaluate(const Normal3f& wo, const Normal3f& wi) const                           -> Color3f = 0;
            auto Pdf(const Normal3f& wo, const Normal3f& wi) const                                -> FLOAT;
    virtual auto Emitted(const Intersection& isect, const Normal3f& dir) const                       -> Color3f;

    //auto LocalToWorld(const Normal3f& n) const -> Normal3f;
    //auto WorldToLocal(const Normal3f& n) const -> Normal3f;


};



class Matte : public Material 
{
public:

    auto Evaluate(const Normal3f& wo, const Normal3f& wi) const -> Color3f override;

    Color3f m_attenuation = Color3f{ 0.38 };
};

class Emissive : public Matte
{
public:

    //auto Sample(SamplingInfo& info, Sampler& sampler) const -> void override; // set pdf as well
    //auto Evaluate(const SamplingInfo& info) const -> Color3f override;
    //auto Emitted() const -> Color3f override;
    

    auto Emitted(const Intersection& isect, const Normal3f& dir) const -> Color3f override;

    Color3f m_radiance = Color3f{1.0};

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