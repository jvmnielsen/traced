#pragma once
#include "RenderPrimitive.h"
#include <memory>
#include <vector>
#include "Polygon.h"

class PolygonMesh :
    public RenderPrimitive
{
public:
    PolygonMesh();
    ~PolygonMesh();

    bool intersects( const Rayf& ray, float& t, Vec3f& intercpt_coord ) override
    {
        for ( const auto& polygon : m_mesh )
        {
            polygon->intersects( ray, t, intercpt_coord );
        }
    }

private:
    std::vector<std::unique_ptr<Polygon>> m_mesh;
};

