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

    const auto transformNormals = object_to_world.inverse(); // normals are transformed by the inverse matrix

    for (auto& normal : m_vertex_normals)
    {
        normal = transformNormals.multiply_with_dir(normal);
    }

    UpdateEdges(); // precompute edges again
}

void Polygon::TransformByMatrix2(const Matrix44f &object_to_world)
{
    for (auto& vertex : m_vertices)
    {
        vertex = object_to_world.multiply_with_point(vertex);
    }

    UpdateEdges(); // precompute edges again
}

bool Polygon::Intersects(const Rayf& ray, Intersection& intersection)
{
    const auto p_vec = ray.direction().CrossProduct(m_edge0_2);
    const auto det = m_edge0.DotProduct(p_vec);

    // back-face culling
    if (fabs(det < m_epsilon) && m_is_single_sided)
        return false;
   
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

    intersection.SetBarycentric(barycentric);
    intersection.SetParameter(parameter);

    return true;
} 

void Polygon::CalculateNormal(Intersection &hit_data) const
{
    // for flat shading simply return the face normal
    const auto normal = (1 - hit_data.Barycentric().x - hit_data.Barycentric().y) * m_vertex_normals[0]
        + hit_data.Barycentric().x * m_vertex_normals[1]
        + hit_data.Barycentric().y * m_vertex_normals[2];

    hit_data.SetNormal(Normalize(normal));
}


void Polygon::TranslateBy(const Vecf& dir) 
{
    const Matrix44f translation =
    {    1,      0,     0, 0,
          0,     1,     0, 0,
          0,     0,     1, 0,
      dir.x, dir.y, dir.z, 1 };

    TransformByMatrix2(translation);
}

void Polygon::RotateAroundX(float angle) 
{
    const auto rad = Math::DegreeToRadian(angle);
    const Matrix44f rotation =
    {   1,        0,         0, 0,
        0, cos(rad), -sin(rad), 0,
        0, sin(rad),  cos(rad), 0,
        0,        0,         0, 1};
    
    TransformByMatrix(rotation);
}

void Polygon::RotateAroundY(float angle) 
{
    const auto rad = Math::DegreeToRadian(angle);
    const Matrix44f rotation =
    { cos(rad), 0, sin(rad), 0,
             0, 1,        0, 0,
     -sin(rad), 0, cos(rad), 0,
             0, 0,        0, 1};

    TransformByMatrix(rotation);
}

void Polygon::RotateAroundZ(float angle) 
{
    const auto rad = Math::DegreeToRadian(angle);
    const Matrix44f rotation =
    { cos(rad), -sin(rad), 0, 0,
      sin(rad),  cos(rad), 0, 0,
             0,         0, 1, 0,
             0,         0, 0, 1};

    TransformByMatrix(rotation);
}

void Polygon::ScaleBy(float factor)
{
    
    const Matrix44f scale =
    {factor,      0,      0, 0,
          0, factor,      0, 0,
          0,      0, factor, 0,
          0,      0,      0, 1};

    TransformByMatrix2(scale);
}
