//
// Created by Jacob Vesti Moeslund Nielsen on 18/10/2018.
//

/*
#include "BoundingVolume.h"

AABB::AABB(std::array<Point3f, 2> bounds)
    : m_bounds(std::move(bounds))
{
}

AABB::AABB(std::unique_ptr<Mesh> mesh)
    : m_bounds(mesh->GetExtent())
    , m_mesh(std::move(mesh))
{
}

bool AABB::Intersects(const Rayf& ray, Intersection& isect) const
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

bool AABB::IntersectsQuick(const Rayf& ray) const
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

    return m_mesh->IntersectsFast(ray);
}

bool AABB::IntersectsShape(const Rayf& ray, Intersection& isect) const
{
    return m_mesh->Intersects(ray, isect);

}

void AABB::SetShape(std::unique_ptr<Mesh> shape)
{
    m_mesh = std::move(shape);
}

Mesh& AABB::GetShape() const
{
    return *m_mesh;
}


auto
AABB::Diagonal() const -> Vec3f
{
    return {m_bounds[1] - m_bounds[0]};
}

auto
AABB::SurfaceArea() const -> float
{
    const auto diagonal = Diagonal();
    return 2 * (diagonal.x * diagonal.y + diagonal.x * diagonal.z + diagonal.y * diagonal.z);
}
*/