#pragma once

#include "../math/point3.hpp"
#include "../math/math_util.hpp"
//#include "../geometry/mesh.hpp"
#include <optional>
#include "../math/ray.hpp"
#include "../core/intersection.hpp"
#include "../geometry/bounds.hpp"

//class Bounds;
class Triangle;
class Mesh;

// Axis-aligned bounding-box
class AABB
{
public:

    explicit AABB(std::unique_ptr<Mesh> mesh);
    explicit AABB(Bounds bounds);

    AABB(std::unique_ptr<Mesh> mesh, Bounds bounds);

    AABB(const AABB& other);
    auto operator=(const AABB& other) -> AABB&;


    auto IntersectsMesh(const Rayf& ray) const -> std::optional<Intersection>;
    auto IntersectsMeshFast(const Rayf& ray) const -> bool;
    auto intersects_bounds(const Rayf& ray) const -> bool;
    auto IntersectsFast(const Rayf& ray) const -> bool;
    auto Intersects(const Rayf& ray) const->std::optional<Intersection>;

    auto bounds()        const -> const Bounds&;
    auto GetShape()         const -> const Mesh&;

    auto SetMesh(std::unique_ptr<Mesh> mesh) { m_mesh = std::move(mesh); }
    //auto DoesNotContainMesh() const -> bool;

    auto center() const -> const Point3f&;

private:
    bool IntersectsShape(const Rayf& ray, Intersection& isect) const;

    
    Bounds m_bounds;
    Point3f m_center;
    std::unique_ptr<Mesh> m_mesh;
};
