#include "Polygon.h"
#include <cmath>

bool Polygon::geometric_triangle_intersect( const Rayf& ray, float& t )
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

    Vec3f perpendicular_to_plane;

    /* The inside-outside test simply checks that the point is to the left of every vertex
    * by taking the dot product of the plane's normal and the vector formed by taking the
    * cross product of the vertex and a vector to the point in question. If they point
    * in the same direction the dot product will be > 0 ( cos(0) ).
    *
    * We are doing them one at a time as we want to terminate immediately if the test fails.
    */

    // edge 0
    const auto vertx0_to_intersect = intersection - m_vertx0;
    perpendicular_to_plane = m_edge0.cross( vertx0_to_intersect );

    if ( m_normal.dot( perpendicular_to_plane ) > 0 )
         return false;

    // edge 1
    const auto vertx1_to_intersect = intersection - m_vertx1;
    perpendicular_to_plane = m_edge1.cross( vertx1_to_intersect );

    if ( ( m_barycentric_intercpt.m_x = m_normal.dot( perpendicular_to_plane ) > 0))
        return false;

    // edge 2
    const auto vertx2_to_intersect = intersection - m_vertx2;
    perpendicular_to_plane = m_edge2.cross( vertx2_to_intersect );

    if ( ( m_barycentric_intercpt.m_y = m_normal.dot( perpendicular_to_plane ) ) > 0)
        return false;

    /* The barycentric coordinates are the ratio of the triangles formed when drawing
     * lines from the vertices to the intercept point.
     *
     * They are calculated using the area of the parallelogram (cross product) they 
     * form. 
     * 
     * for example: v = triangleABP_area / triangleABC_area = ( AB x AP ) . N / ( AB x AC ) . N
     */
    
    const auto denom = m_normal.dot( m_normal );

    m_barycentric_intercpt.m_x /= denom;
    m_barycentric_intercpt.m_y /= denom;
    m_barycentric_intercpt.m_z = 1 - m_barycentric_intercpt.m_x - m_barycentric_intercpt.m_y;

    return true;
}

bool Polygon::intersects( const Rayf& ray, float& t )
{
    return geometric_triangle_intersect( ray, t );
}
