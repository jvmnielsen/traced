#include "Mesh.h"

bool Mesh::Intersects(const Rayf &ray, Intersection &hit_data)
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

void Mesh::AddPolygon(std::unique_ptr<Triangle> polygon)
{
	m_mesh.push_back(std::move(polygon));
}

void Mesh::CalculateNormal(Intersection &hit_data) const
{
    hit_data.RenderablePtr()->CalculateNormal(hit_data);
}

void Mesh::SetMaterialType(const MaterialType& type)
{
    for (auto& polygon : m_mesh)
    {
        polygon->SetMaterialType(type);
    }
}

void Mesh::TransformBy(const Transform& transform)
{
    for (auto& triangle : m_mesh)
        triangle->TransformBy(transform);
}

std::unique_ptr<Mesh> Mesh::CloneMesh()
{
    auto newMesh = std::make_unique<Mesh>();

    for (const auto& polygon : m_mesh)
    {
        auto newPolygon = std::make_unique<Triangle>();

        *newPolygon = *polygon;

        newMesh->AddPolygon(std::move(newPolygon));
    }

    return newMesh;
}
