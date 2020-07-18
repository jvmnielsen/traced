#pragma once

#include "aabb.hpp"
#include "../geometry/mesh.hpp"
#include "../core/intersection.hpp"

#include <vector>
#include <memory>
#include <optional>

namespace tr {

    class BVH {
    public:
        explicit BVH(std::vector<std::unique_ptr<Mesh>> meshes);
        auto intersects(Rayf const& ray) const->std::optional<Intersection>;

    private:

        struct BVHNode {
            explicit BVHNode(AABB aabb);
            BVHNode(std::unique_ptr<BVHNode> leftChild, std::unique_ptr<BVHNode> rightChild);
            auto intersects(Rayf const& ray) const -> std::optional<Intersection>;
            auto is_interior_node() const -> bool;

            AABB m_aabb;
            std::unique_ptr<BVHNode> m_left_child;
            std::unique_ptr<BVHNode> m_right_child;
        };

        std::vector<AABB> m_aabbs;
        std::unique_ptr<BVHNode> m_root_node;

        auto build_tree(int start, int end) -> std::unique_ptr<BVHNode>;
        
    };

}
