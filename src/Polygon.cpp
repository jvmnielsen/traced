#include "Polygon.h"
#include <cmath>

void Polygon::UpdateEdges()
{
	m_edge0 = m_vertices[1] - m_vertices[0];
	m_edge0_2 = m_vertices[2] - m_vertices[0];
	m_edge1 = m_vertices[2] - m_vertices[1];
	m_edge2 = m_vertices[0] - m_vertices[2];
	m_normal = CrossProduct(m_vertices[1] - m_vertices[0], m_vertices[2] - m_vertices[0]).Normalize(); 
}

void Polygon::TransformByMatrix(const Matrix44f &object_to_world)
{
	for(auto& vertex : m_vertices)
	{
        vertex = object_to_world.multiply_with_point(vertex);
	}
    UpdateEdges();
}

bool Polygon::Intersects(const Rayf &ray, Intersection &hit_data)
{
    const auto p_vec = ray.direction().CrossProduct(m_edge0_2);
    const auto det = m_edge0.DotProduct(p_vec);

    // back-face culling
    if (m_is_single_sided)
    {
        if (det < m_epsilon)
            return false;
    }
    else
    {
        if (fabs(det) < m_epsilon)
            return false;
    }

    // precompute for performance
    const auto inverted_det = 1 / det;

    Vecf barycentric(0);

    // barycentric coordinate u
    const auto t_vec = ray.origin() - m_vertices[0];
    barycentric.x = t_vec.DotProduct(p_vec) * inverted_det;
    if (barycentric.x < 0 || barycentric.x > 1)
        return false;

    // barycentric coordinate v
    const auto q_vec = t_vec.CrossProduct(m_edge0);
    barycentric.y = ray.direction().DotProduct( q_vec ) * inverted_det;
    if (barycentric.y < 0 || barycentric.y + barycentric.x > 1)
        return false;

    const auto parameter = m_edge0_2.DotProduct(q_vec) * inverted_det;

    hit_data.SetBarycentric(barycentric);
    hit_data.SetParameter(parameter);

    return true;
}

void Polygon::CalculateNormal(Intersection &hit_data) const
{
    // for flat shading simply return the face normal
    const auto normal = (1 - hit_data.Barycentric().x - hit_data.Barycentric().y) * m_vertex_normals[0]
        + hit_data.Barycentric().x * m_vertex_normals[1]
        + hit_data.Barycentric().y * m_vertex_normals[2];

    hit_data.SetNormal(normal);
}
