#pragma once


#include "bounds.hpp"
#include "../geometry/ray.hpp"

#include <graphics-math.hpp>

#include <optional>
#include <memory>

namespace tr {

    class Intersection;
    class Mesh;

    // Axis-aligned bounding-box
    class AABB {
    public:

        explicit AABB(std::unique_ptr<Mesh> mesh);
        explicit AABB(Bounds bounds);

        AABB(std::unique_ptr<Mesh> mesh, Bounds bounds);

        AABB(AABB const& other);
        auto operator=(AABB const& other) -> AABB&;

        auto intersects_mesh(Rayf const& ray) const -> std::optional<Intersection>;
        //auto IntersectsMeshFast(const Rayf& ray) const -> bool;
        auto intersects_bounds(Rayf const& ray) const -> bool;
        //auto IntersectsFast(const Rayf& ray) const -> bool;
        auto intersects(Rayf const& ray) const->std::optional<Intersection>;

        auto bounds() const -> Bounds const&;
        auto center() const -> gm::Point3f const&;

    private:
        //bool IntersectsShape(const Rayf& ray, Intersection& isect) const;

        
        Bounds m_bounds;
        gm::Point3f m_center;
        std::unique_ptr<Mesh> m_mesh;
    };

}
