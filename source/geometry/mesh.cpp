
#include "mesh.hpp"
#include <functional>
#include "../acceleration/aabb.hpp"

Mesh::Mesh(std::vector<Triangle> triangles, std::shared_ptr<Material> material)
    : m_triangles(std::move(triangles)),
      m_material(std::move(material))
{
    m_surface_area = GetSurfaceArea();
}




auto 
Mesh::Intersects(const Rayf& ray) const -> std::optional<Intersection> {

    std::optional<Intersection> isect;


    for (auto& triangle : m_triangles) {
        // the last overridden isect will always be the closest (ray max_param shrinks every time)
        auto tmp = triangle.Intersects(ray);
        if (tmp.has_value())
            isect = tmp;
    }

    if (isect.has_value()) {
        isect->SetMeshAndMaterial(this, m_material.get());
        return isect;
    }

    return std::nullopt;//.has_value() ? std::optional<Intersection>{isect.value()} : std::nullopt;
}


bool Mesh::IntersectsFast(const Rayf& ray) const {
    for (const auto& triangle : m_triangles)
        if (triangle.IntersectsFast(ray))
            return true;

    return false;
}


void Mesh::TransformBy(std::shared_ptr<Transform> transform) {

    m_transform_to_world = std::move(transform);

    for (auto& triangle : m_triangles)
        triangle.TransformBy(*m_transform_to_world);
}

/*
std::unique_ptr<Mesh> Mesh::Clone()
{
    auto newMesh = std::make_unique<Mesh>();

    std::vector<Triangle> triangles;

    for (const auto& polygon : m_triangles)
    {
        auto copyTriangle = m_triangles;



        newMesh->AddPolygon(std::move(newPolygon));
    }

    return newMesh;
}
 */


auto 
Mesh::calculate_bounds() const -> Bounds {

    Point3f min{ Math::Constants::MaxFloat };
    Point3f max{ Math::Constants::MinFloat};

    for (const auto& triangle : m_triangles) {
        for (const auto& vertex : triangle.GetVertices()) {
            min = elementwise_min(min, vertex);
            max = elementwise_max(max, vertex);
        }
    }
   
    return {min, max};
}

auto
Mesh::GetSurfaceArea() const -> FLOAT {
    auto sum = 0.0;
    for (const auto& triangle : m_triangles)
        sum += triangle.GetArea();

    return sum;
}

auto
Mesh::SampleRandomTriangle(Sampler& sampler) const -> std::tuple<Intersection, FLOAT> {
    const auto randTriangle = m_triangles[sampler.GetRandomInDistribution(m_triangles.size())];
    return randTriangle.SampleSurface(sampler);
}

auto
Mesh::Sample(const Intersection& ref, Sampler& sampler) const -> std::tuple<Intersection, FLOAT> {
    auto [sampledIsect, pdf] = SampleRandomTriangle(sampler);
    const auto wi = Normal3f(sampledIsect.GetPoint() - ref.GetPoint());

    sampledIsect.SetMeshAndMaterial(this, m_material.get());

    const auto denom = std::abs(dot(sampledIsect.GetGeometricNormal(), -wi)) * GetSurfaceArea();

    if (denom != 0.0)
        pdf *= (sampledIsect.GetPoint() - ref.GetPoint()).length_squared() / denom;

    return std::make_tuple(sampledIsect, pdf);

}

auto 
Mesh::SampleAsLight(const Intersection& ref, Sampler& sampler) const -> std::tuple<Intersection, Normal3f, FLOAT, Color3f>
{
    const auto [sampledIsect, pdf] = Sample(ref, sampler);

    const auto wi = Normal3f(sampledIsect.GetPoint() - ref.GetPoint());

    if (pdf == 0)
        return std::make_tuple(sampledIsect, wi, pdf, Color3f::Black());

    return std::make_tuple(sampledIsect, wi, pdf, m_material->Emitted(sampledIsect, -wi));
}


auto
Mesh::Pdf(const Intersection& ref, const Normal3f& wi) const -> FLOAT {
    
    Ray ray = Rayf{ref.GetPoint(), wi};
    auto isect = Intersects(ray);
    if (isect.has_value()) {

        const auto denom = std::abs(dot(isect->GetGeometricNormal(), -wi)) * GetSurfaceArea();

        if (denom == 0)
            return 0;

        const auto a =  (ref.GetPoint() - isect->GetPoint()).length_squared() / denom;
        return a;
    }
    return 0;
}

auto 
Mesh::generate_internal_bounding_boxes() const -> std::vector<Bounds> {
    
    const auto num_divisions = static_cast<int>(std::sqrt(triangle_count()));

    const auto mesh_bounds = calculate_bounds();
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
                    return Point3f{ generate_axis_value_for_bounds(i + offset, 0),
                                    generate_axis_value_for_bounds(j + offset, 1),
                                    generate_axis_value_for_bounds(k + offset, 2)};
                };

                internal_bounds.emplace_back(generate_point(0), generate_point(1));
            }
        }
    }

    return internal_bounds;  
}

auto 
Mesh::assign_triangles_to_internal_bounds(const std::vector<Bounds>& internal_bounds) const -> std::vector<AABB> {
 
    std::vector<AABB> aabbs;

    for (const auto& bounds : internal_bounds) {
        std::vector<Triangle> sub_mesh;
        for (const auto& triangle : m_triangles) {
            const auto triangle_bounds = triangle.calculate_bounds();
            if (bounds.overlaps(triangle_bounds)) 
                sub_mesh.push_back(triangle);
        }
        if (!sub_mesh.empty()) 
            aabbs.emplace_back(std::make_unique<Mesh>(sub_mesh, m_material), bounds);
    }
    return aabbs;
}

auto Mesh::generate_internal_aabbs() const ->std::vector<AABB> {
    return assign_triangles_to_internal_bounds(generate_internal_bounding_boxes());
}

auto Mesh::triangle_count() const -> std::size_t {
    return m_triangles.size();
}
