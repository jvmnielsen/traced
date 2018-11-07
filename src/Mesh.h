#pragma once
#include <memory>
#include <vector>
#include <random>
#include "Triangle.h"

class Mesh 
{
public:
    Mesh();
    ~Mesh();
    explicit Mesh(std::shared_ptr<Material> material);
    Mesh(const Mesh& other);
    Mesh(Mesh&& other) noexcept;

	auto Intersects(const Rayf& ray, Intersection& isect) -> bool;
    auto IntersectsFast(const Rayf& ray) const -> bool;

	auto GetSurfaceArea() const -> float;
    auto GetExtent() const -> std::array<Point3f, 2>;
    auto TransformBy(const Transform& transform) -> void;

    std::vector<Triangle>       m_triangles;
    std::shared_ptr<Material>   m_material;

};

