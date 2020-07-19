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
        : m_vertices(vertices)
        , m_vertex_normals(vertex_normals)
        , m_face_normal(vertex_normals[0])
        , m_uv(uv) {

    update_edges();
}

Triangle::Triangle(
    std::array<Point3f, 3>  vertices,
    std::array<Normal3f, 3> vertex_normals)
        : m_vertices(vertices)
        , m_vertex_normals(vertex_normals)
        , m_face_normal(vertex_normals[0]) {

    update_edges();
}


auto Triangle::update_edges() -> void {
    m_edges[0]    = m_vertices[1] - m_vertices[0];
    m_edges[1]    = m_vertices[2] - m_vertices[0];
    m_face_normal = cross(m_edges[0], m_edges[1]).normalise();
}

auto Triangle::intersects(Rayf const& ray) const -> std::optional<Intersection> {
    
    auto const dir = static_cast<Vec3f>(ray.direction());
    auto const p_vec = cross(dir, m_edges.at(1));
    auto const det = dot(m_edges[0], p_vec);

    const auto recipDet = 1 / det;

    auto barycentric = Point2f{};

    // barycentric coordinate u
    const auto t_vec = ray.origin() - m_vertices[0];
    barycentric.x = dot(t_vec, p_vec) * recipDet;

    if (barycentric.x < 0 || barycentric.x > 1)
        return std::nullopt;

    // barycentric coordinate v
    auto const q_vec = cross(t_vec, m_edges.at(0));
    barycentric.y = dot(dir, q_vec) * recipDet;

    if (barycentric.y < 0 || barycentric.y + barycentric.x > 1)
        return std::nullopt;

    const auto parameter = dot(m_edges.at(1), q_vec) * recipDet;

    if (!ray.within_bounds(parameter))
        return std::nullopt;

    // Update parameter and intersection as necessary
    ray.update_max_parameter(parameter);
    return Intersection{
        ray.point_at_parameter(parameter),
        barycentric, m_face_normal,
        interpolate_normal_at(barycentric)
    };
}

auto Triangle::interpolate_normal_at(Point2f const& uv) const -> Normal3f {

    const auto normal =   static_cast<Vec3f>(m_vertex_normals[0]) * (1 - uv.x - uv.y)
                        + static_cast<Vec3f>(m_vertex_normals[1]) * uv.x
                        + static_cast<Vec3f>(m_vertex_normals[2]) * uv.y;

    return normal.normalise();
}

auto Triangle::transform_by(Transform const& transform) -> void {
    for (auto& vertex : m_vertices)
        vertex = transform.apply(vertex);

    for (auto& normal : m_vertex_normals)
        normal = transform.apply(normal);

    // precompute again
    update_edges();
}

auto Triangle::calculate_surface_area() const -> FLOAT {
    return 0.5 * cross(m_edges[0], m_edges[1]).length();
}

auto Triangle::vertices() const -> std::array<Point3f, 3> const& {
    return m_vertices;
}

auto Triangle::point_from_uv(const Point2f& uv) const -> Point3f {
    // https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/barycentric-coordinates
    // for explanation
    return uv[0] * m_vertices[0] + uv[1] * m_vertices[1] + (1 - uv[0] - uv[1]) * m_vertices[2];
}

auto Triangle::sample_surface(Sampler& sampler) const -> std::tuple<Intersection, FLOAT> {
    const auto uv = sampler.uniform_sample_triangle();
    return std::make_tuple(
        Intersection{
            point_from_uv(uv), uv,
            m_face_normal,
            interpolate_normal_at(uv)
        },
        1 / calculate_surface_area()
    );
}

auto Triangle::calculate_bounds() const -> Bounds {
    
    auto min = Point3f(gm::constants::max_float); // min{Math::Constants::MaxFloat};
    auto max = Point3f(gm::constants::min_float); // max{Math::Constants::MinFloat};

    for (auto const& vertex : m_vertices) {
        min = elementwise_min(min, vertex);
        max = elementwise_max(max, vertex);
    }

    return {min, max};
}
