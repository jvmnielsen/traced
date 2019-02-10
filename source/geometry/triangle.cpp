#include "triangle.hpp"
#include <cmath>
#include "../core/intersection.hpp"
#include "../math/transform.hpp"

Triangle::Triangle(
    std::array<Point3f, 3>     vertices,
    std::array<Normal3f, 3>    vertexNormals,
    std::array<Point2f, 3>     uv)
        : m_vertices(std::move(vertices))
        , m_vertexNormals(std::move(vertexNormals))
        , m_uv(std::move(uv))
{
    UpdateEdges();
}

Triangle::Triangle(
    std::array<Point3f, 3>     vertices,
    std::array<Normal3f, 3>    vertexNormals)
        : m_vertices(std::move(vertices))
        , m_vertexNormals(std::move(vertexNormals))
{
    UpdateEdges();
}


auto 
Triangle::UpdateEdges() -> void
{
    m_edges[0]   = m_vertices[1] - m_vertices[0];
    m_edges[1]   = m_vertices[2] - m_vertices[0];
    m_faceNormal = m_edges[0].CrossProduct(m_edges[1]).Normalize();
}

/*
auto
Triangle::Intersects(const Rayf& ray, Intersection& isect) -> bool
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
    //isect.Update(ray.PointAtParameter(parameter), barycentric, m_faceNormal, InterpolateNormalAt(barycentric), this);
   
    return true;
} */

auto
Triangle::IntersectsFast(const Rayf& ray) const -> bool
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
Triangle::Intersects(const Rayf& ray) const -> std::optional<Intersection>
{
    const auto p_vec = Cross(ray.GetDirection(), m_edges.at(1));
    const auto det = Dot(m_edges[0], p_vec);

    const auto recipDet = 1 / det;

    Point2f barycentric;

    // barycentric coordinate u
    const auto t_vec = ray.GetOrigin() - m_vertices[0];
    barycentric.x = t_vec.DotProduct(p_vec) * recipDet;

    if (barycentric.x < 0 || barycentric.x > 1)
        return std::nullopt;

    // barycentric coordinate v
    const auto q_vec = t_vec.CrossProduct(m_edges.at(0));
    barycentric.y = ray.GetDirection().DotProduct(q_vec) * recipDet;

    if (barycentric.y < 0 || barycentric.y + barycentric.x > 1)
        return std::nullopt;

    const auto parameter = m_edges.at(1).DotProduct(q_vec) * recipDet;

    if (!ray.ParameterWithinBounds(parameter))
        return std::nullopt;

    // Update parameter and intersection as necessary
    ray.NewMaxParameter(parameter);

    return Intersection{ ray.PointAtParameter(parameter), barycentric, m_faceNormal, InterpolateNormalAt(barycentric) };
}

auto 
Triangle::InterpolateNormalAt(const Point2f& uv) const -> Normal3f
{
    // for flat shading simply return the face normal
    const auto normal = m_vertexNormals[0] * (1 - uv.x - uv.y)
                        + m_vertexNormals[1] * uv.x
                        + m_vertexNormals[2] * uv.y;

    return Normalize(normal);
}

auto
Triangle::TransformBy(const Transform& transform) -> void
{
    for (auto& vertex : m_vertices)
        transform(vertex);

    for (auto& normal : m_vertexNormals)
        transform(normal);

    // precompute again
    UpdateEdges();
}

auto 
Triangle::GetArea() const -> float
{
    return 0.5f * Cross(m_edges[0], m_edges[1]).Length();
}

auto
Triangle::GetVertices() const -> const std::array<Point3f, 3>&
{
    return m_vertices;
}

auto
Triangle::GetPointFromUV(const Point2f& uv) const -> Point3f {
    // https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/barycentric-coordinates
    // for explanation
    return uv[0] * m_vertices[0] + uv[1] * m_vertices[1] + (1 - uv[0] - uv[1]) * m_vertices[2];
    //return m_vertices[0] + uv.x * m_edges[0] + uv.y * m_edges[1];
}

auto
Triangle::SampleSurface(Sampler& sampler) const -> Intersection {
    //pdf = 1 / GetArea(); // TODO: figure out if this should be for triangle or whole mesh
    const Point2f uv { sampler.GetRandomReal(), sampler.GetRandomReal() };
    return Intersection{ GetPointFromUV(uv), uv, m_faceNormal, InterpolateNormalAt(uv) };
}


