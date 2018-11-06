#pragma once
#include <memory>
//#include "Intersection.h"
//#include "Triangle.h"
#include "Mesh.h"

//class Shape;
//class Intersection;
//class Mesh;

class BoundingVolume
{
public:
    explicit BoundingVolume(
        const Point3f& lowerBound = Point3f{0},
        const Point3f& upperBound = Point3f{0});

    //~BoundingVolume() = default;

    bool Intersects(const Rayf& ray, Intersection& isect) const;
    bool IntersectsQuick(const Rayf& ray) const;

    void SetShape(std::unique_ptr<Mesh> shape);
    Mesh& GetShape() const;

private:
    bool IntersectsShape(const Rayf& ray, Intersection& isect) const;

    std::array<Point3f, 2> m_bounds;

    std::shared_ptr<Mesh> m_mesh;
};

