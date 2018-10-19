#include "Mesh.h"

bool Mesh::Intersects(const Rayf& ray, Intersection& isect)
{
	for (const auto& triangle : m_triangles)
	    triangle->Intersects(ray, isect);

	return isect.m_hasBeenHit;
}

void Mesh::AddPolygon(std::shared_ptr<Triangle> polygon)
{
	m_triangles.push_back(polygon);
}

void Mesh::CalculateNormal(Intersection &hit_data) const
{
    hit_data.m_shape->CalculateNormal(hit_data);
}

/*
void Mesh::SetMaterialType(const MaterialType& type)
{
    for (auto& polygon : m_triangles)
    {
        polygon->SetMaterialType(type);
    }
} */

void Mesh::TransformBy(const Transform& transform)
{
    for (auto& triangle : m_triangles)
        triangle->TransformBy(transform);
}

/*
std::shared_ptr<Mesh> Mesh::CloneMesh()
{
    auto newMesh = std::make_unique<Mesh>();

    for (const auto& polygon : m_triangles)
    {
        auto newPolygon = std::make_unique<Triangle>();

        *newPolygon = *polygon;

        newMesh->AddPolygon(std::move(newPolygon));
    }

    return newMesh;
} */
