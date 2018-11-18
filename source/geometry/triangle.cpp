#include "triangle.hpp"
#include <cmath>
#include "../core/intersection.hpp"
#include "../math/transform.hpp"

triangle::triangle(
    std::array<Point3f, 3>     vertices,
    std::array<Normal3f, 3>    vertexNormals,
    std::array<Point2f, 3>     uv)
    : m_vertices(std::move(vertices))
    , m_vertexNormals(std::move(vertexNormals))
    , m_uv(std::move(uv))
{
    UpdateEdges();
}

triangle::triangle(
    std::array<Point3f, 3>     vertices,
    std::array<Normal3f, 3>    vertexNormals)
    : m_vertices(std::move(vertices))
    , m_vertexNormals(std::move(vertexNormals))
{
    //std::cout << "Triangle constructed\n";
    UpdateEdges();
}


auto 
triangle::UpdateEdges() -> void
{
    m_edges[0]   = m_vertices[1] - m_vertices[0];
    m_edges[1]   = m_vertices[2] - m_vertices[0];
    m_faceNormal = m_edges[0].CrossProduct(m_edges[1]).Normalize();
}

auto
triangle::Intersects(const Rayf& ray, Intersection& isect) -> bool
{
    const auto p_vec = ray.GetDirection().CrossProduct(m_edges.at(1));
    const auto det = Dot(m_edges[0], p_vec);

    const auto recipDet = 1 / det;

    Point2f barycentric{0};

    // barycentric coordinate u
    const auto t_vec = ray.GetOrigin() - m_vertices[0];
    barycentric.x = t_vec.DotProduct(p_vec) * recipDet;
    
    if (barycentric.x < 0 || barycentric.x > 1)
        return false;

    // barycentric coordinate v
    const auto q_vec = t_vec.CrossProduct(m_edges.at(0));
    barycentric.y = ray.GetDirection().DotProduct( q_vec ) * recipDet;
    
    if (barycentric.y < 0 || barycentric.y + barycentric.x > 1)
        return false;

    const auto parameter = m_edges.at(1).DotProduct(q_vec) * recipDet;

    if (!ray.ParameterWithinBounds(parameter))
        return false;


    // Update parameter and intersection as necessary
    ray.NewMaxParameter(parameter);
    isect.Update(ray.PointAtParameter(parameter), barycentric, m_faceNormal, InterpolateNormalAt(barycentric), this);
   
    return true;
}

auto
triangle::IntersectsFast(const Rayf& ray) const -> bool
{
    const auto p_vec = ray.GetDirection().CrossProduct(m_edges.at(1));
    const auto det = m_edges.at(0).DotProduct(p_vec);

    // precompute for performance
    const auto inverted_det = 1 / det;

    Vec2f barycentric{0};

    // barycentric coordinate u
    const auto t_vec = ray.GetOrigin() - m_vertices[0];
    barycentric.x = t_vec.DotProduct(p_vec) * inverted_det;
    if (barycentric.x < 0 || barycentric.x > 1)
        return false;

    // barycentric coordinate v
    const auto q_vec = t_vec.CrossProduct(m_edges.at(0));
    barycentric.y = ray.GetDirection().DotProduct( q_vec ) * inverted_det;
    if (barycentric.y < 0 || barycentric.y + barycentric.x > 1)
        return false;

    const auto parameter = m_edges.at(1).DotProduct(q_vec) * inverted_det;

    return parameter > 0 && parameter <= ray.GetMaxParameter();

}

auto 
triangle::InterpolateNormalAt(const Point2f& uv) const -> Normal3f
{
    // for flat shading simply return the face normal
    const auto normal = m_vertexNormals[0] * (1 - uv.x - uv.y)
                        + m_vertexNormals[1] * uv.x
                        + m_vertexNormals[2] * uv.y;

    return Normalize(normal);
}

auto
triangle::TransformBy(const Transform& transform) -> void
{
    for (auto& vertex : m_vertices)
        transform(vertex);

    for (auto& normal : m_vertexNormals)
        transform(normal);

    // precompute again
    UpdateEdges();
}

auto 
triangle::GetArea() const -> float
{
    return 0.5f * Cross(m_edges[0], m_edges[1]).Length();
}

auto
triangle::GetVertices() const -> const std::array<Point3f, 3>&
{
    return m_vertices;
}

/*
Point3f Triangle::GetPointOnSurface(const float u, const float v) const
{
    return m_vertices[0] + u * m_edges[0] + v * m_edges[1];
}

Point3f Triangle::GetRandomPointOnSurface() 
{
    return GetPointOnSurface(m_dist(m_gen), m_dist(m_gen));
}

Intersection Triangle::GetRandomSurfaceIntersection() 
{
    return Intersection{GetRandomPointOnSurface(), m_faceNormal};
}



auto
Triangle::GetBoundingVolume() const -> std::unique_ptr<AABB>
{
    return nullptr;
}
*/

