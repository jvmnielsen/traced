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
    explicit AABB(std::array<Point3f, 2> bounds);
    AABB(Point3f lowerBound, Point3f upperBound);
    explicit AABB(std::unique_ptr<Mesh> mesh);

    //~AABB() = default;

    bool Intersects(const Rayf& ray, Intersection& isect) const;
    bool IntersectsQuick(const Rayf& ray) const;

    auto Diagonal() const -> Vec3f;

    auto SurfaceArea() const -> float;

    auto GetBounds() const -> const std::array<Point3f, 2>&;

    void SetShape(std::unique_ptr<Mesh> shape);
    Mesh& GetShape() const;

private:
    bool IntersectsShape(const Rayf& ray, Intersection& isect) const;

    std::array<Point3f, 2> m_bounds;
    std::unique_ptr<Mesh> m_mesh;
};

auto
Union(const AABB& b1, const AABB& b2) -> AABB
{
    return AABB{ Point3f{std::max(b1.GetBounds()[0].x, b2.GetBounds()[0].x),
                 std::max(b1.GetBounds()[0].y, b2.GetBounds()[0].y),
                 std::max(b1.GetBounds()[0].z, b2.GetBounds()[0].z)},
                 Point3f{ std::max(b1.GetBounds()[1].x, b2.GetBounds()[1].x),
                 std::max(b1.GetBounds()[1].y, b2.GetBounds()[1].y),
                 std::max(b1.GetBounds()[1].z, b2.GetBounds()[1].x)}};
}

class BVH
{
public:
    BVH(std::vector<std::unique_ptr<Mesh>> shapes);

    ~BVH();


private:

    struct BucketInfo
    {
        int count = 0;
        AABB bv;
    };

    std::vector<std::unique_ptr<Mesh>> m_shapes;

    constexpr static int nBuckets = 12;

    std::array<BucketInfo, nBuckets> m_buckets;

};

