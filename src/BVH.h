#pragma once
#include <vector>
#include <memory>
//#include "src/Shape.h"
//#include "src/Mesh.h"
//#include "AABB.h"
#include "Mesh.h"


class AABB
{
public:
    AABB() { } // careful
    explicit AABB(std::array<Point3f, 2> bounds);
    AABB(Point3f lowerBound, Point3f upperBound);
    explicit AABB(std::unique_ptr<Mesh> mesh);

    //~AABB() = default;

    bool Intersects(const Rayf& ray, Intersection& isect) const;
    bool IntersectsQuick(const Rayf& ray) const;

    auto Diagonal() const -> Vec3f;

    auto SurfaceArea() const -> float;

    auto Center() const -> const Point3f&;

    auto GetBounds() const -> const std::array<Point3f, 2>&;
    auto LowerBound() const -> const Point3f&;
    auto UpperBound() const -> const Point3f&;
    auto MaximumExtent() const -> int;
    auto CalculateCenter() const -> Point3f;

    void SetShape(std::unique_ptr<Mesh> shape);
    Mesh& GetShape() const;

private:
    bool IntersectsShape(const Rayf& ray, Intersection& isect) const;

    std::array<Point3f, 2> m_bounds;
    Point3f m_center;

    std::unique_ptr<Mesh> m_mesh;
};

inline auto
Union(const AABB& b1, const AABB& b2) -> AABB
{
    return AABB{ Point3f{ std::min(b1.LowerBound().x, b2.LowerBound().x),
                          std::min(b1.LowerBound().y, b2.LowerBound().y),
                          std::min(b1.LowerBound().z, b2.LowerBound().z)},
                 Point3f{ std::max(b1.UpperBound().x, b2.UpperBound().x),
                          std::max(b1.UpperBound().y, b2.UpperBound().y),
                          std::max(b1.UpperBound().z, b2.UpperBound().z)}};
}

inline auto
Union(const AABB& b, const Point3f& p) -> AABB
{
    return AABB{ Point3f{ std::min(b.LowerBound().x, p.x),
                          std::min(b.LowerBound().y, p.y),
                          std::min(b.LowerBound().z, p.z)},
                 Point3f{ std::max(b.UpperBound().x, p.x),
                          std::max(b.UpperBound().y, p.y),
                          std::max(b.UpperBound().z, p.z)}};
}

class BVH
{
public:
    BVH(std::vector<std::unique_ptr<Mesh>> shapes);

    ~BVH();

    auto BuildTree() -> void;
private:

    auto AxisOfMaximumExtent() const-> int;

    std::vector<std::unique_ptr<Mesh>> m_shapes;
    std::vector<AABB> m_boundingBoxes;

    auto GenerateAABBList() -> void;
};

