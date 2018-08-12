#include "Polygon.h"
#include <cmath>

bool inside_outside_test( const Vec3f& vertx, const Vec3f& intersection, const Vec3f& point, const Vec3f& normal )
{
    /* The inside-outside test simply checks that the point is to the left of every vertex
    * by taking the dot product of the plane's normal and the vector formed by taking the
    * cross product of the vertex and a vector to the point in question. If they point
    * in the same direction the dot product will be > 0 ( cos(0) ).
    *
    * We are doing them one at a time, as we want to terminate immediately if the test fails
    * even once.
    */
    const auto point_to_intersect = intersection - point;
    const auto perpendicular_to_plane = cross( vertx, point_to_intersect );

    return normal.dot(perpendicular_to_plane) >= 0;
}

bool Polygon::geometric_triangle_intersect( const Rayf& ray, float& t ) const
{
    // back-face culling
    if ( dot( ray.direction(), m_normal ) > 0 && m_is_single_sided )
        return false;

    // ray & plane parallel?
    const auto normal_dot_ray_dir = dot( m_normal, ray.direction() );
    
    if ( fabs( normal_dot_ray_dir ) < m_epsilon )
        return false;

    

    /* Derived by substituting ray equation ( O + t R ) into the equation for a plane ( Ax + By + Cz + D = 0 ) 
     * and solving for parameter t.
     * 
     * Because the camera by default is oriented along the negative z-axis we have cancelled
     * the minus from the equation.
     */
    t = ( m_normal.dot( ray.origin() ) + m_dist_origin_to_plane ) / normal_dot_ray_dir; 

    if (t < 0) // triangle is behind ray
        return false;

    const auto intersection = ray.origin() + t * ray.direction();

    // inside-outside test for all vertices
    if (!(inside_outside_test( m_vertx0, intersection, m_point0, m_normal )))
        return false;

    if (!(inside_outside_test( m_vertx1, intersection, m_point1, m_normal )))
        return false;

    if (!(inside_outside_test( m_vertx2, intersection, m_point2, m_normal )))
        return false;

    return true;
}

bool Polygon::intersects( const Rayf& ray, float& t ) const
{
    return geometric_triangle_intersect( ray, t );
}
