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

    const auto parameter = (solutionOne < solutionTwo) ? solutionOne : solutionTwo;

    if (parameter > ray.m_maxParam || parameter < ray.m_minParam)
        return false;

    ray.m_maxParam = parameter;
    isect.m_shape = this;
    isect.m_point = ray.PointAtParameter(ray.m_maxParam);
    CalculateNormal(isect);
    isect.m_hasBeenHit = true;
    isect.m_matPtr = m_material.get();

    return true;
 }

bool Sphere::IntersectsQuick(const Rayf& ray) const
{
    float solutionOne, solutionTwo;

    auto centerToOrigin = ray.Origin() - m_center;

    const auto a = ray.Direction().DotProduct(ray.Direction());
    const auto b = 2 * ray.Direction().DotProduct(centerToOrigin);
    const auto c = centerToOrigin.DotProduct(centerToOrigin) - m_radius_squared;

    if (!Math::SolveQuadratic(a, b, c, solutionOne, solutionTwo))
        return false;

    return solutionOne > 0 || solutionTwo > 0;
}

std::shared_ptr<BoundingVolume> Sphere::GetBoundingVolume() const
{
    return std::make_shared<BoundingVolume>(Point3f{ -m_radius + m_center.x, -m_radius + m_center.y, -m_radius + m_center.z },
             Point3f{ m_radius + m_center.x, m_radius + m_center.y, m_radius + m_center.z });
}

void Sphere::TransformBy(const Transform& transform)
{
    transform(m_center);
}
