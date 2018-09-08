#pragma once
#include "RenderPrimitive.h"
#include <memory>
#include <vector>
#include "Polygon.h"
#include "Material.h"

class PolygonMesh :
    public RenderPrimitive
{
public:
    PolygonMesh();
    ~PolygonMesh();

    bool intersects( const Rayf& ray, float& t, Vec3f& intercpt_coord ) override
    {
		static const float infinity = std::numeric_limits<float>::max();

		HitData hit_data;

		for (auto& polygon : m_mesh)
		{
			if (polygon->intersects(ray, hit_data.m_t, hit_data.m_coordinates))
			{
				hit_data.update_closest_and_assign(polygon);
			}
		}

		if (hit_data.has_been_hit())
		{
			t = hit_data.m_t;
			intercpt_coord = hit_data.m_coordinates;
			return true;
		}

		return false;  
    }

	void add_polygon(const std::shared_ptr<Polygon>& polygon)
    {
		m_mesh.emplace_back(polygon);
    }

private:
    std::vector<std::shared_ptr<Polygon>> m_mesh;
	
};

