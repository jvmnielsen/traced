#pragma once
#include "Renderable.h"

class Sphere :
    public Renderable
{
public:
    Sphere(const Point3f& center, const float radius, const Color3f& albedo, MaterialType material)
        : Renderable(albedo, material)
        , m_center(center)
        , m_radius(radius)
        , m_radius_squared(radius * radius)
    {}

    bool Intersects(const Rayf &ray, Intersection &intersection) override;

    bool solve_quadratic(const float a, const float b, const float c, float& solutionOne, float& solutionTwo) const;

    void CalculateNormal(Intersection &intersection) const override;

    void SetMaterialType(const MaterialType& type) override
    {
        m_material = type;
    }

    void TransformBy(const Transform& transform) override;

private:
    Point3f m_center;
    float m_radius;
    float m_radius_squared;
	//Material* m_material;
    
};

