#pragma once

#include <optional>
#include "bounds.hpp"
#include "../math/ray.hpp"

class Intersection;
class Mesh;

// Axis-aligned bounding-box
class AABB {
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

    //auto DoesNotContainMesh() const -> bool;

    auto center() const -> const Point3f&;

private:
    bool IntersectsShape(const Rayf& ray, Intersection& isect) const;

    
    Bounds m_bounds;
    Point3f m_center;
    std::unique_ptr<Mesh> m_mesh;
};
