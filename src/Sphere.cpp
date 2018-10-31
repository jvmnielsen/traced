#include "Sphere.h"
//#include "Material.h"

inline Normal3f Sphere::NormalAtIntesection(Intersection& isect) const
{
    return (isect.GetPoint() - m_center).Normalize();
}

bool Sphere::Intersects(const Rayf& ray, Intersection& isect)
{
    float solutionOne, solutionTwo;

    auto centerToOrigin = ray.GetOrigin() - m_center;

    const auto a = ray.GetDirection().DotProduct(ray.GetDirection());
    const auto b = 2 * ray.GetDirection().DotProduct(centerToOrigin);
    const auto c = centerToOrigin.DotProduct(centerToOrigin) - m_radius_squared;

    if (!Math::SolveQuadratic(a, b, c, solutionOne, solutionTwo))
        return false;

    if (!(solutionOne > 0 || solutionTwo > 0))
        return false;

    const auto parameter = (solutionOne < solutionTwo) ? solutionOne : solutionTwo;

    if (ray.ParameterWithinBounds(parameter))
        return false;

    ray.NewMaxParameter(parameter);
    isect.Update(ray.PointAtParameter(parameter), Point2f{}, this, m_material.get());

    return true;
 }

bool Sphere::IntersectsQuick(const Rayf& ray) const
{
    float solutionOne, solutionTwo;

    auto centerToOrigin = ray.GetOrigin() - m_center;

    const auto a = ray.GetDirection().DotProduct(ray.GetDirection());
    const auto b = 2 * ray.GetDirection().DotProduct(centerToOrigin);
    const auto c = centerToOrigin.DotProduct(centerToOrigin) - m_radius_squared;

    if (!Math::SolveQuadratic(a, b, c, solutionOne, solutionTwo))
        return false;

    return solutionOne > 0 || solutionTwo > 0;
}

std::unique_ptr<BoundingVolume> Sphere::GetBoundingVolume() const
{
    return std::make_unique<BoundingVolume>(Point3f{ -m_radius + m_center.x, -m_radius + m_center.y, -m_radius + m_center.z },
             Point3f{ m_radius + m_center.x, m_radius + m_center.y, m_radius + m_center.z });
}

inline void Sphere::TransformBy(const Transform& transform)
{
    transform(m_center);
}
