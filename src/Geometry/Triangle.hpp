#pragma once

#include <memory>
#include "../math/Point3.hpp"
#include "../Math/Normal3.hpp"
#include "../Math/Point2.hpp"
#include "../Math/Ray.hpp"
#include "../Math/Vec3.hpp"


class Intersection;
class Transform;

class Triangle 
{
public:

    Triangle(std::array<Point3f, 3>     vertices,
             std::array<Normal3f, 3>    vertexNormals,
             std::array<Point2f, 3>     uv);

    Triangle(std::array<Point3f, 3>     vertices,
             std::array<Normal3f, 3>    vertexNormals);

    Triangle(const Triangle& other);
    Triangle(Triangle&& other);

    bool Intersects(const Rayf &ray, Intersection& isect);
    bool IntersectsFast(const Rayf& ray) const;

    auto InterpolateNormalAt(const Point2f& uv) const -> Normal3f;

    auto TransformBy(const Transform& transform) -> void;

    auto GetArea() const -> float;

    /*
    Point3f GetPointOnSurface(const float u, const float v) const override;
    Point3f GetRandomPointOnSurface() override;
    Intersection GetRandomSurfaceIntersection() override;
    */


    const std::array<Point3f, 3>& GetVertices() const;
    //std::unique_ptr<AABB> GetBoundingVolume() const override;


private:
    std::array<Point3f, 3>      m_vertices;         // Three points making up the triangle
    std::array<Normal3f, 3>     m_vertexNormals;    // Normal at each vertex, used to interpolate a normal across the face
    Vec3f                       m_faceNormal;       // In cases where vertex normals are not available, use face normal
    std::array<Vec3f, 2>        m_edges;            // Pre-calculated for use in Intercept function
    std::array<Point2f, 3>      m_uv;               // Texture coordinates
         
    

	void UpdateEdges();
    

    
    //bool m_isSingleSided;
    //float m_epsilon = 1e-8f;
};

