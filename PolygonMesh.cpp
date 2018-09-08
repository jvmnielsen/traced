#include "PolygonMesh.h"

bool PolygonMesh::intersects(const Rayf& ray, float& t, Vec3f& intercpt_coord)
{
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

void PolygonMesh::add_polygon(const std::shared_ptr<Polygon>& polygon)
{
	m_mesh.push_back(polygon);
}

void PolygonMesh::transform_object_to_world(const Matrix44f& object_to_world)
{
	for (auto& polygon : m_mesh)
	{
		polygon->transform_object_to_world(object_to_world);
	}
}