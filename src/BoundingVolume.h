#pragma once
#include <memory>
//#include "Intersection.h"
//#include "Triangle.h"
#include "Mesh.h"

//class Shape;
//class Intersection;
//class Mesh;

/*
class AABB
{
public:
    explicit AABB(std::array<Point3f, 2> bounds);

    explicit AABB(std::unique_ptr<Mesh> mesh);

    //~AABB() = default;

    bool Intersects(const Rayf& ray, Intersection& isect) const;
    bool IntersectsQuick(const Rayf& ray) const;

    auto Diagonal() const -> Vec3f;

    auto SurfaceArea() const -> float;


    void SetShape(std::unique_ptr<Mesh> shape);
    Mesh& GetShape() const;

private:
    bool IntersectsShape(const Rayf& ray, Intersection& isect) const;

    std::array<Point3f, 2> m_bounds;
    std::unique_ptr<Mesh> m_mesh;
}; */

