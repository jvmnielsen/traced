#pragma once

#include "../math/point3.hpp"
#include "../math/math_util.hpp"
//#include "../geometry/mesh.hpp"
#include <optional>
#include "../math/ray.hpp"
#include "../core/intersection.hpp"

class Triangle;
class Mesh;

class Bounds {

    Point3f m_lower;
    Point3f m_upper;
    
public:
    Bounds(Point3f& lower, Point3f upper) : m_lower(std::move(lower)), m_upper(std::move(upper)) { }
    auto Lower() const -> const Point3f& { return m_lower; }
    auto Upper() const -> const Point3f& { return m_upper; }

    auto Overlaps(const Bounds& other) const -> bool;
    auto IsInside(const Point3f& point) const -> bool;

    auto Diagonal()         const->Vec3f;
    auto SurfaceArea()      const -> FLOAT;
    auto CalculateCenter()           const -> Point3f;
    auto MaximumExtent()    const -> int;

};

// Axis-aligned bounding-box
class AABB
{
public:

    //AABB(Point3f lowerBound = Point3f{Math::Infinity}, Point3f upperBound = Point3f{-Math::Infinity});
    explicit AABB(std::unique_ptr<Mesh> mesh);

    AABB(std::unique_ptr<Mesh> mesh, Bounds bounds);

    //AABB(const AABB& other);
    //auto operator=(const AABB& other) -> AABB&;


    auto IntersectsMesh(const Rayf& ray) const -> std::optional<Intersection>;
    auto IntersectsMeshFast(const Rayf& ray) const -> bool;
    auto IntersectsBox(const Rayf& ray) const -> bool;
    auto IntersectsFast(const Rayf& ray) const -> bool;
    auto Intersects(const Rayf& ray) const->std::optional<Intersection>;


    //auto Diagonal()         const -> Vec3f;
    //auto SurfaceArea()      const -> float;
    //auto Center()           const -> const Point3f&;
    auto GetBounds()        const -> const Bounds&;
    //auto LowerBound()       const -> const Point3f&;
    //auto UpperBound()       const -> const Point3f&;
    //auto MaximumExtent()    const -> int;
    //auto CalculateCenter()  const -> Point3f;
    auto GetShape()         const -> const Mesh&;

    auto SetMesh(std::unique_ptr<Mesh> mesh) { m_mesh = std::move(mesh); }
    auto DoesNotContainMesh() const -> bool;

private:
    bool IntersectsShape(const Rayf& ray, Intersection& isect) const;

    Bounds m_bounds;
    Point3f m_center;

    std::unique_ptr<Mesh> m_mesh;
};