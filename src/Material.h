#pragma once
#include "MathUtil.h"
#include "Imaging.h"
#include "Intersection.h"
#include "Scene.h"

class Material  {
public:
    virtual Color3f CalculateSurfaceColor(const Rayf& rayIn, const Intersection& isect, const Scene& scene) const = 0;
};

class Matte : public Material {
public:
    explicit Matte(const Color3f& a) : m_diffuse(a) {}

    Color3f CalculateSurfaceColor(const Rayf& rayIn, const Intersection& isect, const Scene& scene) const override;

    Color3f m_diffuse;
};

class Plastic : public Material {
public:
    explicit Plastic(const Color3f& diffuse, const Color3f& specular, float smoothness)
        : m_diffuse(diffuse)
        , m_specular(specular)
        , m_smoothness(smoothness)
        {}

    Color3f CalculateSurfaceColor(const Rayf& rayIn, const Intersection& isect, const Scene& scene) const override;

    Color3f m_diffuse;
    Color3f m_specular;
    float   m_smoothness;
};

