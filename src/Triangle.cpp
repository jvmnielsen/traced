#include "Triangle.h"
#include <cmath>
#include "BoundingVolume.h"


Triangle::Triangle(
    std::array<Point3f, 3> vertices,
    std::array<Normal3f, 3> vertexNormals,
    bool isSingleSided,
    std::shared_ptr<Material> material)
    : Shape(material)
    , m_vertices(std::move(vertices))
    , m_vertexNormals(std::move(vertexNormals))
    , m_isSingleSided(isSingleSided)
{
    m_edges.at(0) = m_vertices.at(1) - m_vertices.at(0);
    m_edges.at(1) = m_vertices.at(2) - m_vertices.at(0);
}

void Triangle::UpdateEdges()
{
	m_edges.at(0) = m_vertices.at(1) - m_vertices.at(0);
    m_edges.at(1) = m_vertices.at(2) - m_vertices.at(0);
	m_faceNormal = m_edges.at(0).CrossProduct(m_edges.at(1)).Normalize();
}

bool Triangle::Intersects(const Rayf& ray, Intersection& isect)
{
    const auto p_vec = ray.GetDirection().CrossProduct(m_edges.at(1));
    const auto det = m_edges.at(0).DotProduct(p_vec);

    // back-face culling
    if (fabs(det < m_epsilon) && m_isSingleSided)
        return false;
   
    // precompute for performance
    const auto inverted_det = 1 / det;

    Point2f barycentric{0};

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

    if (!ray.ParameterWithinBounds(parameter))
        return false;


    // Update parameter and intersection as necessary
    ray.NewMaxParameter(parameter);
    isect.Update(ray.PointAtParameter(parameter), barycentric, m_faceNormal, this, m_material.get());
   
    return true;
}

bool Triangle::IntersectsQuick(const Rayf& ray) const
{
    const auto p_vec = ray.GetDirection().CrossProduct(m_edges.at(1));
    const auto det = m_edges.at(0).DotProduct(p_vec);

    // back-face culling
    if (fabs(det < m_epsilon) && m_isSingleSided)
        return false;

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

Normal3f Triangle::CalculateShadingNormal(const Intersection& isect) const
{
    // for flat shading simply return the face normal
    const auto normal = m_vertexNormals[0] * (1 - isect.GetUV().x - isect.GetUV().y)
        + m_vertexNormals[1] * isect.GetUV().x
        + m_vertexNormals[2] * isect.GetUV().y;

    return Normalize(normal);
}

void Triangle::TransformBy(const Transform& transform)
{
    for (auto& vertex : m_vertices)
        transform(vertex);

    for (auto& normal : m_vertexNormals)
        transform(normal);

    // precompute again
    UpdateEdges();
}

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
Triangle::GetVertices() const -> const std::array<Point3f, 3>&
{
    return m_vertices;
}

auto
Triangle::GetBoundingVolume() const -> std::unique_ptr<BoundingVolume>
{
    return nullptr;
}