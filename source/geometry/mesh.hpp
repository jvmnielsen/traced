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

    auto Intersects(const Rayf& ray) const -> std::optional<Intersection>;
    auto IntersectsFast(const Rayf& ray) const -> bool;

	auto GetSurfaceArea() const -> float;
    auto GetExtent() const -> std::array<Point3f, 2>;
    auto TransformBy(const Transform& transform) -> void;

    auto ApplyMaterial(std::shared_ptr<Material> material) -> void;
    auto GetMaterial() const -> const Material&;

    auto SampleSurface(float& pdf, Sampler& sampler) const -> Intersection;

    auto Pdf(const Point3f& ref, const Vec3f& wi) const -> float;

private:
    std::vector<Triangle>       m_triangles;
    std::shared_ptr<Material>   m_material;
    float                       m_surfaceArea;

};

