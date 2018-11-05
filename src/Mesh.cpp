#include "Mesh.h"
#include "BoundingVolume.h"

bool Mesh::Intersects(const Rayf& ray, Intersection& isect)
{
    for (const auto& triangle : m_triangles)
        if (triangle->Intersects(ray, isect))
            isect.SetParentMeshMaterial(m_material.get());
	        
	return isect.HasBeenHit();
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

void Mesh::TransformBy(const Transform& transform)
{
    for (auto& triangle : m_triangles)
        triangle->TransformBy(transform);
}


std::unique_ptr<Mesh> Mesh::Clone()
{
    auto newMesh = std::make_unique<Mesh>();

    for (const auto& polygon : m_triangles)
    {
        auto newPolygon = std::make_unique<Triangle>();

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
        for (const auto& vertices : triangle->GetVertices())
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

auto
Mesh::GetRandomTriangleVertex() -> int
{
    std::uniform_int_distribution<> dist(0, m_triangles.size()-1);
    return dist(m_gen);
}

Point3f Mesh::GetRandomPointOnSurface()
{
    return {-1.0, -1.0, -1.0};
}

auto
Mesh::GetRandomSurfaceIntersection() -> Intersection
{
    auto randTriangle = *m_triangles[GetRandomTriangleVertex()];
    return randTriangle.GetRandomSurfaceIntersection();
}

auto 
Mesh::GetPointOnSurface(const float u, const float v) const -> Point3f
{
    return Point3f{0};
}

auto
Mesh::CalculateShadingNormal(const Intersection& isect) const -> Normal3f
{
    return Normal3f{0};
}

/*
auto 
Mesh::SetParentMeshMaterial(std::shared_ptr<Material> material) -> void
{
    for (auto& triangle : m_triangles)
        triangle->SetParentMeshMaterial(material);
}
*/

//inline Point3f GetPointOnSurface(const float u, const float v) const override;
//inline Point3f GetRandomPointOnSurface() override;
//inline Intersection GetRandomSurfaceIntersection() override;
