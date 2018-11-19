#pragma once
#include <vector>
#include <memory>
#include "../geometry/mesh.hpp"

//#include "Source/Shape.h"
//#include "Source/Mesh.h"
//#include "AABB.h"

#include "aabb.hpp"
#include <optional>

class BVH {
public:
    explicit BVH(std::vector<std::unique_ptr<Mesh>> meshes);

    //auto Intersects(const Rayf& ray) const -> std::optional<Intersection>;

private:

    struct BVHNode {

        explicit BVHNode(const AABB& bounds);
        BVHNode(int axis, std::unique_ptr<BVHNode> leftChild, std::unique_ptr<BVHNode> rightChild);

        auto Intersects(const Rayf& ray) const -> std::optional<Intersection>;

        AABB m_aabb;
        std::unique_ptr<BVHNode> m_leftChild;
        std::unique_ptr<BVHNode> m_rightChild;
        //BVHNode* children[2];
        int m_splitAxis; //, firstPrimOffset, nPrimitives;
    };

    std::vector<AABB> m_AABBs;

    std::unique_ptr<BVHNode> m_rootNode;

    std::vector<std::unique_ptr<BVHNode>> m_flattenedTree;

    auto BuildTree(int start, int end) -> std::unique_ptr<BVHNode>;
    auto FlattenTree(std::unique_ptr<BVHNode> rootNode) -> void;
};

