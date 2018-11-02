//
// Created by Jacob Vesti Moeslund Nielsen on 18/10/2018.
//

#include "BoundingVolume.h"
//#include "Intersection.h"

BoundingVolume::BoundingVolume(
    const Point3f& lowerBound,
    const Point3f& upperBound)
{
    m_bounds[0] = lowerBound;
    m_bounds[1] = upperBound;
}


bool BoundingVolume::Intersects(const Rayf& ray, Intersection& isect) const
{
    // "An Efficient and Robust Ray-Box Intersection Algorithm"
    // Journal of graphics tools, 10(1):49-54, 2005

    float tmin, tmax, tymin, tymax, tzmin, tzmax;

    tmin  = (m_bounds[    ray.GetReciprocSigns().at(0)].x - ray.GetOrigin().x) * ray.GetReciprocDirection().x;
    tmax  = (m_bounds[1 - ray.GetReciprocSigns().at(0)].x - ray.GetOrigin().x) * ray.GetReciprocDirection().x;
    tymin = (m_bounds[    ray.GetReciprocSigns().at(1)].y - ray.GetOrigin().y) * ray.GetReciprocDirection().y;
    tymax = (m_bounds[1 - ray.GetReciprocSigns().at(1)].y - ray.GetOrigin().y) * ray.GetReciprocDirection().y;

    if ( (tmin > tymax) || (tymin > tmax) )
        return false;

    if (tymin > tmin)
        tmin = tymin;
    if (tymax < tmax)
        tmax = tymax;

    tzmin = (m_bounds[    ray.GetReciprocSigns().at(2)].z - ray.GetOrigin().z) * ray.GetReciprocDirection().z;
    tzmax = (m_bounds[1 - ray.GetReciprocSigns().at(2)].z - ray.GetOrigin().z) * ray.GetReciprocDirection().z;

    if ( (tmin > tzmax) || (tzmin > tmax) )
        return false;
    if (tzmin > tmin)
        tmin = tzmin;
    if (tzmax < tmax)
        tmax = tzmax;

    //return ( (tmin < t1) && (tmax > t0) );

    auto parameter = tmin; // > 0 ? tmin : tmax > 0 ? tmax : 0; //= tmin;

    if (parameter < 0)
    {
        parameter = tmax;
        if (parameter < 0)
            return false;
    }

    if (!ray.ParameterWithinUpperBound(parameter)) // We're only checking the upper bounds as we may be
        return false;                             // within a BV already

    IntersectsShape(ray, isect);

    return isect.HasBeenHit();
}

bool BoundingVolume::IntersectsQuick(const Rayf& ray) const
{
    float tmin, tmax, tymin, tymax, tzmin, tzmax;

    tmin  = (m_bounds[    ray.GetReciprocSigns().at(0)].x - ray.GetOrigin().x) * ray.GetReciprocDirection().x;
    tmax  = (m_bounds[1 - ray.GetReciprocSigns().at(0)].x - ray.GetOrigin().x) * ray.GetReciprocDirection().x;
    tymin = (m_bounds[    ray.GetReciprocSigns().at(1)].y - ray.GetOrigin().y) * ray.GetReciprocDirection().y;
    tymax = (m_bounds[1 - ray.GetReciprocSigns().at(1)].y - ray.GetOrigin().y) * ray.GetReciprocDirection().y;

    if ( (tmin > tymax) || (tymin > tmax) )
        return false;

    if (tymin > tmin)
        tmin = tymin;
    if (tymax < tmax)
        tmax = tymax;

    tzmin = (m_bounds[    ray.GetReciprocSigns().at(2)].z - ray.GetOrigin().z) * ray.GetReciprocDirection().z;
    tzmax = (m_bounds[1 - ray.GetReciprocSigns().at(2)].z - ray.GetOrigin().z) * ray.GetReciprocDirection().z;

    if ( (tmin > tzmax) || (tzmin > tmax) )
        return false;
    if (tzmin > tmin)
        tmin = tzmin;
    if (tzmax < tmax)
        tmax = tzmax;

    //return ( (tmin < t1) && (tmax > t0) );

    auto parameter = tmin;

    if (parameter < 0)
    {
        parameter = tmax;
        if (parameter < 0)
            return false;
    }

    return m_shape->IntersectsQuick(ray);
}

bool BoundingVolume::IntersectsShape(const Rayf& ray, Intersection& isect) const
{
    return m_shape->Intersects(ray, isect);

}

void BoundingVolume::SetShape(std::unique_ptr<Shape> shape)
{
    m_shape = std::move(shape);
}

Shape& BoundingVolume::GetShape() const
{
    return *m_shape;
}
