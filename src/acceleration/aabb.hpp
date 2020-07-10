#pragma once


#include "bounds.hpp"
#include "../math/ray.hpp"

#include <optional>
#include <memory>

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


    auto intersects_mesh(const Rayf& ray) const -> std::optional<Intersection>;
    //auto IntersectsMeshFast(const Rayf& ray) const -> bool;
    auto intersects_bounds(const Rayf& ray) const -> bool;
    //auto IntersectsFast(const Rayf& ray) const -> bool;
    auto intersects(const Rayf& ray) const->std::optional<Intersection>;

    auto bounds() const -> const Bounds&;
    auto center() const -> const Point3f&;

private:
    //bool IntersectsShape(const Rayf& ray, Intersection& isect) const;

    
    Bounds m_bounds;
    Point3f m_center;
    std::unique_ptr<Mesh> m_mesh;
};
