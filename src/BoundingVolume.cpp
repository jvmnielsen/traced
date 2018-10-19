//
// Created by Jacob Vesti Moeslund Nielsen on 18/10/2018.
//

#include "BoundingVolume.h"
#include "Intersection.h"

bool BoundingVolume::Intersects(const Rayf& ray, Intersection& isect) const
{
    // "An Efficient and Robust Ray-Box Intersection Algorithm"
    // Journal of graphics tools, 10(1):49-54, 2005

    float tmin, tmax, tymin, tymax, tzmin, tzmax;

    tmin = (m_bounds[ray.m_sign[0]].x - ray.Origin().x) * ray.m_invDirection.x;
    tmax = (m_bounds[1-ray.m_sign[0]].x - ray.Origin().x) * ray.m_invDirection.x;
    tymin = (m_bounds[ray.m_sign[1]].y - ray.Origin().y) * ray.m_invDirection.y;
    tymax = (m_bounds[1-ray.m_sign[1]].y - ray.Origin().y) * ray.m_invDirection.y;

    if ( (tmin > tymax) || (tymin > tmax) )
        return false;

    if (tymin > tmin)
        tmin = tymin;
    if (tymax < tmax)
        tmax = tymax;

    tzmin = (m_bounds[ray.m_sign[2]].z - ray.Origin().z) * ray.m_invDirection.z;
    tzmax = (m_bounds[1-ray.m_sign[2]].z - ray.Origin().z) * ray.m_invDirection.z;

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

    if (parameter > ray.m_maxParam)
        return false;

    IntersectsShape(ray, isect);

    return isect.m_hasBeenHit;
}

bool BoundingVolume::IntersectsQuick(const Rayf& ray) const
{
    float tmin, tmax, tymin, tymax, tzmin, tzmax;

    tmin = (m_bounds[ray.m_sign[0]].x - ray.Origin().x) * ray.m_invDirection.x;
    tmax = (m_bounds[1-ray.m_sign[0]].x - ray.Origin().x) * ray.m_invDirection.x;
    tymin = (m_bounds[ray.m_sign[1]].y - ray.Origin().y) * ray.m_invDirection.y;
    tymax = (m_bounds[1-ray.m_sign[1]].y - ray.Origin().y) * ray.m_invDirection.y;

    if ( (tmin > tymax) || (tymin > tmax) )
        return false;

    if (tymin > tmin)
        tmin = tymin;
    if (tymax < tmax)
        tmax = tymax;

    tzmin = (m_bounds[ray.m_sign[2]].z - ray.Origin().z) * ray.m_invDirection.z;
    tzmax = (m_bounds[1-ray.m_sign[2]].z - ray.Origin().z) * ray.m_invDirection.z;

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
