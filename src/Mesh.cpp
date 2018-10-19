#include "Mesh.h"

bool Mesh::Intersects(const Rayf& ray, Intersection& isect)
{
	for (const auto& triangle : m_triangles)
	    triangle->Intersects(ray, isect);

	isect.m_matPtr = m_material.get();

	return isect.m_hasBeenHit;
}

bool Mesh::IntersectsQuick(const Rayf& ray) const
{
    for (const auto& triangle : m_triangles)
        if (triangle->IntersectsQuick(ray))
            return true;

    return false;
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

std::shared_ptr<BoundingVolume> Mesh::GetBoundingVolume() const
{
    float minX = -Math::Infinity;
    float minY = -Math::Infinity;
    float minZ = -Math::Infinity;
    float maxX = Math::Infinity;
    float maxY = Math::Infinity;
    float maxZ = Math::Infinity;
    for (const auto& triangle : m_triangles)
    {
        for (const auto& vertices : triangle->m_vertex)
        {
            if (vertices.x < minX)
                minX = vertices.x;
            if (vertices.y < minY)
                minX = vertices.y;
            if (vertices.z < minZ)
                minX = vertices.z;

            if (vertices.x > maxX)
                minX = vertices.x;
            if (vertices.y > maxY)
                minX = vertices.y;
            if (vertices.z > minZ)
                minX = vertices.z;
        }
    }
    Point3f min { minX, minY, minZ };
    Point3f max { maxX, maxY, maxZ };
    return std::make_shared<BoundingVolume>( min, max );
}
