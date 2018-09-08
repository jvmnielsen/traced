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
        for ( auto& polygon : m_mesh )
        {
            polygon.intersects( ray, t, intercpt_coord );
        }
    }

	void add_polygon(const Polygon& polygon)
    {
		m_mesh.emplace_back(polygon);
    }

private:
    std::vector<Polygon> m_mesh;
};

