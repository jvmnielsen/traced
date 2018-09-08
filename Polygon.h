#pragma once
#include "RenderPrimitive.h"
#include "Vec3.h"

class Polygon :
    public RenderPrimitive
{
public:
    Polygon( 
			const Vec3f& vertx0, 
			const Vec3f& vertx1, 
			const Vec3f& vertx2, 
			bool is_single_sided )
        : m_vertices { vertx0, vertx1, vertx2 }
        , m_vertx1( vertx1 )
        , m_vertx2( vertx2 )
        , m_edge0( vertx1 - vertx0 )
        , m_edge0_2( vertx2 - vertx0 )
        , m_edge1( vertx2 - vertx1 )
        , m_edge2( vertx0 - vertx2 )
        , m_normal( cross( vertx1 - vertx0, vertx2 - vertx0 ) ) // not normalized: area needed for baycentric coordinates
    {
        m_dist_origin_to_plane = m_normal.dot( m_vertx0 );// precompute for performance, remember every point is in reality P_vec = P - 0
    }

    //~Polygon();

        
    bool intersects(const Rayf& ray, float& t, Vec3f& intercpt_coord) override;

	void transform_object_to_world(const Matrix44f& object_to_world) override;

	/*
    Vec3f get_surface_color( const Vec3f& coordinates ) const override
    {
        Vec3f color_profile[3]{ {0.6f, 0.4f, 0.1f}, {0.1f, 0.5f, 0.3f}, {0.1f, 0.3f, 0.7f} };
        auto color{ coordinates.m_x * color_profile[0] + coordinates[1] * coordinates.m_y + coordinates.m_z * color_profile[2] };
        for (auto i = 0; i < 3; ++i)
        {
            color[i] *= 255;
        }
        return color;
    } */

private:

    bool geometric_triangle_intersect( const Rayf& ray, float& t, Vec3f& intercpt_coord ) const;
    bool moller_trumbore_intersect( const Rayf& ray, float& t, Vec3f& intercpt_coord ) const;

	Vec3f m_vertices[3];
    Vec3f m_vertx0;
    Vec3f m_vertx1;
    Vec3f m_vertx2;

    Vec3f m_edge0;
    Vec3f m_edge0_2; // from vertx 0 to vertx 2. used for normal
    Vec3f m_edge1;
    Vec3f m_edge2;
    
    Vec3f m_normal;
    float m_dist_origin_to_plane;

    bool m_is_single_sided;

    float m_epsilon = 1e-8;
};

