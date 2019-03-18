#pragma once
#include <memory>
#include <vector>
#include <random>
#include "triangle.hpp"
#include <optional>
#include "../acceleration/aabb.hpp"

class Transform;
//class AABB;

class Mesh
{
public:
    explicit Mesh(std::vector<Triangle> triangle);

    /*
    Mesh(const Mesh& other) 
    : m_triangles(other.m_triangles), 
      m_material(other.m_material), 
      m_surfaceArea(other.m_surfaceArea), 
      m_transformToWorld(std::make_unique<Transform>(*other.m_transformToWorld)),
      //m_internalBoundingBoxes(other.m_internalBoundingBoxes)
    {
    } */

    auto Intersects(const Rayf& ray) const -> std::optional<Intersection>;
    auto IntersectsFast(const Rayf& ray) const -> bool;

	auto GetSurfaceArea() const -> float;
    auto GetExtent() const -> std::array<Point3f, 2>;
    auto TransformBy(std::unique_ptr<Transform> transform) -> void;

    auto ApplyMaterial(std::shared_ptr<Material> material) -> void;
    auto GetMaterial() const -> const Material&;

    auto SampleRandomTriangle(Sampler& sampler) const -> std::tuple<Intersection, FLOAT>;

    auto SampleAsLight(const Intersection& ref, Sampler& sampler) const -> std::tuple<Intersection, Normal3f, FLOAT, Color3f>;
    auto Sample(const Intersection& ref, Sampler& sampler) const -> std::tuple<Intersection, FLOAT>;

    auto GetPreTransformedPoint(const Point3f& p) const -> Point3f;
	//auto GetTransform() const -> const Transform& { return *m_transformToWorld; }


    auto Pdf(const Intersection& ref, const Normal3f& wi) const -> FLOAT;

    //auto HasInternalBoundingBoxes() const -> bool { return !m_internalBoundingBoxes.empty(); }
    auto GetInternalBoundingBoxes() const -> const std::vector<AABB>&; 

    auto IsHollow() const -> bool;

    auto GenerateInternalBoundingBoxes(std::size_t numDivisions) -> void;

private:

    //Mesh(std::vector<Triangle> triangle, bool isInternal);

    
    auto SetTrianglesInsideBounds(AABB& bounds) -> void; 

    std::vector<AABB> m_internalBoundingBoxes;

    std::vector<Triangle>       m_triangles;
    std::shared_ptr<Material>   m_material;
    FLOAT                       m_surfaceArea;
    std::unique_ptr<Transform>  m_transformToWorld;

    static constexpr int lowerBoundForTriangleCount = 10;

};

