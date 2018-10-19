#include "Sphere.h"
//#include "Material.h"

void Sphere::CalculateNormal(Intersection &intersection) const
{
    intersection.m_normal = (intersection.m_point - m_center).Normalize();
}

bool Sphere::Intersects(const Rayf& ray, Intersection& isect)
{
    float solutionOne, solutionTwo;

    auto centerToOrigin = ray.Origin() - m_center;

    const auto a = ray.Direction().DotProduct(ray.Direction());
    const auto b = 2 * ray.Direction().DotProduct(centerToOrigin);
    const auto c = centerToOrigin.DotProduct(centerToOrigin) - m_radius_squared;

    if (!Math::SolveQuadratic(a, b, c, solutionOne, solutionTwo))
        return false;

    if (!(solutionOne > 0 || solutionTwo > 0))
        return false;

    ray.m_maxParam = (solutionOne < solutionTwo) ? solutionOne : solutionTwo;
    isect.m_shape = this;
    isect.m_point = ray.PointAtParameter(ray.m_maxParam);
    isect.m_hasBeenHit = true;

    return true;
 }

BoundingVolume Sphere::GetBoundingVolume() const
{
    return BoundingVolume{ Point3f{ -m_radius, -m_radius, -m_radius },
             Point3f{ m_radius, m_radius, m_radius } };
}

void Sphere::TransformBy(const Transform& transform)
{
    transform(m_center);
}
