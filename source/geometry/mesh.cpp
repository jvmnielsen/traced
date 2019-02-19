
#include "mesh.hpp"

Mesh::Mesh(std::vector<Triangle> triangle)
    : m_triangles(std::move(triangle)) {

    m_surfaceArea = GetSurfaceArea();
}

auto 
Mesh::Intersects(const Rayf& ray) const -> std::optional<Intersection>
{
    std::optional<Intersection> isect;
    for (auto& triangle : m_triangles) {
        // the last overridden isect will always be the closest (ray max_param shrinks every time)
        auto tmp = triangle.Intersects(ray);
        if (tmp.has_value())
            isect = tmp;
    }
    if (isect.has_value()) {
        isect->m_material = m_material.get();
        isect->m_mesh = this;
        return isect;
    }

    return std::nullopt;//.has_value() ? std::optional<Intersection>{isect.value()} : std::nullopt;
}


bool Mesh::IntersectsFast(const Rayf& ray) const {
    for (const auto& triangle : m_triangles)
        if (triangle.IntersectsFast(ray))
            return true;

    return false;
}


void Mesh::TransformBy(std::unique_ptr<Transform> transform) {

    m_transformToWorld = std::move(transform);

    for (auto& triangle : m_triangles)
        triangle.TransformBy(*m_transformToWorld);
}

/*
std::unique_ptr<Mesh> Mesh::Clone()
{
    auto newMesh = std::make_unique<Mesh>();

    std::vector<Triangle> triangles;

    for (const auto& polygon : m_triangles)
    {
        auto copyTriangle = m_triangles;



        newMesh->AddPolygon(std::move(newPolygon));
    }

    return newMesh;
}
 */


auto 
Mesh::GetExtent() const->std::array<Point3f, 2>
{
    float minX = Math::Infinity;
    float minY = Math::Infinity;
    float minZ = Math::Infinity;
    float maxX = -Math::Infinity;
    float maxY = -Math::Infinity;
    float maxZ = -Math::Infinity;
    for (const auto& triangle : m_triangles)
    {
        for (const auto& vertices : triangle.GetVertices())
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
    return std::array<Point3f, 2>{ min, max };
}

auto
Mesh::GetSurfaceArea() const -> float
{
    float sum = 0;
    for (const auto& triangle : m_triangles)
        sum += triangle.GetArea();

    return sum;
}

auto
Mesh::ApplyMaterial(std::shared_ptr<Material> material) -> void
{
    m_material = std::move(material);
}

auto
Mesh::GetMaterial() const -> const Material&
{
    return *m_material;
}

auto
Mesh::SampleSurface(Sampler& sampler) const -> Intersection {
    const auto randTriangle = m_triangles[sampler.GetRandomInDistribution(m_triangles.size())];
    return randTriangle.SampleSurface(sampler);
}

auto 
Mesh::SampleAsLight(const Intersection& ref, Sampler& sampler) const -> std::tuple<Intersection, Normal3f, FLOAT, Color3f>
{
    auto sampled = SampleSurface(sampler);

    auto wi = Normalize(sampled.GetPoint() - ref.GetPoint());
    FLOAT pdf = 0; // = 1.0 / m_surfaceArea;

    if (wi.LengthSquared() == 0)
        pdf = 0;
    else {
        // convert to solid angle
        auto denom = std::abs(Dot(sampled.GetGeometricNormal(), -wi)) * GetSurfaceArea();
        if (denom != 0.0)
            pdf = (sampled.GetPoint() - ref.GetPoint()).LengthSquared() / denom;
    }

    sampled.m_mesh = this;
    sampled.m_material = m_material.get();

    if (pdf == 0)
        return std::make_tuple(sampled, wi, pdf, Color3f::Black());

    return std::make_tuple(sampled, wi, pdf, m_material->Emitted(sampled, -wi));
}


auto
Mesh::Pdf(const Intersection& ref, const Normal3f& wi) const -> float {
    
    Ray ray = Rayf{ref.GetPoint(), wi};
    auto isect = Intersects(ray);
    if (isect.has_value()) {

        const auto denom = std::abs(Dot(isect->GetGeometricNormal(), -wi)) * GetSurfaceArea();

        if (denom == 0)
            return 0;

        return (ref.GetPoint() - isect->GetPoint()).LengthSquared() / denom;
    }
    return 0; 

   
    return 1/GetSurfaceArea();
}


auto
Mesh::GetPreTransformedPoint(const Point3f& p) const -> Point3f {
    return m_transformToWorld->Inverse(p);
}
