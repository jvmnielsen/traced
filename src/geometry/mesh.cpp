
#include "mesh.hpp"
#include <functional>
#include <future>
#include "../acceleration/aabb.hpp"
#include <algorithm>
#include "../utility/utility.hpp"

using namespace tr;
using namespace gm; 

Mesh::Mesh(std::vector<Triangle> triangles, std::shared_ptr<Material> material, gm::Transform const& transform)
    : m_triangles(std::move(triangles)),
      m_material(std::move(material)) {

    m_surface_area = calculate_surface_area();
    transform_by(transform);

    if (m_triangles.size() > 100)
        generate_internal_aabbs();
}

Mesh::Mesh(std::vector<Triangle> triangles)
    : m_triangles(std::move(triangles)) {
}

auto Mesh::intersects_internal_aabbs(const Rayf& ray) const -> std::optional<Intersection> {
    
    std::optional<Intersection> isect;
    for (auto& aabb : m_internal_bounding) {
        // the last overridden isect will always be the closest (ray max_param shrinks every time)
        auto tmp = aabb.intersects(ray);
        if (tmp.has_value())
            isect = tmp;
    }
    if (isect.has_value()) {
        isect->set_mesh_and_material(this, m_material.get());
        return isect;
    }
    return std::nullopt;
}

auto Mesh::intersects_mesh_proper(const Rayf& ray) const -> std::optional<Intersection> {

    std::optional<Intersection> isect;
    for (auto& triangle : m_triangles) {
        // the last overridden isect will always be the closest (ray max_param shrinks every time)
        auto tmp = triangle.intersects(ray);
        if (tmp.has_value())
            isect = tmp;
    }
    if (isect.has_value()) {
        isect->set_mesh_and_material(this, m_material.get());
        return isect;
    }
    return std::nullopt;
}


auto Mesh::intersects(const Rayf& ray) const -> std::optional<Intersection> {
    if (!m_internal_bounding.empty())
        return intersects_internal_aabbs(ray);
    return intersects_mesh_proper(ray);
}



auto Mesh::transform_by(Transform const& transform) -> void {
    for (auto& triangle : m_triangles)
        triangle.transform_by(transform);
    m_surface_area = calculate_surface_area();
}

auto Mesh::calculate_bounds() const -> Bounds {
    auto min = Point3f(gm::constants::max_float);
    auto max = Point3f(gm::constants::min_float);
    for (auto const& triangle : m_triangles) {
        for (auto const& vertex : triangle.vertices()) {
            min = elementwise_min(min, vertex);
            max = elementwise_max(max, vertex);
        }
    }
    return {min, max};
}

auto Mesh::calculate_surface_area() const -> FLOAT {
    auto sum = 0.0;
    for (const auto& triangle : m_triangles)
        sum += triangle.calculate_surface_area();
    return sum;
}

auto Mesh::sample_random_triangle(Sampler& sampler) const -> std::tuple<Intersection, FLOAT> {
    const auto randTriangle = m_triangles[sampler.get_random_in_distribution(m_triangles.size())];
    return randTriangle.sample_surface(sampler);
}

auto Mesh::sample_as_light(
        Intersection const& ref,
        Sampler& sampler
    ) const -> std::tuple<Intersection, Vec3f, FLOAT, Color3f> {
    
    const auto[sampledIsect, pdfs] = sample_random_triangle(sampler);
    const auto wi = (sampledIsect.point() - ref.point()).normalise();
    FLOAT pdf = 0;
    const auto denom = std::abs(dot(-wi, sampledIsect.geometric_normal())) * calculate_surface_area();

    if (denom != 0.0)
        pdf = (sampledIsect.point() - ref.point()).length_squared() / denom;

    if (pdf == 0 || (sampledIsect.point() - ref.point()).length() == 0)
        return std::make_tuple(sampledIsect, static_cast<Vec3f>(wi), 0.0f, Color3f::black());

    return std::make_tuple(sampledIsect, static_cast<Vec3f>(wi), pdf, m_material->emitted(sampledIsect, -static_cast<Vec3f>(wi)));
}


