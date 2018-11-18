#pragma once

#include <random>
#include "../math/ray.hpp"
#include "../imaging/color3.hpp"
#include "../math/vec3.hpp"
#include "../math/point2.hpp"


class Intersection;

class Material  
{
public:

    Material();

    virtual bool Scatter(const Rayf& rayIn, const Intersection& isect, Color3f& attenuation, Rayf& scattered) = 0;

    virtual Color3f Emitted(const Point2f& uv, const Point3f& point) const { return {0.0f, 0.0f, 0.0f}; }

    virtual Color3f EvaluateBSDF(const Vec3f& w_o, const Vec3f& w_i) = 0;


protected:
    Vec3f RandomInUnitSphere();

    //std::shared_ptr<BSDF> m_bsdf;

    // to generate random numbers [0,1]
    //std::random_device m_seed;
    std::mt19937 m_gen;
    std::uniform_real_distribution<float> m_dist;
};


class Matte : public Material 
{
public:
    explicit Matte(const Color3f& a) : m_diffuse(a) {}

    Matte(const Matte& other) : m_diffuse(other.m_diffuse)
    {
        std::cout << "Copied matte!\n";
    }

    Matte(Matte&& other) : m_diffuse(other.m_diffuse)
    {
        std::cout << "Moved matte!\n";
    }

    bool Scatter(const Rayf& rayIn, const Intersection& isect, Color3f& attenuation, Rayf& scattered) override { return true; }

    Color3f EvaluateBSDF(const Vec3f& w_o, const Vec3f& w_i) override { return m_diffuse / Math::Pi; }

private:
    
    Color3f m_diffuse;
};

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