#pragma once

#include "triangle.hpp"
#include "ray.hpp"
#include "../acceleration/bvh.hpp"

#include <graphics-math.hpp>

#include <optional>
#include <memory>
#include <vector>

namespace tr {

    class Mesh {
    public:
        Mesh(std::vector<Triangle> triangles, std::shared_ptr<Material> material, gm::Transform const& transform);
        Mesh(std::vector<Triangle> triangles);

        auto intersects(Rayf const& ray) const -> std::optional<Intersection>;
        auto calculate_surface_area() const -> FLOAT;
        auto calculate_bounds() const -> Bounds;
        auto transform_by(gm::Transform const& transform) -> void;
        auto sample_random_triangle(Sampler& sampler) const -> std::tuple<Intersection, FLOAT>;
        auto sample_as_light(Intersection const& ref, Sampler& sampler) const -> std::tuple<Intersection, gm::Vec3f, FLOAT, gm::Color3f>;
        auto triangle_count() const -> std::size_t;
        auto pdf(Intersection const& ref, gm::Vec3f const& wi) const -> FLOAT;

    private:
        auto intersects_internal_aabbs(Rayf const& ray) const -> std::optional<Intersection>;
        auto intersects_mesh_proper(Rayf const& ray) const -> std::optional<Intersection>;
        auto generate_internal_aabbs() -> void;
        auto generate_internal_bounding_boxes() const -> std::vector<Bounds>;
        auto assign_triangles_to_internal_bounds( std::vector<Bounds> const& internal_bounds) const -> std::vector<AABB>;

        std::vector<AABB>           m_internal_bounding;
        std::vector<Triangle>       m_triangles;
        std::shared_ptr<Material>   m_material;
        FLOAT                       m_surface_area;
    };
}
