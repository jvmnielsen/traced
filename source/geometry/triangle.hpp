#pragma once

#include <memory>
#include "../math/point3.hpp"
#include "../math/normal3.hpp"
#include "../math/point2.hpp"
#include "../math/ray.hpp"
#include "../math/vec3.hpp"
#include <optional>
#include <random>
#include "../core/intersection.hpp"
#include "../math/transform.hpp"

class Triangle
{
public:

    Triangle(std::array<Point3f, 3>     vertices,
             std::array<Normal3f, 3>    vertexNormals,
             std::array<Point2f, 3>     uv);

    Triangle(std::array<Point3f, 3>     vertices,
             std::array<Normal3f, 3>    vertexNormals);

    //bool Intersects(const Rayf &ray, Intersection& isect);
    bool IntersectsFast(const Rayf& ray) const;
    auto Intersects(const Rayf& ray) const -> std::optional<Intersection>;


    auto InterpolateNormalAt(const Point2f& uv) const -> Normal3f;

    auto TransformBy(const Transform& transform) -> void;

    auto GetArea() const -> float;

    /*
    Point3f GetPointOnSurface(const float u, const float v) const override;
    Point3f GetRandomPointOnSurface() override;
    Intersection GetRandomSurfaceIntersection() override;
    */

    auto SampleSurface(float& pdf, Sampler& sampler) const -> Intersection;

    const std::array<Point3f, 3>& GetVertices() const;
    //std::unique_ptr<AABB> GetBoundingVolume() const override;


private:
    std::array<Point3f, 3>      m_vertices;         // Three points making up the triangle
    std::array<Normal3f, 3>     m_vertexNormals;    // Normal at each vertex, used to interpolate a normal across the face
    Vec3f                       m_faceNormal;       // In cases where vertex normals are not available, use face normal
    std::array<Vec3f, 2>        m_edges;            // Pre-calculated for use in Intercept function
    std::array<Point2f, 3>      m_uv;               // Texture coordinates

	void UpdateEdges();

    auto GetPointFromUV(const Point2f& uv) const -> Point3f;

};

