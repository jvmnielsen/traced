#include "Triangle.h"
#include <cmath>


void Triangle::UpdateEdges()
{
	m_edge0 = m_vertices[1] - m_vertices[0];
	m_edge0_2 = m_vertices[2] - m_vertices[0];
	m_edge1 = m_vertices[2] - m_vertices[1];
	m_edge2 = m_vertices[0] - m_vertices[2];
	m_normal = (m_vertices[1] - m_vertices[0]).CrossProduct(m_vertices[2] - m_vertices[0]).Normalize();
}

bool Triangle::Intersects(const Rayf& ray, Intersection& intersection)
{
    const auto p_vec = ray.Direction().CrossProduct(m_edge0_2);
    const auto det = m_edge0.DotProduct(p_vec);

    // back-face culling
    if (fabs(det < m_epsilon) && m_is_single_sided)
        return false;
   
    // precompute for performance
    const auto inverted_det = 1 / det;

    Vec2f barycentric{0};

    // barycentric coordinate u
    const auto t_vec = ray.Origin() - m_vertices[0];
    barycentric.x = t_vec.DotProduct(p_vec) * inverted_det;
    if (barycentric.x < 0 || barycentric.x > 1)
        return false;

    // barycentric coordinate v
    const auto q_vec = t_vec.CrossProduct(m_edge0);
    barycentric.y = ray.Direction().DotProduct( q_vec ) * inverted_det;
    if (barycentric.y < 0 || barycentric.y + barycentric.x > 1)
        return false;

    const auto parameter = m_edge0_2.DotProduct(q_vec) * inverted_det;

    intersection.SetBarycentric(barycentric);
    intersection.SetParameter(parameter);

    return true;
} 

void Triangle::CalculateNormal(Intersection &intersec) const
{
    // for flat shading simply return the face normal
    const auto normal = m_vertex_normals[0] * (1 - intersec.Barycentric().x - intersec.Barycentric().y)
        + m_vertex_normals[1] * intersec.Barycentric().x
        + m_vertex_normals[2] * intersec.Barycentric().y;

    intersec.SetNormal(Normalize(normal));
}

void Triangle::TransformBy(const Transform& transform)
{

    for (auto& vertex : m_vertices)
        transform(vertex);

    for (auto& normal : m_vertex_normals)
        transform(normal);

    // precompute again
    UpdateEdges();

}