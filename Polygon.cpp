#include "Polygon.h"
#include <cmath>

void Polygon::transform_object_to_world(const Matrix44f& object_to_world)
{
	for(auto& vertex : m_vertices)
	{
		const auto tmp_vertex = object_to_world.multiply_with_point(vertex);
		vertex = tmp_vertex;
	}
}


bool Polygon::geometric_triangle_intersect( const Rayf& ray, float& t, Vec3f& intercpt_coord ) const
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

    if ( m_normal.dot( perpendicular_to_plane ) < 0 )
         return false;

    // edge 1
    const auto vertx1_to_intersect = intersection - m_vertx1;
    perpendicular_to_plane = m_edge1.cross( vertx1_to_intersect );

    if ( ( intercpt_coord.m_x = m_normal.dot( perpendicular_to_plane ) < 0))
        return false;

    // edge 2
    const auto vertx2_to_intersect = intersection - m_vertx2;
    perpendicular_to_plane = m_edge2.cross( vertx2_to_intersect );

    if ( ( intercpt_coord.m_y = m_normal.dot( perpendicular_to_plane ) ) < 0)
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

    intercpt_coord.m_x /= denom;
    intercpt_coord.m_y /= denom;
    intercpt_coord.m_z = 1 - intercpt_coord.m_x - intercpt_coord.m_y;

    return true;
}

bool Polygon::moller_trumbore_intersect( const Rayf& ray, float& t, Vec3f& intercpt_coord ) const
{
    const auto p_vec = ray.direction().cross( m_edge0_2 );
    const auto det = m_edge0.dot( p_vec );

    // back-face culling
    // do fabs( det ) to include back-face
    if ( det < m_epsilon)
        return false;

    // precompute for performance
    const auto inverted_det = 1 / det;

    // barycentric coordinate u
    const auto t_vec = ray.origin() - m_vertx0;
    intercpt_coord.m_x = t_vec.dot( p_vec ) * inverted_det;
    if (intercpt_coord.m_x < 0 || intercpt_coord.m_x > 1)
        return false;

    // barycentric coordinate v
    const auto q_vec = t_vec.cross( m_edge0 );
    intercpt_coord.m_y = ray.direction().dot( q_vec ) * inverted_det;
    if (intercpt_coord.m_y < 0 || intercpt_coord.m_y + intercpt_coord.m_x > 1)
        return false;


    intercpt_coord.m_z = m_edge0_2.dot( q_vec ) * inverted_det;

    return true;

}


bool Polygon::intersects(const Rayf& ray, float& t, Vec3f& intercpt_coord)
{
    return moller_trumbore_intersect( ray, t, intercpt_coord );
}