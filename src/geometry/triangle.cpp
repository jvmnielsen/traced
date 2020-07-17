#include "triangle.hpp"
#include <cmath>
#include <numeric>
#include "../acceleration/bounds.hpp"

using namespace tr;
using namespace gm;

Triangle::Triangle(
    std::array<Point3f, 3>     vertices,
    std::array<Normal3f, 3>    vertex_normals,
    std::array<Point2f, 3>     uv)
        : m_vertices(std::move(vertices))
        , m_vertex_normals(std::move(vertex_normals))
        , m_face_normal(vertex_normals[0])
        , m_uv(std::move(uv))
{
    UpdateEdges();
}

Triangle::Triangle(
    std::array<Point3f, 3>  vertices,
    std::array<Normal3f, 3>    vertex_normals)
        : m_vertices(std::move(vertices))
        , m_vertex_normals(std::move(vertex_normals))
        , m_face_normal(vertex_normals[0])
{
    UpdateEdges();
}


auto 
Triangle::UpdateEdges() -> void
{
    m_edges[0]   = m_vertices[1] - m_vertices[0];
    m_edges[1]   = m_vertices[2] - m_vertices[0];
    m_face_normal = cross(m_edges[0], m_edges[1]).normalise();
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
Triangle::IntersectsFast(const Rayf& ray) const -> bool {

    const auto dir = static_cast<Vec3f>(ray.direction());
    const auto p_vec = cross(dir, m_edges[1]);
    const auto det = dot(m_edges[0], p_vec);

    // precompute for performance
    const auto inverted_det = 1 / det;

    Vec2f barycentric;

    // barycentric coordinate u
    const auto t_vec = ray.origin() - m_vertices[0];
    barycentric.x = dot(t_vec, p_vec) * inverted_det;
    if (barycentric.x < 0 || barycentric.x > 1)
        return false;

    // barycentric coordinate v
    const auto q_vec = cross(t_vec, m_edges[0]);
    barycentric.y = dot(dir, q_vec) * inverted_det;
    if (barycentric.y < 0 || barycentric.y + barycentric.x > 1)
        return false;

    const auto parameter = dot(m_edges[1], q_vec) * inverted_det;

    return parameter > 0 && parameter <= ray.max_parameter();
}

auto 
Triangle::Intersects(const Rayf& ray) const -> std::optional<Intersection> {
    
    const auto dir = static_cast<Vec3f>(ray.direction());
    const auto p_vec = cross(dir, m_edges.at(1));
    const auto det = dot(m_edges[0], p_vec);

    const auto recipDet = 1 / det;

    Point2f barycentric;

    // barycentric coordinate u
    const auto t_vec = ray.origin() - m_vertices[0];
    barycentric.x = dot(t_vec, p_vec) * recipDet;

    if (barycentric.x < 0 || barycentric.x > 1)
        return std::nullopt;

    // barycentric coordinate v
    const auto q_vec = cross(t_vec, m_edges.at(0));
    barycentric.y = dot(dir, q_vec) * recipDet;

    if (barycentric.y < 0 || barycentric.y + barycentric.x > 1)
        return std::nullopt;

    const auto parameter = dot(m_edges.at(1), q_vec) * recipDet;

    if (!ray.within_bounds(parameter))
        return std::nullopt;

    // Update parameter and intersection as necessary
    ray.update_max_parameter(parameter);

    //const auto shaded = InterpolateNormalAt(barycentric);

    return Intersection{ ray.point_at_parameter(parameter), barycentric, m_face_normal, InterpolateNormalAt(barycentric)}; //
}

auto Triangle::InterpolateNormalAt(Point2f const& uv) const -> Normal3f {

    const auto normal =   static_cast<Vec3f>(m_vertex_normals[0]) * (1 - uv.x - uv.y)
                        + static_cast<Vec3f>(m_vertex_normals[1]) * uv.x
                        + static_cast<Vec3f>(m_vertex_normals[2]) * uv.y;

    return normal.normalise();
}

auto Triangle::TransformBy(Transform const& transform) -> void
{
    for (auto& vertex : m_vertices)
        vertex = transform.apply(vertex);

    for (auto& normal : m_vertex_normals)
        normal = transform.apply(normal);

    // precompute again
    UpdateEdges();
}

auto 
Triangle::calculate_surface_area() const -> FLOAT {
    return 0.5 * cross(m_edges[0], m_edges[1]).length();
    //return area < 1 ? 1 : area;
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
Triangle::SampleSurface(Sampler& sampler) const -> std::tuple<Intersection, FLOAT> {
    //pdf = 1 / GetArea(); // TODO: figure out if this should be for triangle or whole mesh
    //const Point2f uv { sampler.GetRandomReal(), sampler.GetRandomReal() };
    const auto uv = sampler.uniform_sample_triangle();
    return std::make_tuple(Intersection{ GetPointFromUV(uv), uv, m_face_normal, InterpolateNormalAt(uv) }, 1 / calculate_surface_area());
}

auto 
Triangle::calculate_bounds() const -> Bounds {
    
    auto min = Point3f(gm::constants::max_float); // min{Math::Constants::MaxFloat};
    auto max = Point3f(gm::constants::min_float); // max{Math::Constants::MinFloat};

    for (const auto& vertex : m_vertices) {
        min = elementwise_min(min, vertex);
        max = elementwise_max(max, vertex);
    }

    return {min, max};
}
