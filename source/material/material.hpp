#pragma once
#include "../math/ray.hpp"
#include "../imaging/color3.hpp"
#include "../core/intersection.hpp"


class Intersection;

class Material  
{
public:

    //Material();

    virtual auto ComputeScatteringFunctions(Intersection& isect) -> void = 0;

    //virtual bool Scatter(const Rayf& rayIn, const Intersection& isect, Color3f& attenuation, Rayf& scattered) = 0;

    //virtual Color3f Emitted(const Point2f& uv, const Point3f& point) const { return {0.0f, 0.0f, 0.0f}; }

    //virtual Color3f EvaluateBSDF(const Vec3f& w_o, const Vec3f& w_i) = 0;


protected:
    //Vec3f RandomInUnitSphere();

    std::unique_ptr<BSDF> m_bsdf;

    //std::shared_ptr<BSDF> m_bsdf;

    // to generate random numbers [0,1]
    //std::random_device m_seed;
    //std::mt19937 m_gen;
    //std::uniform_real_distribution<float> m_dist;
};



class Matte : public Material 
{
public:
    explicit Matte(const Color3f& albedo) {}

    auto ComputeScatteringFunctions(Intersection& isect) -> void override {
        //isect.m_bsdf = &m_lambertian;

        std::vector<std::unique_ptr<BxDF>> bxdfs;
        bxdfs.emplace_back(std::make_unique<Lambertian>(m_albedo));

        isect.m_bsdf = std::make_unique<BSDF>(isect, bxdfs);
    }

    Color3f m_albedo;
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