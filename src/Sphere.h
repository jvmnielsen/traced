#pragma once
#include "Shape.h"
#include "Intersection.h"

class Sphere :
    public Shape
{
public:
    Sphere() : Shape(nullptr) { }

    Sphere(const Point3f& center, const float radius, std::shared_ptr<Material> material)
        : Shape(material)
        , m_center(center)
        , m_radius(radius)
        , m_radius_squared(radius * radius)
    {}

    bool Intersects(const Rayf& ray, Intersection& isect) override;
    bool IntersectsQuick(const Rayf& ray) const override;

    std::unique_ptr<BoundingVolume> GetBoundingVolume() const override;

    inline Normal3f NormalAtIntesection(Intersection& isect) const override;

    inline void TransformBy(const Transform& transform) override;

private:
    Point3f m_center;
    float m_radius;
    float m_radius_squared;
	//Material* m_material;
    
};

