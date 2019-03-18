#pragma once
#include <memory>
#include <vector>
#include "triangle.hpp"
#include <optional>
#include "../acceleration/aabb.hpp"


class Transform;
//class AABB;

class Mesh
{
public:
    explicit Mesh(std::vector<Triangle> triangles, std::shared_ptr<Material> material);

    auto Intersects(const Rayf& ray) const -> std::optional<Intersection>;
    auto IntersectsFast(const Rayf& ray) const -> bool;

	auto GetSurfaceArea() const -> float;
    auto calculate_bounds() const -> Bounds;
    auto TransformBy(std::shared_ptr<Transform> transform) -> void;

    //auto ApplyMaterial(std::shared_ptr<Material> material) -> void;
    auto GetMaterial() const -> const Material&;

    auto SampleRandomTriangle(Sampler& sampler) const -> std::tuple<Intersection, FLOAT>;

    auto SampleAsLight(const Intersection& ref, Sampler& sampler) const -> std::tuple<Intersection, Normal3f, FLOAT, Color3f>;
    auto Sample(const Intersection& ref, Sampler& sampler) const -> std::tuple<Intersection, FLOAT>;

    //auto GetPreTransformedPoint(const Point3f& p) const -> Point3f;
	//auto GetTransform() const -> const Transform& { return *m_transformToWorld; }


    auto triangle_count() const -> std::size_t { return m_triangles.size(); }

    auto Pdf(const Intersection& ref, const Normal3f& wi) const -> FLOAT;

    //auto HasInternalBoundingBoxes() const -> bool { return !m_internalBoundingBoxes.empty(); }

    auto generate_internal_aabbs() const -> std::vector<AABB>;

    auto generate_internal_bounding_boxes() const -> std::vector<Bounds>;
    auto assign_triangles_to_internal_bounds(const std::vector<Bounds>& internal_bounds) const -> std::vector<AABB>;

private:

    std::vector<Triangle>       m_triangles;
    std::shared_ptr<Material>   m_material;
    FLOAT                       m_surface_area;
    std::shared_ptr<Transform>  m_transform_to_world;

};

