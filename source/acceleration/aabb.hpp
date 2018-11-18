//
// Created by Jacob Vesti Moeslund Nielsen on 18/11/2018.
//

#ifndef TRACED_AABB_HPP
#define TRACED_AABB_HPP

#include "../math/point3.hpp"
#include "../math/math_util.hpp"
#include "../geometry/mesh.hpp"

// Axis-aligned bounding-box
class AABB
{
public:

    AABB(Point3f lowerBound = Point3f{Math::Infinity}, Point3f upperBound = Point3f{-Math::Infinity});
    explicit AABB(std::unique_ptr<Mesh> mesh);

    AABB(const AABB& other);
    auto operator=(const AABB& other) -> AABB&;

    auto Intersects(const Rayf& ray, Intersection& isect) const -> bool;
    auto IntersectsQuick(const Rayf& ray) const -> bool;

    auto Diagonal()         const -> Vec3f;
    auto SurfaceArea()      const -> float;
    auto Center()           const -> const Point3f&;
    auto GetBounds()        const -> const std::array<Point3f, 2>&;
    auto LowerBound()       const -> const Point3f&;
    auto UpperBound()       const -> const Point3f&;
    auto MaximumExtent()    const -> int;
    auto CalculateCenter()  const -> Point3f;
    auto GetShape()         const -> const Mesh&;

private:
    bool IntersectsShape(const Rayf& ray, Intersection& isect) const;

    std::array<Point3f, 2> m_bounds;
    Point3f m_center;

    std::unique_ptr<Mesh> m_mesh;
};

#endif //TRACED_AABB_HPP
