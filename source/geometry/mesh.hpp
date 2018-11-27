#pragma once
#include <memory>
#include <vector>
#include <random>


#include "triangle.hpp"
#include "../core/intersection.hpp"
#include <optional>

class Transform;

class Mesh
{
public:
    //Mesh();
    explicit Mesh(std::vector<Triangle> triangle);
    //Mesh(const Mesh& other);
    //Mesh(Mesh&& other) noexcept;
    //auto operator=(const Mesh& other) -> Mesh&;

    auto Intersects(const Rayf& ray) -> std::optional<Intersection>;
	auto Intersects(const Rayf& ray, Intersection& isect) -> bool;
    auto IntersectsFast(const Rayf& ray) const -> bool;

	auto GetSurfaceArea() const -> float;
    auto GetExtent() const -> std::array<Point3f, 2>;
    auto TransformBy(const Transform& transform) -> void;

    auto ApplyMaterial(std::shared_ptr<Material> material) -> void;
    auto GetMaterial() const -> const Material&;

    auto SampleSurface(float& pdf) -> Intersection;

private:

    auto GetRandomTriangleIndex() -> int;

    std::vector<Triangle>       m_triangles;
    std::shared_ptr<Material>   m_material;

    std::mt19937 m_gen;

};

