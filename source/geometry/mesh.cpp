
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


void Mesh::TransformBy(const Transform& transform)
{
    for (auto& triangle : m_triangles)
        triangle.TransformBy(transform);
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
    auto randTriangle = m_triangles[sampler.GetRandomInDistribution(m_triangles.size())];
    return randTriangle.SampleSurface(sampler);
}

auto 
Mesh::SampleAsLight(const Intersection& ref, Sampler& sampler) const-> std::tuple<Intersection, Vec3f, double, Color3f>
{
    auto sampled = SampleSurface(sampler);

    auto wi = (sampled.GetPoint() - ref.GetPoint()).Normalize();
    auto pdf = 1.0 / m_surfaceArea;

    if (wi.LengthSquared() == 0)
        pdf = 0;
    else {
        // convert to solid angle
        auto dot = std::abs(Dot(sampled.GetGeometricNormal(), -wi));
        if (dot != 0.0)
            pdf *= (sampled.GetPoint() - ref.GetPoint()).LengthSquared() / dot;
                
        // TODO: check pdf is not inf
    }

    sampled.m_mesh = this;
    sampled.m_material = m_material.get();

    if (pdf == 0)
        return std::make_tuple(sampled, wi, pdf, Color3f::Black());

    return std::make_tuple(sampled, wi, pdf, m_material->Emitted(sampled, wi));
}


auto
Mesh::Pdf(const Point3f& ref, const Vec3f& wi) const -> float {
    /*
    Ray ray = Rayf{ref, wi};
    auto isect = Intersects(ray);
    if (isect.has_value()) {

        float pdf = (ref - isect->GetPoint()).LengthSquared()
                    / (std::abs(Dot(isect->GetGeometricNormal(), -wi)) * GetSurfaceArea());
        if (std::isinf(pdf))
            pdf = 0.0f;
        float pdf = 1/GetSurfaceArea();
        return pdf;
    }
    return 0.0f; */
    return 1/GetSurfaceArea();
}
