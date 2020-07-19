#pragma once
#include "../core/intersection.hpp"
#include "../acceleration/bounds.hpp"

#include <graphics-math.hpp>

#include <optional>

namespace tr {

    class Triangle {
    public:
        Triangle(std::array<gm::Point3f, 3>     vertices,
                 std::array<gm::Normal3f, 3>    vertex_normals,
                 std::array<gm::Point2f, 3>     uv);

        Triangle(std::array<gm::Point3f, 3>     vertices,
                 std::array<gm::Normal3f, 3>    vertex_normals);

        [[nodiscard]] auto intersects(Rayf const& ray) const -> std::optional<Intersection>;
        [[nodiscard]] auto interpolate_normal_at(gm::Point2f const& uv) const -> gm::Normal3f;
        auto transform_by(gm::Transform const& transform) -> void;
        [[nodiscard]] auto calculate_surface_area() const -> FLOAT;
        [[nodiscard]] auto calculate_bounds() const -> Bounds;
        auto sample_surface(Sampler& sampler) const -> std::tuple<Intersection, FLOAT>;
        [[nodiscard]] auto vertices() const -> const std::array<gm::Point3f, 3>&;

    private:
        std::array<gm::Point3f, 3>      m_vertices;         // Three points making up the triangle
        std::array<gm::Normal3f, 3>     m_vertex_normals;   // Normal at each vertex, used to interpolate a normal across the face
        gm::Normal3f                    m_face_normal;      // In cases where vertex normals are not available, use face normal
        std::array<gm::Vec3f, 2>        m_edges;            // Pre-calculated for use in Intercept function
        std::array<gm::Point2f, 3>      m_uv;               // Texture coordinates

        auto update_edges() -> void;
        [[nodiscard]] auto point_from_uv(gm::Point2f const& uv) const -> gm::Point3f;
    };
}