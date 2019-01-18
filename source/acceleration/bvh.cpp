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

BVH::BVHNode::BVHNode(AABB aabb)
        : m_aabb(std::move(aabb))
        , m_leftChild(nullptr)
        , m_rightChild(nullptr) {
}

BVH::BVHNode::BVHNode(
            int axis,
            std::unique_ptr<BVHNode> leftChild,
            std::unique_ptr<BVHNode> rightChild)
        : m_aabb(Union(leftChild->m_aabb, rightChild->m_aabb))
        , m_splitAxis(axis)
        , m_leftChild(std::move(leftChild))
        , m_rightChild(std::move(rightChild)) {
}

BVH::BVH(std::vector<std::unique_ptr<Mesh>> meshes) {

    for (auto& shape : meshes) {
        m_AABBs.emplace_back(std::move(shape));
    }

    m_rootNode = BuildTree(0, static_cast<int>(m_AABBs.size()));
}

auto BVH::FlattenTree(std::unique_ptr<BVHNode> rootNode) -> void {

}

auto BVH::BuildTree(int start, int end) -> std::unique_ptr<BVHNode> {

    int numShapes = end - start;

    // We've bottomed out
    if (numShapes == 1) {
        // Total bounds for current level
        //AABB totalBounds;
        //for (int i = start; i < end; ++i) {
        //    totalBounds = Union(totalBounds, m_AABBs[i]);
        //}
        // Create leaf node
        return std::make_unique<BVHNode>(m_AABBs[start]);
    }

    AABB centerBounds;
    for (int i = start; i < end; ++i) {
        centerBounds = Union(centerBounds, m_AABBs[i].CalculateCenter());
    }
    auto axis = centerBounds.MaximumExtent();
    auto mid = (end + start) / 2;

    std::nth_element(&m_AABBs[start], &m_AABBs[mid], &m_AABBs[end-1],
                        [axis](const AABB& a, const AABB& b) {
                            return a.CalculateCenter()[axis] < b.CalculateCenter()[axis];
                        });

    return std::make_unique<BVHNode>(axis, BuildTree(start, mid), BuildTree(mid, end));
}


auto 
BVH::BVHNode::IsInteriorNode() const -> bool
{
    return m_leftChild || m_rightChild;
}

auto BVH::BVHNode::Intersects(const Rayf& ray) const -> std::optional<Intersection> {
    // Do we hit the bounding box?
    if (!m_aabb.IntersectsBox(ray)) 
        return std::nullopt;
    
    // Is the current node in the interior or a leaf?
    if (IsInteriorNode()) {
        auto leftIsect = m_leftChild->Intersects(ray);
        auto rightIsect = m_rightChild->Intersects(ray);
        // do right first - it can only have a value if it's closer
        if (rightIsect.has_value()) {
            return rightIsect;
        }
        if (leftIsect.has_value()) {
            return leftIsect;
        }
        return std::nullopt;
    }
    //    throw std::exception();
    return m_aabb.IntersectsMesh(ray);
}

auto BVH::BVHNode::IntersectsFast(const Rayf& ray) const -> bool {
    // Do we hit the bounding box?
    if (!m_aabb.IntersectsBox(ray))
        return false;

    // Is the current node in the interior or a leaf?
    if (IsInteriorNode()) {
        return m_leftChild->IntersectsFast(ray) || m_rightChild->IntersectsFast(ray);
    }
    return m_aabb.IntersectsMeshFast(ray);
}

auto 
BVH::Intersects(const Rayf& ray) const -> std::optional<Intersection> {
    return m_rootNode->Intersects(ray);
}

auto
BVH::IntersectsFast(const Rayf& ray) const -> bool {
    return m_rootNode->IntersectsFast(ray);
}
