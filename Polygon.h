#pragma once
#include "RenderPrimitive.h"
#include "Vec3.h"

class Polygon :
    public RenderPrimitive
{
public:
    Polygon( const Vec3f& point0, const Vec3f& point1, const Vec3f& point2, bool is_single_sided )
        : m_point0( point0 )
        , m_point1( point1 )
        , m_point2( point2 )
        , m_vertx0( point1 - point0 )
        , m_vertx1( point2 - point1 )
        , m_vertx2( point0 - point2 )
        , m_normal( cross( point1 - point0, point2 - point0 ).normalize() ) // no need to normalise
    {
        m_dist_origin_to_plane = m_normal.dot( m_point0 );// precompute for performance, remember every point is in reality P_vec = P - 0
    }

    //~Polygon();
        
    bool intersects( const Rayf& ray, float& t ) const override;

private:

    bool geometric_triangle_intersect( const Rayf& ray, float& t ) const;

    Vec3f m_point0;
    Vec3f m_point1;
    Vec3f m_point2;

    Vec3f m_vertx0;
    Vec3f m_vertx1;
    Vec3f m_vertx2;
    
    Vec3f m_normal;
    float m_dist_origin_to_plane;

    bool m_is_single_sided;

    float m_epsilon = 1e-8;
};