auto Mesh::pdf(Intersection const& ref, Vec3f const& wi) const -> FLOAT {
    Ray ray = Rayf{ref.point(), wi};
    auto isect = intersects(ray);
    if (isect.has_value()) {
        const auto denom = std::abs(dot(isect->geometric_normal(), -wi)) * calculate_surface_area();
        if (denom == 0)
            return 0;
        const auto a =  (ref.point() - isect->point()).length_squared() / denom;
        return a;
    }
    return 0;
}

auto Mesh::generate_internal_bounding_boxes() const -> std::vector<Bounds> {
    
    const auto num_divisions = static_cast<int>(std::cbrt(triangle_count() / 10));
    std::cout << "Num divisions: " << num_divisions << '\n';

    const auto mesh_bounds = calculate_bounds();
    std::cout << "Total bounds: " << mesh_bounds[0].x << " " << mesh_bounds[0].y << " " << mesh_bounds[0].z << " , "
        <<  mesh_bounds[1].x << " " << mesh_bounds[1].y << " " << mesh_bounds[1].z << '\n';

    std::vector<Bounds> internal_bounds;
    internal_bounds.reserve(num_divisions);

    const auto axis_value_for_intervals = [&mesh_bounds, num_divisions] (int axis) {
        return std::abs(mesh_bounds[1][axis] - mesh_bounds[0][axis]) / num_divisions;
    };

    std::array<FLOAT, 3> length_intervals = {
        axis_value_for_intervals(0),
        axis_value_for_intervals(1),
        axis_value_for_intervals(2)
    };

    const auto generate_axis_value_for_bounds = [&mesh_bounds, &length_intervals] (std::size_t iteration, int axis) {
        return mesh_bounds[0][axis] + iteration * length_intervals[axis];
    };

    for (size_t i = 0; i < num_divisions; ++i) {
        for (size_t j = 0; j < num_divisions; ++j) {
            for (size_t k = 0; k < num_divisions; ++k) {

                auto generate_point = [i, j, k, &generate_axis_value_for_bounds](int offset) {
                    return Point3f{ generate_axis_value_for_bounds(k + offset, 0),
                                    generate_axis_value_for_bounds(j + offset, 1),
                                    generate_axis_value_for_bounds(i + offset, 2)};
                };

                internal_bounds.emplace_back(generate_point(0), generate_point(1));
            }
        }
    }
    return internal_bounds;  
}

auto Mesh::assign_triangles_to_internal_bounds(std::vector<Bounds> const& internal_bounds) const -> std::vector<AABB> {

    std::vector<std::future<std::optional<AABB>>> assigned_triangles;
    assigned_triangles.reserve(internal_bounds.size());
    for (auto const& bounds : internal_bounds) {
        assigned_triangles.emplace_back(
            std::async([&bounds, this]() {
                std::vector<Triangle> sub_mesh;
                std::copy_if(m_triangles.begin(), m_triangles.end(), std::back_inserter(sub_mesh),
                             [&bounds](const Triangle& triangle) { return bounds.overlaps(triangle.calculate_bounds()); });

                return sub_mesh.empty() ? std::nullopt : std::make_optional(AABB(std::make_unique<Mesh>(sub_mesh), bounds));
            }));
    }
    std::vector<AABB> aabbs;
    for (auto& potential_aabb : assigned_triangles)     {
        const auto aabb = potential_aabb.get();
        if (aabb) {
            aabbs.push_back(aabb.value());
        }
    }
    return aabbs;
}

auto Mesh::generate_internal_aabbs() -> void {
    Timer timer{std::string{"Generating internal bounds took: "}};
    std::cout << "Size of mesh: " << m_triangles.size() << '\n';
    m_internal_bounding = assign_triangles_to_internal_bounds(generate_internal_bounding_boxes());
//    std::cout << "Size of list: " << m_internal_bounding.size() << "\n";
}

auto Mesh::triangle_count() const -> std::size_t {
    return m_triangles.size();
}
