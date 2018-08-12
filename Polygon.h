#pragma once
#include "RenderPrimitive.h"
#include "Vec3.h"

class Polygon :
    public RenderPrimitive
{
public:
    Polygon( const Vec3f& vertx0, const Vec3f& vertx1, const Vec3f& vertx2, bool is_single_sided )
        : m_vertx0( vertx0 )
        , m_vertx1( vertx1 )
        , m_vertx2( vertx2 )
        , m_edge0( vertx1 - vertx0 )
        , m_edge1( vertx2 - vertx1 )
        , m_edge2( vertx0 - vertx2 )
        , m_normal( cross( vertx1 - vertx0, vertx2 - vertx0 ) ) // not normalized: area needed for baycentric coordinates
        , m_barycentric_intercpt( 0 )
    {
        m_dist_origin_to_plane = m_normal.dot( m_vertx0 );// precompute for performance, remember every point is in reality P_vec = P - 0
    }

    //~Polygon();

    const Vec3f& get_barycentric_intercept( ) const
    {
        return m_barycentric_intercpt;
    }
        
    bool intersects( const Rayf& ray, float& t ) override;

private:

    bool geometric_triangle_intersect( const Rayf& ray, float& t );

    Vec3f m_vertx0;
    Vec3f m_vertx1;
    Vec3f m_vertx2;

    Vec3f m_edge0;
    Vec3f m_edge1;
    Vec3f m_edge2;
    
    Vec3f m_normal;
    float m_dist_origin_to_plane;

    bool m_is_single_sided;

    Vec3f m_barycentric_intercpt;

    float m_epsilon = 1e-8;
};

