#pragma once
#include "MathUtil.h"
#include "Imaging.h"
#include "Intersection.h"

class Material  {
public:
    virtual bool Scatter(const Rayf& rayIn, const Intersection& isect, Color3f& attenuation, Rayf& scattered) const = 0;
};

class Lambertian : public Material {
public:
    explicit Lambertian(const Color3f& a) : m_albedo(a) {}

    bool Scatter(const Rayf& rayIn, const Intersection& isect, Color3f& attenuation, Rayf& scattered) const override;

    Color3f m_albedo;
};
