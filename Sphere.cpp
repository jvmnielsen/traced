#include "Sphere.h"

bool Sphere::intersects(const Rayf& ray, float& t, Vec3f& intercpt_coord)
{
    float solu_one, solu_two;

    auto center_at_orig = m_center - ray.origin();

    const auto a = ray.direction().dot( ray.direction() );
    const auto b = 2 * ray.direction().dot( center_at_orig );
    const auto c = center_at_orig.dot( center_at_orig ) - m_radius_squared;

    if ( !solve_quadratic( a, b, c, solu_one, solu_two ) )
        return false;

    if (!(solu_one > 0 || solu_two > 0))
        return false;

    t = (solu_one > solu_two) ?
        solu_one :
        solu_two;

    return true;


}

bool Sphere::solve_quadratic(const float a, const float b, const float c, float& solu_one, float& solu_two) const
{
    const auto discr = b * b - 4 * a * c;

    if (discr < 0) 
        return false;

    else if (discr == 0) 
        solu_one = solu_two = -0.5 * b / a;

    else
    {
        const float q = (b > 0) ?
            -0.5 * (b + sqrt( discr )) :
            -0.5 * (b - sqrt( discr ));

        solu_one = q / a;
        solu_two = c / q;
    }

    if (solu_one > solu_two)
        std::swap( solu_one, solu_two );

    return true;
}
