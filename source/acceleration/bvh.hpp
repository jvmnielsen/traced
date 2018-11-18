#pragma once
#include <vector>
#include <memory>
#include "../geometry/mesh.hpp"

//#include "Source/Shape.h"
//#include "Source/Mesh.h"
//#include "AABB.h"

#include "aabb.hpp"


auto
Union(const AABB& b1, const AABB& b2) -> AABB;



class BVH
{
public:
    explicit BVH(std::vector<std::unique_ptr<Mesh>> meshes);

private:

    struct BVHNode
    {
        auto InitLeaf(int first, int n, const AABB& b) -> void
        {
            firstPrimOffset = first;
            nPrimitives = n;
            bounds = b;
            children[0] = children[1] = nullptr;

        }

        auto InitInner(int axis, BVHNode* c0, BVHNode* c1) -> void
        {
            children[0] = c0;
            children[1] = c1;
            bounds = Union(c0->bounds, c1->bounds);
            splitAxis = axis;
            nPrimitives =0;

        }

        AABB bounds;
        BVHNode* children[2];
        int splitAxis, firstPrimOffset, nPrimitives;
    };

    auto AxisOfMaximumExtent() const-> int;

    std::vector<AABB> m_AABBs;

    BVHNode* m_rootNode;

    auto
    BuildTree(int start, int end) -> BVHNode*;
};

