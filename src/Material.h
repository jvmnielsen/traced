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
    explicit Matte(const Color3f& a) : m_albedo(a) {}

    Color3f CalculateSurfaceColor(const Rayf& rayIn, const Intersection& isect, const Scene& scene) const override;

    Color3f m_albedo;
};

