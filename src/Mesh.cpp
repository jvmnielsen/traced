#include "Mesh.h"

bool Mesh::Intersects(const Rayf& ray, Intersection& isect)
{
	for (const auto& triangle : m_triangles)
	    if (triangle->Intersects(ray, isect))
	        isect.m_material = m_material.get();

	return isect.m_hasBeenHit;
}

bool Mesh::IntersectsQuick(const Rayf& ray) const
{
    for (const auto& triangle : m_triangles)
        if (triangle->IntersectsQuick(ray))
            return true;

    return false;
}

void Mesh::AddPolygon(std::unique_ptr<Triangle> polygon)
{
	m_triangles.push_back(std::move(polygon));
}

void Mesh::NormalAtIntesection(Intersection &hit_data) const
{
    //hit_data.m_shape->NormalAtIntersection(hit_data);
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


std::shared_ptr<Mesh> Mesh::Clone()
{
    auto newMesh = std::make_shared<Mesh>();

    for (const auto& polygon : m_triangles)
    {
        auto newPolygon = std::make_shared<Triangle>();

        Triangle triangle;
        *newPolygon = *polygon;

        newMesh->AddPolygon(std::move(newPolygon));
    }

    return newMesh;
}

std::unique_ptr<BoundingVolume> Mesh::GetBoundingVolume() const
{
    float minX = Math::Infinity;
    float minY = Math::Infinity;
    float minZ = Math::Infinity;
    float maxX = -Math::Infinity;
    float maxY = -Math::Infinity;
    float maxZ = -Math::Infinity;
    for (const auto& triangle : m_triangles)
    {
        for (const auto& vertices : triangle->m_vertex)
        {
            if (vertices.x < minX)
                minX = vertices.x;
            if (vertices.y < minY)
                minY = vertices.y;
            if (vertices.z < minZ)
                minZ = vertices.z;

            if (vertices.x > maxX)
                maxX = vertices.x;
            if (vertices.y > maxY)
                maxY = vertices.y;
            if (vertices.z > maxZ)
                maxZ = vertices.z;
        }
    }
    Point3f min { minX, minY, minZ };
    Point3f max { maxX, maxY, maxZ };
    return std::make_unique<BoundingVolume>( min, max );
}
