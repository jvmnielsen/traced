#include "bvh.hpp"
#include "../geometry/mesh.hpp"

BVH::BVH(std::vector<std::unique_ptr<Mesh>> meshes) {

    //std::vector<AABB> aabbs;

    for (auto& shape : meshes) {
        //if (shape->triangle_count() > 10) {
        //    const auto internal_aabbs = shape->generate_internal_aabbs();
        //    for (auto&& aabb : internal_aabbs)
        //        m_aabbs.push_back(aabb);
        //} else {
            m_aabbs.emplace_back(std::move(shape));
        //}
    }

    m_root_node = build_tree(0, static_cast<int>(m_aabbs.size()));
}

auto BVH::build_tree(int start, int end) -> std::unique_ptr<BVHNode> {

    const auto num_shapes = end - start;

    // We've bottomed out
    if (num_shapes == 1) {
        // Create leaf node
        return std::make_unique<BVHNode>(m_aabbs[start]);
    }

    Bounds center_bounds{Point3f{0.0}, Point3f{0.0}};
    for (int i = start; i < end; ++i) {
        center_bounds = point_union(center_bounds, m_aabbs.at(i).center());
    }
    const auto axis = center_bounds.axis_of_max_extent();
    const int mid = (end + start) / 2;

    std::nth_element(&m_aabbs[start], &m_aabbs[mid], &m_aabbs[end-1],
                        [axis](const AABB& a, const AABB& b) {
                            return a.center()[axis] < b.center()[axis];
                        });

    return std::make_unique<BVHNode>(build_tree(start, mid), build_tree(mid, end));
}


auto 
BVH::BVHNode::IsInteriorNode() const -> bool
{
    return m_left_child || m_right_child;
}

auto BVH::BVHNode::Intersects(const Rayf& ray) const -> std::optional<Intersection> {
    // Do we hit the bounding box?
    if (!m_aabb.intersects_bounds(ray)) 
        return std::nullopt;
    
    // Is the current node in the interior or a leaf?
    if (IsInteriorNode()) {
        auto leftIsect = m_left_child->Intersects(ray);
        auto rightIsect = m_right_child->Intersects(ray);
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
    if (!m_aabb.intersects_bounds(ray))
        return false;

    // Is the current node in the interior or a leaf?
    if (IsInteriorNode()) {
        return m_left_child->IntersectsFast(ray) || m_right_child->IntersectsFast(ray);
    }
    return m_aabb.IntersectsMeshFast(ray);
}

BVH::BVHNode::BVHNode(
        std::unique_ptr<BVHNode> leftChild,
        std::unique_ptr<BVHNode> rightChild)
        : m_aabb(bounds_union(leftChild->m_aabb.bounds(), rightChild->m_aabb.bounds()))
        , m_left_child(std::move(leftChild))
        , m_right_child(std::move(rightChild)) {
}

BVH::BVHNode::BVHNode(AABB aabb)
    : m_aabb(std::move(aabb)),
      m_right_child(nullptr),
      m_left_child(nullptr)
{

}

auto 
BVH::Intersects(const Rayf& ray) const -> std::optional<Intersection> {
    return m_root_node->Intersects(ray);
}

auto
BVH::IntersectsFast(const Rayf& ray) const -> bool {
    return m_root_node->IntersectsFast(ray);
}
