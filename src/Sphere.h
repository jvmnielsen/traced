#pragma once
#include "Shape.h"
#include "Intersection.h"

class Sphere :
    public Shape
{
public:
    Sphere(const Point3f& center, const float radius, const Color3f& albedo, std::shared_ptr<Material> material)
        : Shape(albedo, material)
        , m_center(center)
        , m_radius(radius)
        , m_radius_squared(radius * radius)
    {}

    bool Intersects(const Rayf& ray, Intersection& isect) override;

    BoundingVolume GetBoundingVolume() const override;

    void CalculateNormal(Intersection &intersection) const override;

    void TransformBy(const Transform& transform) override;

private:
    Point3f m_center;
    float m_radius;
    float m_radius_squared;
	//Material* m_material;
    
};

