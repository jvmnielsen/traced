#include "bvh.hpp"
#include "../geometry/mesh.hpp"

using namespace tr;
using namespace gm;

BVH::BVH(std::vector<std::unique_ptr<Mesh>> meshes) {
    for (auto& shape : meshes) {
        m_aabbs.emplace_back(std::move(shape));
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
    Bounds center_bounds{Point3f(0), Point3f(0)};
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


auto BVH::BVHNode::is_interior_node() const -> bool {
    return m_left_child || m_right_child;
}

auto BVH::BVHNode::intersects(Rayf const& ray) const -> std::optional<Intersection> {
    // Do we hit the bounding box?
    if (!m_aabb.intersects_bounds(ray)) 
        return std::nullopt;
    
    // Is the current node in the interior or a leaf?
    if (is_interior_node()) {
        auto left_isect = m_left_child->intersects(ray);
        auto right_isect = m_right_child->intersects(ray);
        // do right first - it can only have a value if it's closer
        if (right_isect.has_value()) {
            return right_isect;
        }
        if (left_isect.has_value()) {
            return left_isect;
        }
        return std::nullopt;
    }
    return m_aabb.intersects_mesh(ray);
}

BVH::BVHNode::BVHNode(
        std::unique_ptr<BVHNode> leftChild,
        std::unique_ptr<BVHNode> rightChild)
        : m_aabb(bounds_union(leftChild->m_aabb.bounds(), rightChild->m_aabb.bounds()))
        , m_left_child(std::move(leftChild))
        , m_right_child(std::move(rightChild)) {
}

BVH::BVHNode::BVHNode(AABB aabb)
    : m_aabb(aabb),
      m_right_child(nullptr),
      m_left_child(nullptr) {

}

auto BVH::intersects(const Rayf& ray) const -> std::optional<Intersection> {
    return m_root_node->intersects(ray);
}