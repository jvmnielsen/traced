#pragma once
#include <memory>
#include <vector>
#include <random>
#include "triangle.hpp"
#include <optional>

class Transform;

class Mesh
{
public:
    explicit Mesh(std::vector<Triangle> triangle);

    Mesh(const Mesh& other) 
    : m_triangles(other.m_triangles), 
      m_material(other.m_material), 
      m_surfaceArea(other.m_surfaceArea), 
      m_transformToWorld(std::make_unique<Transform>(*other.m_transformToWorld)) {} 

    auto Intersects(const Rayf& ray) const -> std::optional<Intersection>;
    auto IntersectsFast(const Rayf& ray) const -> bool;

	auto GetSurfaceArea() const -> float;
    auto GetExtent() const -> std::array<Point3f, 2>;
    auto TransformBy(std::unique_ptr<Transform> transform) -> void;

    auto ApplyMaterial(std::shared_ptr<Material> material) -> void;
    auto GetMaterial() const -> const Material&;

    auto SampleSurface(Sampler& sampler) const -> Intersection;
    auto SampleAsLight(const Intersection& ref, Sampler& sampler) const->std::tuple<Intersection, Vec3f, double, Color3f>;


    auto GetPreTransformedPoint(const Point3f& p) const -> Point3f;
	auto GetTransform() const -> const Transform& { return *m_transformToWorld; }


    auto Pdf(const Intersection& ref, const Vec3f& wi) const -> float;

private:
    std::vector<Triangle>       m_triangles;
    std::shared_ptr<Material>   m_material;
    float                       m_surfaceArea;
    std::unique_ptr<Transform>  m_transformToWorld;

};

