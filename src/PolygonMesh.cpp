#include "PolygonMesh.h"

bool PolygonMesh::Intersects(const Rayf &ray, Intersection &hit_data)
{
	for (const auto& polygon : m_mesh)
	{
		if (polygon->Intersects(ray, hit_data))
		{
			hit_data.UpdateClosest(polygon.get(), ray);
		}
	}

	return hit_data.HasBeenHit();
}

void PolygonMesh::add_polygon(const std::shared_ptr<Polygon>& polygon)
{
	m_mesh.push_back(polygon);
}

void PolygonMesh::TransformByMatrix(const Matrix44f &object_to_world)
{
	for (auto& polygon : m_mesh)
	{
		polygon->TransformByMatrix(object_to_world);
	}
}
