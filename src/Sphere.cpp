#include "Sphere.h"
//#include "Material.h"

void Sphere::CalculateNormal(Intersection &intersection) const
{
    intersection.SetNormal(Normalize(intersection.Point() - m_center));
}

bool Sphere::Intersects(const Rayf &ray, Intersection &intersection)
{
    float solutionOne, solutionTwo;

    auto center_at_orig = ray.origin() - m_center;

    const auto a = ray.direction().DotProduct(ray.direction());
    const auto b = 2 * ray.direction().DotProduct(center_at_orig);
    const auto c = center_at_orig.DotProduct(center_at_orig) - m_radius_squared;

    if (!solve_quadratic(a, b, c, solutionOne, solutionTwo))
        return false;

    if (!(solutionOne > 0 || solutionTwo > 0))
        return false;

    intersection.SetParameter(
            (solutionOne < solutionTwo) ? solutionOne : solutionTwo);

    return true;
 }

bool Sphere::solve_quadratic(
        const float a,
        const float b,
        const float c,
        float& solutionOne,
        float& solutionTwo) const
{
    const auto discr = b * b - 4 * a * c;

    if (discr < 0) 
        return false;

    else if (discr == 0) 
        solutionOne = solutionTwo = -0.5f * b / a;

    else
    {
        const float q = (b > 0) ?
            -0.5f * (b + sqrt( discr )) :
            -0.5f * (b - sqrt( discr ));

        solutionOne = q / a;
        solutionTwo = c / q;
    }

    if (solutionOne > solutionTwo)
        std::swap(solutionOne, solutionTwo);

    return true;
}
