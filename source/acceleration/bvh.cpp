#include "bvh.hpp"



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


BVH::BVH(std::vector<std::unique_ptr<Mesh>> meshes)
{
    for (auto& shape : meshes)
    {
        m_AABBs.emplace_back(std::move(shape));
    }

    BuildTree(0, m_AABBs.size());
}


auto 
BVH::AxisOfMaximumExtent() const -> int
{
    AABB globalBounds;
    //for (const auto& bounds : m_boundingBoxes)
      //  globalBounds = Union(globalBounds, bounds);

    return globalBounds.MaximumExtent();
}

auto 
BVH::BuildTree(int start, int end) -> BVHNode*
{
    // Node to return
    BVHNode* node;

    // Total bounds for current level
    AABB totalBounds;
    for (int i = start; i < end; ++i)
        totalBounds = Union(totalBounds, m_AABBs[i]);

    int numShapes = end - start;

    if (numShapes == 1)
    {
        // Create leaf node
        node->InitLeaf(0, 1, totalBounds);
    }
    else
    {
        AABB centerBounds;
        for (int i = start; i < end; ++i)
        {
            centerBounds = Union(centerBounds, m_AABBs[i].CalculateCenter());
        }
        auto axis = centerBounds.MaximumExtent();
        auto mid = (end + start) / 2;


        std::nth_element(
                &m_AABBs[start],
                &m_AABBs[mid],
                &m_AABBs[end],
                [axis](const AABB& a, const AABB& b)
                {
                    return a.CalculateCenter()[axis] < b.CalculateCenter()[axis];
                });


        node->InitInner(axis,
                       BuildTree(start, mid),
                       BuildTree(mid, end));
    }

    return node;
}