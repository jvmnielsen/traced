#pragma once

#include "../core/intersection.hpp"
//#include "../math/transform.hpp"
// #include "../acceleration/bvh.hpp"
#include "../acceleration/bounds.hpp"

#include <graphics-math.hpp>

#include <optional>

namespace tr {

    enum class IntersectOption {
        with_record,
        discard_record
    };

    class Triangle {
    public:

        Triangle(std::array<gm::Point3f, 3>     vertices,
                std::array<gm::Normal3f, 3>    vertex_normals,
                std::array<gm::Point2f, 3>     uv);

        Triangle(std::array<gm::Point3f, 3>     vertices,
                std::array<gm::Normal3f, 3>    vertex_normals);

        //bool Intersects(const Rayf &ray, Intersection& isect);
        bool IntersectsFast(const Rayf& ray) const;
        auto Intersects(const Rayf& ray) const -> std::optional<Intersection>;


        auto InterpolateNormalAt(gm::Point2f const & uv) const -> gm::Normal3f;

        auto TransformBy(gm::Transform const& transform) -> void;

        auto calculate_surface_area() const -> FLOAT;

        /*
        Point3f GetPointOnSurface(const float u, const float v) const override;
        Point3f GetRandomPointOnSurface() override;
        Intersection GetRandomSurfaceIntersection() override;
        */

        auto calculate_bounds() const -> Bounds;

        auto SampleSurface(Sampler& sampler) const -> std::tuple<Intersection, FLOAT>;

        const std::array<gm::Point3f, 3>& GetVertices() const;
        //std::unique_ptr<AABB> GetBoundingVolume() const override;


    private:
        std::array<gm::Point3f, 3>      m_vertices;         // Three points making up the triangle
        std::array<gm::Normal3f, 3>        m_vertex_normals;    // Normal at each vertex, used to interpolate a normal across the face
        gm::Normal3f                       m_face_normal;       // In cases where vertex normals are not available, use face normal
        std::array<gm::Vec3f, 2>        m_edges;            // Pre-calculated for use in Intercept function
        std::array<gm::Point2f, 3>      m_uv;               // Texture coordinates

        void UpdateEdges();

        auto GetPointFromUV(gm::Point2f const& uv) const -> gm::Point3f;

    };

}