#include "Intersection.h"

inline void Intersection::Update(
    const Point3f&      point,
    const Point2f&      uvCoord,
    Shape*              shape,
    Material*           material)
{
    m_hasBeenHit = true;

    m_point = point;
    m_uvCoord = uvCoord;
    m_shape = shape;
    m_material = material;

    m_normal = m_shape->NormalAtIntersection(*this);
}