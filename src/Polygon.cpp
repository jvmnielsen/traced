#include "Polygon.h"
#include <cmath>

void Polygon::update_edges()
{
	m_edge0 = m_vertices[1] - m_vertices[0];
	m_edge0_2 = m_vertices[2] - m_vertices[0];
	m_edge1 = m_vertices[2] - m_vertices[1];
	m_edge2 = m_vertices[0] - m_vertices[2];
	m_normal = CrossProduct(m_vertices[1] - m_vertices[0], m_vertices[2] - m_vertices[0]).Normalize(); 
	m_dist_origin_to_plane = m_normal.DotProduct(m_vertices[0]);
}

void Polygon::transform_object_to_world(const Matrix44f& object_to_world)
{
	for(auto& vertex : m_vertices)
	{
		const auto tmp_vertex = object_to_world.multiply_with_point(vertex);
		vertex = tmp_vertex;
	}
	update_edges();
}


bool Polygon::geometric_triangle_intersect(const Rayf& ray, HitData& hit_data) const
{
    // back-face culling
    if (DotProduct( ray.direction(), m_normal ) > 0 && m_is_single_sided )
        return false;

    // ray & plane parallel?
    const auto normal_dot_ray_dir = DotProduct( m_normal, ray.direction() );
    
    if ( fabs( normal_dot_ray_dir ) < m_epsilon )
        return false;

    /* Derived by substituting ray equation ( O + t R ) into the equation for a plane ( Ax + By + Cz + D = 0 ) 
     * and solving for parameter t.
     * 
     * Because the camera by default is oriented along the negative z-axis we have cancelled
     * the minus from the equation.
     */
    hit_data.m_t = ( m_normal.DotProduct( ray.origin() ) + m_dist_origin_to_plane ) / normal_dot_ray_dir; 

    if (hit_data.m_t < 0) // triangle is behind ray
        return false;

    const auto intersection = ray.origin() + hit_data.m_t * ray.direction();

    Vecf perpendicular_to_plane;

    /* The inside-outside test simply checks that the point is to the left of every vertex
    * by taking the DotProduct product of the plane's normal and the vector formed by taking the
    * CrossProduct product of the vertex and a vector to the point in question. If they point
    * in the same direction the DotProduct product will be > 0 ( cos(0) ).
    *
    * We are doing them one at a time as we want to terminate immediately if the test fails.
    */

    // edge 0
    const auto vertx0_to_intersect = intersection - m_vertices[0];
    perpendicular_to_plane = m_edge0.CrossProduct( vertx0_to_intersect );

    if ( m_normal.DotProduct( perpendicular_to_plane ) < 0 )
         return false;

    // edge 1
    const auto vertx1_to_intersect = intersection - m_vertices[1];
    perpendicular_to_plane = m_edge1.CrossProduct( vertx1_to_intersect );

    if ( ( hit_data.m_point.x = m_normal.DotProduct( perpendicular_to_plane ) < 0))
        return false;

    // edge 2
    const auto vertx2_to_intersect = intersection - m_vertices[2];
    perpendicular_to_plane = m_edge2.CrossProduct( vertx2_to_intersect );

    if ( ( hit_data.m_point.y = m_normal.DotProduct( perpendicular_to_plane ) ) < 0)
        return false;

    /* The barycentric coordinates are the ratio of the triangles formed when drawing
     * lines from the vertices to the intercept point.
     *
     * They are calculated using the area of the parallelogram (CrossProduct product) they 
     * form. 
     * 
     * for example: v = triangleABP_area / triangleABC_area = ( AB x AP ) . N / ( AB x AC ) . N
     */
    
    const auto denom = m_normal.DotProduct( m_normal );

    hit_data.m_point.x /= denom;
    hit_data.m_point.y /= denom;
	hit_data.m_point.z = 1 - hit_data.m_point.x - hit_data.m_point.y;

    return true;
}

bool Polygon::moller_trumbore_intersect(const Rayf& ray, HitData& hit_data) const
{
    const auto p_vec = ray.direction().CrossProduct( m_edge0_2 );
    const auto det = m_edge0.DotProduct( p_vec );

    // back-face culling
    // do fabs( det ) to include back-face
    if ( det < m_epsilon)
        return false;

    // precompute for performance
    const auto inverted_det = 1 / det;

    // barycentric coordinate u
    const auto t_vec = ray.origin() - m_vertices[0];
	hit_data.m_barycentric_coord.x = t_vec.DotProduct( p_vec ) * inverted_det;
    if (hit_data.m_barycentric_coord.x < 0 || hit_data.m_barycentric_coord.x > 1)
        return false;

    // barycentric coordinate v
    const auto q_vec = t_vec.CrossProduct( m_edge0 );
	hit_data.m_barycentric_coord.y = ray.direction().DotProduct( q_vec ) * inverted_det;
    if (hit_data.m_barycentric_coord.y < 0 || hit_data.m_barycentric_coord.y + hit_data.m_barycentric_coord.x > 1)
        return false;


	hit_data.m_t = m_edge0_2.DotProduct( q_vec ) * inverted_det;

    return true;

}


bool Polygon::intersects(const Rayf& ray, HitData& hit_data)
{
    return moller_trumbore_intersect( ray, hit_data );
}

void Polygon::set_normal(HitData& hit_data) const
{
    hit_data.m_normal = (1 - hit_data.barycentric().x - hit_data.barycentric().y) * m_vertex_normals[0]
        + hit_data.barycentric().x * m_vertex_normals[1]
        + hit_data.barycentric().y * m_vertex_normals[2];

    // for flat shading simply return the face normal
    hit_data.m_normal.Normalize();
}
