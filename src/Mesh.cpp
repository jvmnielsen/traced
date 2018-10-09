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

void Mesh::AddPolygon(std::unique_ptr<Polygon> polygon)
{
	m_mesh.push_back(std::move(polygon));
}

void Mesh::TransformByMatrix(const Matrix44f &object_to_world)
{
	for (auto& polygon : m_mesh)
	{
		polygon->TransformByMatrix(object_to_world);
	}
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

void Mesh::TranslateBy(const Vecf& dir)
{
    for (auto& polygon : m_mesh)
        polygon->TranslateBy(dir);
}

void Mesh::RotateAroundX(float angle)
{
    for (auto& polygon : m_mesh)
        polygon->RotateAroundX(angle);
}

void Mesh::RotateAroundY(float angle)
{
    for (auto& polygon : m_mesh)
        polygon->RotateAroundY(angle);
}

void Mesh::RotateAroundZ(float angle)
{
    for (auto& polygon : m_mesh)
        polygon->RotateAroundZ(angle);
}

void Mesh::ScaleBy(float factor)
{
    for (auto& polygon : m_mesh)
        polygon->ScaleBy(factor);
}

std::unique_ptr<Mesh> Mesh::CloneMesh()
{
    auto newMesh = std::make_unique<Mesh>();

    for (const auto& polygon : m_mesh)
    {
        auto newPolygon = std::make_unique<Polygon>();

        *newPolygon = *polygon;

        newMesh->AddPolygon(std::move(newPolygon));
    }

    return newMesh;
}
