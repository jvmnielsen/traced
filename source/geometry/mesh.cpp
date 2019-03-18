
#include "mesh.hpp"
#include <functional>
#include "../acceleration/aabb.hpp"

Mesh::Mesh(std::vector<Triangle> triangle)
    : m_triangles(std::move(triangle)) {

    /*
    if (m_triangles.size() > lowerBoundForTriangleCount) {
        GenerateInternalBoundingBoxes(static_cast<int>(std::sqrt(m_triangles.size())));
    }*/
    m_surfaceArea = GetSurfaceArea();
}




auto 
Mesh::Intersects(const Rayf& ray) const -> std::optional<Intersection> {

    std::optional<Intersection> isect;

    if (!m_internalBoundingBoxes.empty()) {
        for (auto& aabb : m_internalBoundingBoxes) {
            auto tmp = aabb.Intersects(ray);
            if (tmp.has_value())
                isect = tmp;
        }
    } else {
        for (auto& triangle : m_triangles) {
            // the last overridden isect will always be the closest (ray max_param shrinks every time)
            auto tmp = triangle.Intersects(ray);
            if (tmp.has_value())
                isect = tmp;
        }
    }

    if (isect.has_value()) {
        isect->SetMeshAndMaterial(this, m_material.get());
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
Mesh::GetExtent() const -> std::array<Point3f, 2> {

    Point3f min{ Math::Infinity };
    Point3f max{ -Math::Infinity };

    for (const auto& triangle : m_triangles) {
        for (const auto& vertices : triangle.GetVertices()) {

            auto assignMin = [&min, &vertices] (int axis) {
                if (vertices[axis] < min[axis])
                    min[axis] = vertices[axis];
            };

            auto assignMax = [&max, &vertices](int axis) {
                if (vertices[axis] > max[axis])
                    max[axis] = vertices[axis];
            };
        
            for (int axis = 0; axis < 3; ++axis) {
                assignMin(axis);
                assignMax(axis);
            }
        }
    }
   
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
Mesh::SampleRandomTriangle(Sampler& sampler) const -> std::tuple<Intersection, FLOAT> {
    const auto randTriangle = m_triangles[sampler.GetRandomInDistribution(m_triangles.size())];
    return randTriangle.SampleSurface(sampler);
}

auto
Mesh::Sample(const Intersection& ref, Sampler& sampler) const -> std::tuple<Intersection, FLOAT> {
    auto [sampledIsect, pdf] = SampleRandomTriangle(sampler);
    const auto wi = Normalize(sampledIsect.GetPoint() - ref.GetPoint());

    sampledIsect.SetMeshAndMaterial(this, m_material.get());

    const auto denom = std::abs(Dot(sampledIsect.GetGeometricNormal(), -wi)) * GetSurfaceArea();

    if (denom != 0.0)
        pdf *= (sampledIsect.GetPoint() - ref.GetPoint()).LengthSquared() / denom;

    return std::make_tuple(sampledIsect, pdf);

}

auto 
Mesh::SampleAsLight(const Intersection& ref, Sampler& sampler) const -> std::tuple<Intersection, Normal3f, FLOAT, Color3f>
{
    const auto [sampledIsect, pdf] = Sample(ref, sampler);

    const auto wi = Normalize(sampledIsect.GetPoint() - ref.GetPoint());

    if (pdf == 0)
        return std::make_tuple(sampledIsect, wi, pdf, Color3f::Black());

    return std::make_tuple(sampledIsect, wi, pdf, m_material->Emitted(sampledIsect, -wi));
}


auto
Mesh::Pdf(const Intersection& ref, const Normal3f& wi) const -> FLOAT {
    
    Ray ray = Rayf{ref.GetPoint(), wi};
    auto isect = Intersects(ray);
    if (isect.has_value()) {

        const auto denom = std::abs(Dot(isect->GetGeometricNormal(), -wi)) * GetSurfaceArea();

        if (denom == 0)
            return 0;

        const auto a =  (ref.GetPoint() - isect->GetPoint()).LengthSquared() / denom;
        return a;
    }
    return 0;
}


auto
Mesh::GetPreTransformedPoint(const Point3f& p) const -> Point3f {
    return m_transformToWorld->Inverse(p);
}


auto 
Mesh::SetTrianglesInsideBounds(AABB& bounds) -> void {
    
    std::vector<Triangle> insideBounds;

    for (const auto triangle : m_triangles) {
        for (const auto& vertex : triangle.GetVertices()) {
            if (bounds.PointInsideBounds(vertex)) {
                insideBounds.push_back(triangle);
                break;
            }
        }
    }

    auto new_mesh = std::make_unique<Mesh>(std::move(insideBounds));
    new_mesh->ApplyMaterial(m_material);

    bounds.SetMesh(std::move(new_mesh));
}



auto 
Mesh::GenerateInternalBoundingBoxes(std::size_t numDivisions) -> void {
    
    const auto totalExtent = GetExtent();
    std::vector<AABB> internalBounds;
    internalBounds.reserve(numDivisions);

    const auto axisValueForIntervals = [&totalExtent, numDivisions] (int axis) {
        return std::abs(totalExtent.at(1)[axis] - totalExtent.at(0)[axis]) / numDivisions;
    };

    std::array<FLOAT, 3> lengthIntervals = {
        axisValueForIntervals(0),
        axisValueForIntervals(1),
        axisValueForIntervals(2)
    };

    const auto generateAxisValueForBounds = [&totalExtent, &lengthIntervals] (std::size_t iteration, int axis) {
        return totalExtent.at(0)[axis] + iteration * lengthIntervals.at(axis);
    };

    for (size_t i = 0; i < numDivisions; ++i) {
        for (size_t j = 0; j < numDivisions; ++j) {
            for (size_t k = 0; k < numDivisions; ++k) {

                auto generatePoint = [=](int offset) {
                    return Point3f{ generateAxisValueForBounds(i + offset, 0),
                                    generateAxisValueForBounds(j + offset, 1),
                                    generateAxisValueForBounds(k + offset, 2)};
                };

                internalBounds.emplace_back(generatePoint(0), generatePoint(1));
            }
        }
    }

    for (auto& aabb : internalBounds) 
        SetTrianglesInsideBounds(aabb);

    internalBounds.erase(std::remove_if(internalBounds.begin(), internalBounds.end(), [](const AABB& bound) { return bound.DoesNotContainMesh(); }), internalBounds.end());
    m_internalBoundingBoxes = internalBounds;

    //m_triangles.clear();
}


auto
Mesh::GetInternalBoundingBoxes() const -> const std::vector<AABB>&
{
    return m_internalBoundingBoxes;
}

auto Mesh::IsHollow() const -> bool
{
    return m_triangles.empty();
}

