#include "PolygonMesh.h"

bool PolygonMesh::intersects(const Rayf& ray, HitData& hit_data)
{
	for (const auto& polygon : m_mesh)
	{
		if (polygon->intersects(ray, hit_data))
		{
			hit_data.update_closest(polygon, ray);
		}
	}

	return hit_data.has_been_hit();
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

Vec3f PolygonMesh::get_surface_properties(HitData& hit_data) const
{
	return hit_data.m_closest_ptr->get_surface_properties(hit_data);
}