#pragma once
#include <vector>
#include <memory>
#include "aabb.hpp"
#include <optional>

class BVH {
public:
    explicit BVH(std::vector<std::unique_ptr<Mesh>> meshes);

    auto Intersects(const Rayf& ray) const->std::optional<Intersection>;
    auto IntersectsFast(const Rayf& ray) const -> bool;

private:

    struct BVHNode {

        explicit BVHNode(AABB aabb);
        BVHNode(std::unique_ptr<BVHNode> leftChild, std::unique_ptr<BVHNode> rightChild);

        auto Intersects(const Rayf& ray) const -> std::optional<Intersection>;
        auto IntersectsFast(const Rayf& ray) const -> bool;

        auto IsInteriorNode() const -> bool;

        AABB m_aabb;
        std::unique_ptr<BVHNode> m_left_child;
        std::unique_ptr<BVHNode> m_right_child;
        
    };

    std::vector<AABB> m_aabbs;
    std::unique_ptr<BVHNode> m_root_node;

    auto build_tree(int start, int end) -> std::unique_ptr<BVHNode>;
    
};

