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

    virtual Color3f CalculateSurfaceColor(const Rayf& rayIn, const Intersection& isect, const Scene& scene, int depth) const = 0;

    virtual bool Scatter(const Rayf& rayIn, const Intersection& isect, Color3f& attenuation, Rayf& scattered) = 0;

protected:
    // to generate random numbers [0,1]
    //std::random_device m_seed;
    std::mt19937 m_gen;
    std::uniform_real_distribution<float> m_dist;
};

class Matte : public Material {
public:
    explicit Matte(const Color3f& a) : m_diffuse(a) {}

    Color3f CalculateSurfaceColor(const Rayf& rayIn, const Intersection& isect, const Scene& scene, int depth) const override;
    bool Scatter(const Rayf& rayIn, const Intersection& isect, Color3f& attenuation, Rayf& scattered) override;

private:
    Vec3f RandomInUnitSphere();
    Color3f m_diffuse;
};

class Plastic : public Material {
public:
    explicit Plastic(const Color3f& diffuse, const Color3f& specular, float smoothness)
        : m_diffuse(diffuse)
        , m_specular(specular)
        , m_smoothness(smoothness)
        {}

    Color3f CalculateSurfaceColor(const Rayf& rayIn, const Intersection& isect, const Scene& scene, int depth) const override;

    bool Scatter(const Rayf& rayIn, const Intersection& isect, Color3f& attenuation, Rayf& scattered) override { return true; }

    Color3f m_diffuse;
    Color3f m_specular;
    float   m_smoothness;
};

class Metal : public Material
{
    Color3f CalculateSurfaceColor(const Rayf& rayIn, const Intersection& isect, const Scene& scene, int depth) const override;

    bool Scatter(const Rayf& rayIn, const Intersection& isect, Color3f& attenuation, Rayf& scattered) override { return true; }

};

class Glass : public Material
{
    explicit Glass(float refractiveIndex) : m_refractiveIndex(refractiveIndex) { }


    Color3f CalculateSurfaceColor(const Rayf& rayIn, const Intersection& isect, const Scene& scene, int depth) const override;

    bool Scatter(const Rayf& rayIn, const Intersection& isect, Color3f& attenuation, Rayf& scattered) override { return true; }



    float m_refractiveIndex;
};