#include "Intersection.h"
#include "Shape.h"
//#include "Material.h"

Intersection::Intersection(
    bool hasBeenHit,
    const Point3f& point,
    const Point2f& barycentric,
    Shape* shape,
    const Normal3f& geometricNormal,
    const Normal3f& shadingNormal,
    Material* material)
    : m_hasBeenHit(hasBeenHit)
    , m_point(point)
    , m_uvCoord(barycentric)
    , m_shape(shape)
    , m_geometricNormal(geometricNormal)
    , m_shadingNormal(shadingNormal)
    , m_material(material)
{
}

Intersection::Intersection(
    Point3f point,
    Normal3f geometricNormal)
    : m_point(std::move(point))
    , m_geometricNormal(std::move(geometricNormal))
{}


void Intersection::Update(
    const Point3f&      point,
    const Point2f&      uvCoord,
    const Normal3f&     geometricNormal,
    Shape*              shape,
    Material*           material)
{
    m_hasBeenHit = true;

    m_point = point;
    m_uvCoord = uvCoord;
    m_geometricNormal = geometricNormal;
    m_shape = shape;
    m_material = material;

    m_geometricNormal = m_shape->CalculateShadingNormal(*this);
}

bool Intersection::HasBeenHit() const { return m_hasBeenHit; }
const Point3f& Intersection::GetPoint() const { return m_point; }
const Point2f& Intersection::GetUV() const { return m_uvCoord; }
Shape* Intersection::GetShape() const { return m_shape; }
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
    return m_material->Emitted(m_uvCoord, m_point);
}