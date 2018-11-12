#include "BVH.h"

AABB::AABB(std::array<Point3f, 2> bounds)
        : m_bounds(std::move(bounds))
{
    m_center = CalculateCenter();
}

AABB::AABB(Point3f lowerBound, Point3f upperBound)
    : m_bounds(std::move(std::array<Point3f, 2>{std::move(lowerBound), std::move(upperBound)}))
{
    m_center = CalculateCenter();
}


AABB::AABB(std::unique_ptr<Mesh> mesh)
        : m_bounds(mesh->GetExtent())
        , m_mesh(std::move(mesh))
{
    m_center = CalculateCenter();
}

auto 
AABB::Center() const -> const Point3f& 
{
    return m_center;
}

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


auto 
AABB::CalculateCenter() const -> Point3f
{
    return m_bounds[0] + Diagonal() * 0.5f;
}



BVH::BVH(std::vector<std::unique_ptr<Mesh>> shapes)
    : m_shapes(std::move(shapes))
{
    m_boundingBoxes.reserve(m_shapes.size());
    GenerateAABBList();
}


BVH::~BVH()
{
} 


auto
BVH::GenerateAABBList() -> void 
{
    for (auto& shape : m_shapes) 
    {
        m_boundingBoxes.emplace_back(std::move(shape)); // note: this is a constructor we're calling
    }
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
BVH::AxisOfMaximumExtent() const-> int
{
    AABB globalBounds;
    for (const auto& bounds : m_boundingBoxes)
        globalBounds = Union(globalBounds, bounds);

    return globalBounds.MaximumExtent();
}

auto 
BVH::BuildTree(int start, int end) -> void {   
    // total bounds for current level
    AABB totalBounds;
    for (int i = start; i < end; ++i)
        totalBounds = Union(totalBounds, m_boundingBoxes[i]);

    const int numberOfShapes = end - start;

    if (numberOfShapes == 1) {
        // create leaf node
    } else {
        AABB centerBounds;
        for (int i = start; i < end; ++i)
            centerBounds = Union(centerBounds, m_boundingBoxes[i].CalculateCenter());

        auto axis = centerBounds.MaximumExtent();

        auto mid = (end + start) / 2;

        std::nth_element(&m_boundingBoxes[start], &m_boundingBoxes[mid], &m_boundingBoxes[end],
                         [axis](const AABB& a,
                                const AABB& b) { return a.CalculateCenter()[axis] < });

    }
}