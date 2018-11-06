#include "Intersection.h"
#include "Shape.h"
//#include "Material.h"

Intersection::Intersection(
    Point3f point,
    Normal3f geometricNormal)
    : m_point(std::move(point))
    , m_geometricNormal(std::move(geometricNormal))
{}

//void Update(const Point3f& point, const Point2f& uvCoord, const Normal3f& geometricNormal, const Normal3f& shadingNormal, Triangle* shape);

void Intersection::Update(
    const Point3f&      point,
    const Point2f&      uvCoord,
    const Normal3f&     geometricNormal,
    const Normal3f&     shadingNormal,
    Triangle*           triangle)
{
    m_hasBeenHit = true;
    m_point = point;
    m_uv = uvCoord;
    m_geometricNormal = geometricNormal;
    m_triangle = triangle;
    m_shadingNormal = shadingNormal;
}

bool Intersection::HasBeenHit() const { return m_hasBeenHit; }
const Point3f& Intersection::GetPoint() const { return m_point; }
const Point2f& Intersection::GetUV() const { return m_uv; }
Triangle* Intersection::GetTriangle() const { return m_triangle; }
const Normal3f& Intersection::GetGeometricNormal() const { return m_geometricNormal; }
const Normal3f& Intersection::GetShadingNormal() const { return m_shadingNormal; }
Material* Intersection::GetMaterial() const { return m_material; }

Point3f Intersection::OffsetShadingPoint() const
{
    return m_point + m_shadingNormal * Math::Epsilon;
}

Point3f Intersection::OffsetGeometricPoint() const
{
    return m_point + m_geometricNormal * Math::Epsilon;
}

Color3f Intersection::CalculateEmitted() const
{
    if (m_material)
        return m_material->Emitted(m_uv, m_point);
    else
        return Color3f{0.0f};
}
