#pragma once
#include "MathUtil.h"
#include "Imaging.h"
#include "Intersection.h"
#include "Scene.h"
#include <random>

class Material  {
public:

    Material() 
        : m_gen(std::random_device()())
        , m_dist(0.0f, 1.0f)
    { }

    virtual bool Scatter(const Rayf& rayIn, const Intersection& isect, Color3f& attenuation, Rayf& scattered) = 0;

    virtual Color3f Emitted(float u, float v, const Point3f& point) const { return {0.0f, 0.0f, 0.0f}; }


protected:
    Vec3f RandomInUnitSphere();

    // to generate random numbers [0,1]
    //std::random_device m_seed;
    std::mt19937 m_gen;
    std::uniform_real_distribution<float> m_dist;
};

class Matte : public Material {
public:
    explicit Matte(const Color3f& a) : m_diffuse(a) {}

    bool Scatter(const Rayf& rayIn, const Intersection& isect, Color3f& attenuation, Rayf& scattered) override;

private:
    
    Color3f m_diffuse;
};

class Metal : public Material
{
public:
    Metal(const Color3f& albedo, float fuzz) : m_albedo(albedo), m_fuzz(fuzz) { }

    bool Scatter(const Rayf& rayIn, const Intersection& isect, Color3f& attenuation, Rayf& scattered) override;

    float m_fuzz;

private:
    Color3f m_albedo;
};

class Dielectric : public Material
{
public:
    explicit Dielectric(float refractiveIndex) : m_refractiveIndex(refractiveIndex) { }

    bool Scatter(const Rayf& rayIn, const Intersection& isect, Color3f& attenuation, Rayf& scattered) override;

private:
    float m_refractiveIndex;
};

class DiffuseLight : public Material
{
    bool Scatter(const Rayf& rayIn, const Intersection& isect, Color3f& attenuation, Rayf& scattered) override { return false; }
    Color3f Emitted(float u, float v, const Point3f& point) const override { return Color3f{70.0f}; }   
};