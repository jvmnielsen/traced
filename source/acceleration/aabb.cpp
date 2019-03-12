//
// Created by Jacob Vesti Moeslund Nielsen on 18/11/2018.
//
#include "aabb.hpp"
#include <functional>

AABB::AABB(Point3f lowerBound, Point3f upperBound)
        : m_bounds({lowerBound, upperBound})
        , m_mesh(nullptr)
{
}

AABB::AABB(std::unique_ptr<Mesh> mesh)
        : m_bounds(mesh->GetExtent())
        , m_mesh(std::move(mesh))
{
    m_center = CalculateCenter();
}

AABB::AABB(std::unique_ptr<Mesh> mesh, std::array<Point3f, 2> bounds)
    : m_bounds(bounds),
      m_mesh(std::move(mesh))
{ }


AABB::AABB(const AABB& other)
        : m_bounds(other.m_bounds)
        , m_mesh(std::make_unique<Mesh>(*other.m_mesh))
        , m_center(other.m_center)
{
} 


auto
AABB::operator=(const AABB& other) -> AABB&
{
    if (&other == this)
        return *this;

    m_bounds = other.m_bounds;

    if (other.m_mesh)
        m_mesh = std::make_unique<Mesh>(*other.m_mesh);

    if (other.m_center != m_center)
        m_center = other.m_center;

    return *this;
} 



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
        return false;

    if (tymin > tmin)
        tmin = tymin;
    if (tymax < tmax)
        tmax = tymax;

    tzmin = (m_bounds[ray.GetReciprocSigns().at(2)].z - ray.GetOrigin().z) * ray.GetReciprocDirection().z;
    tzmax = (m_bounds[1 - ray.GetReciprocSigns().at(2)].z - ray.GetOrigin().z) * ray.GetReciprocDirection().z;

    if ((tmin > tzmax) || (tzmin > tmax))
        return false;
    if (tzmin > tmin)
        tmin = tzmin;
    if (tzmax < tmax)
        tmax = tzmax;

    //return ( (tmin < t1) && (tmax > t0) );

    auto parameter = tmin;

    if (parameter < 0) {
        parameter = tmax;
        if (parameter < 0)
            return false;
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

auto AABB::GetBounds() const -> const std::array<Point3f, 2>&
{
    return m_bounds;
}

auto
AABB::LowerBound() const -> const Point3f&
{
    return m_bounds[0];
}

auto
AABB::UpperBound() const -> const Point3f&
{
    return m_bounds[1];
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


auto
AABB::CalculateCenter() const -> Point3f
{
    return m_bounds[0] + Diagonal() * 0.5f;
}


auto
AABB::MaximumExtent() const -> int
{
    const auto diagonal = Diagonal();
    if (diagonal.x > diagonal.y && diagonal.x > diagonal.z)
        return 0;
    else if (diagonal.y > diagonal.z)
        return 1;
    else
        return 2;
}


auto 
AABB::InsideBounds(const Point3f& p) const -> bool {
    
    const auto isNotInsideBoundsForAxis = [*this, &p] (int axis) {
        return p[axis] < this->LowerBound()[axis] || p[axis] > this->UpperBound()[axis];
    };

    for (int axis = 0; axis < 3; ++axis) 
        if (isNotInsideBoundsForAxis(axis))
            return false;
    
    return true;
}