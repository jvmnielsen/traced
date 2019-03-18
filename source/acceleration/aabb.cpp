//
// Created by Jacob Vesti Moeslund Nielsen on 18/11/2018.
//
#include "aabb.hpp"
#include <functional>
#include "../geometry/mesh.hpp"

AABB::AABB(std::unique_ptr<Mesh> mesh)
        : m_bounds(mesh->GetExtent())
        , m_mesh(std::move(mesh))
{
    m_center = m_bounds.Center();
}

AABB::AABB(std::unique_ptr<Mesh> mesh, Bounds bounds)
    : m_bounds(std::move(bounds)),
      m_center(bounds.Center()),
      m_mesh(std::move(mesh))
{ }

auto
AABB::Center() const -> const Point3f&
{
    return m_center;
}

auto
AABB::GetShape() const -> const Mesh&
{
    return *m_mesh;
}

/*
auto
AABB::Intersects(const Rayf& ray, Intersection& isect) const -> bool
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

    if (parameter < 0) {
        parameter = tmax;
        if (parameter < 0)
            return false;
    }

    if (!ray.ParameterWithinUpperBound(parameter)) // We're only checking the upper bounds as we may be
        return false;                             // within a BV already

    IntersectsShape(ray, isect);

    //return isect.HasBeenHit();
    return true;
} */

bool AABB::IntersectsFast(const Rayf& ray) const
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

auto AABB::IntersectsBox(const Rayf& ray) const -> bool
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

    return true;
}


auto AABB::Intersects(const Rayf& ray) const -> std::optional<Intersection> {
    float tmin, tmax, tymin, tymax, tzmin, tzmax;

    tmin = (m_bounds[ray.GetReciprocSigns().at(0)].x - ray.GetOrigin().x) * ray.GetReciprocDirection().x;
    tmax = (m_bounds[1 - ray.GetReciprocSigns().at(0)].x - ray.GetOrigin().x) * ray.GetReciprocDirection().x;
    tymin = (m_bounds[ray.GetReciprocSigns().at(1)].y - ray.GetOrigin().y) * ray.GetReciprocDirection().y;
    tymax = (m_bounds[1 - ray.GetReciprocSigns().at(1)].y - ray.GetOrigin().y) * ray.GetReciprocDirection().y;

    if ((tmin > tymax) || (tymin > tmax))
        return std::nullopt;

    if (tymin > tmin)
        tmin = tymin;
    if (tymax < tmax)
        tmax = tymax;

    tzmin = (m_bounds[ray.GetReciprocSigns().at(2)].z - ray.GetOrigin().z) * ray.GetReciprocDirection().z;
    tzmax = (m_bounds[1 - ray.GetReciprocSigns().at(2)].z - ray.GetOrigin().z) * ray.GetReciprocDirection().z;

    if ((tmin > tzmax) || (tzmin > tmax))
        return std::nullopt;
    if (tzmin > tmin)
        tmin = tzmin;
    if (tzmax < tmax)
        tmax = tzmax;

    //return ( (tmin < t1) && (tmax > t0) );

    auto parameter = tmin;

    if (parameter < 0) {
        parameter = tmax;
        if (parameter < 0)
            return std::nullopt;
    }

    return m_mesh->Intersects(ray);
}


auto 
AABB::IntersectsMesh(const Rayf& ray) const->std::optional<Intersection> {
    return m_mesh->Intersects(ray);
}

auto
AABB::IntersectsMeshFast(const Rayf& ray) const -> bool
{
    return m_mesh->IntersectsFast(ray);
}

auto AABB::GetBounds() const -> const Bounds&
{
    return m_bounds;
}



auto
Bounds::Diagonal() const -> Vec3f {
    return {m_upper - m_lower};
}

auto
Bounds::SurfaceArea() const -> FLOAT {
    const auto diagonal = Diagonal();
    return 2.0 * (diagonal.x * diagonal.y + diagonal.x * diagonal.z + diagonal.y * diagonal.z);
}


auto
Bounds::CalculateCenter() const -> Point3f
{
    return m_lower + Diagonal() * 0.5f;
}


auto
Bounds::MaximumExtent() const -> int {
    const auto diagonal = Diagonal();
    if (diagonal.x > diagonal.y && diagonal.x > diagonal.z)
        return 0;

    if (diagonal.y > diagonal.z)
        return 1;
    
    return 2;
}


auto Bounds::IsInside(const Point3f& point) const -> bool {
   
    /*
    const auto isInsideBoundsForAxis = [this, p] (int axis) {
        return p[axis] >= LowerBound()[axis] && p[axis] <= UpperBound()[axis];
    };

    for (int axis = 0; axis < 3; ++axis)
        if (!isInsideBoundsForAxis(axis))
            return false;

    */
    return true;
}

auto
Bounds::Overlaps(const Bounds& other) const ->  bool {
    return     m_upper.x() >= other.Lower().x() && m_lower.x() <= other.Upper().x()
            && m_upper.y() >= other.Lower().y() && m_lower.y() <= other.Upper().y()
            && m_upper.z() >= other.Lower().z() && m_lower.z() <= other.Upper().z();
}


auto 
AABB::DoesNotContainMesh() const -> bool {
    return m_mesh->IsHollow();
}