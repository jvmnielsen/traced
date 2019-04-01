#pragma once
#include <memory>
#include <vector>
#include "triangle.hpp"
#include <optional>
//#include "../acceleration/bvh.hpp"


class Transform;
//class AABB;

class Mesh
{
public:
    Mesh(std::vector<Triangle> triangles, std::shared_ptr<Material> material, const Transform& transform);
	Mesh(std::vector<Triangle> triangles);

    auto Intersects(const Rayf& ray) const -> std::optional<Intersection>;
    auto IntersectsFast(const Rayf& ray) const -> bool;



	auto calculate_surface_area() const -> FLOAT;
    auto calculate_bounds() const -> Bounds;
    auto transform_by(const Transform& transform) -> void;

    //auto ApplyMaterial(std::shared_ptr<Material> material) -> void;
    //auto GetMaterial() const -> const Material&;

    auto sample_random_triangle(Sampler& sampler) const -> std::tuple<Intersection, FLOAT>;

    auto sample_as_light(const Intersection& ref, Sampler& sampler) const -> std::tuple<Intersection, Vec3f, FLOAT, Color3f>;
    //auto sample(const Intersection& ref, Sampler& sampler) const -> std::tuple<Intersection, FLOAT>;

    //auto GetPreTransformedPoint(const Point3f& p) const -> Point3f;
	//auto GetTransform() const -> const Transform& { return *m_transformToWorld; }


    auto triangle_count() const -> std::size_t;

    auto pdf(const Intersection& ref, const Vec3f& wi) const -> FLOAT;

    //auto HasInternalBoundingBoxes() const -> bool { return !m_internalBoundingBoxes.empty(); }



private:



    auto intersects_internal_aabbs(const Rayf& ray) const -> std::optional<Intersection>;
    auto intersects_mesh_proper(const Rayf& ray) const -> std::optional<Intersection>;

	auto generate_internal_aabbs() -> void;
	auto generate_internal_bounding_boxes() const -> std::vector<Bounds>;
	auto assign_triangles_to_internal_bounds(const std::vector<Bounds>& internal_bounds) const -> std::vector<AABB>;

    std::vector<AABB>           m_internal_bounding;
    std::vector<Triangle>       m_triangles;
    std::shared_ptr<Material>   m_material;
    FLOAT                       m_surface_area;
    // std::shared_ptr<Transform>  m_transform_to_world;

};

class SubMesh : public Mesh
{
   
};


