#pragma once
#include "Renderable.h"
#include "Vec3.h"

class Polygon :
    public Renderable
{
public:
    Polygon( 
			const Vec3f& vertx0, 
			const Vec3f& vertx1, 
			const Vec3f& vertx2, 
			bool is_single_sided )
        : m_vertices { vertx0, vertx1, vertx2 }
		//, m_vertices0(vertx0)
        //, m_vertices1( vertx1 )
        //, m_vertices2( vertx2 )
        , m_edge0( vertx1 - vertx0 )
        , m_edge0_2( vertx2 - vertx0 )
        , m_edge1( vertx2 - vertx1 )
        , m_edge2( vertx0 - vertx2 )
        , m_normal(cross(vertx1 - vertx0, vertx2 - vertx0).normalize()) // not normalized: area needed for baycentric coordinates
    {
        m_dist_origin_to_plane = m_normal.dot( m_vertices[0] );// precompute for performance, remember every point is in reality P_vec = P - 0
    }

    Polygon(
        const Vec3f& vertx0,
        const Vec3f& vertx1,
        const Vec3f& vertx2,
        const Vec3f& vertx_normal0,
        const Vec3f& vertx_normal1,
        const Vec3f& vertx_normal2,
        bool is_single_sided )
        : m_vertices{ vertx0, vertx1, vertx2 }
        , m_vertex_normals{ vertx_normal0, vertx_normal1, vertx_normal2 }
        , m_edge0( vertx1 - vertx0 )
        , m_edge0_2( vertx2 - vertx0 )
        , m_edge1( vertx2 - vertx1 )
        , m_edge2( vertx0 - vertx2 )
        , m_normal( cross( vertx1 - vertx0, vertx2 - vertx0 ).normalize() ) // not normalized: area needed for baycentric coordinates
    {
        m_dist_origin_to_plane = m_normal.dot( m_vertices[0] );// precompute for performance, remember every point is in reality P_vec = P - 0
    }

    //~Polygon();

        
    bool intersects(const Rayf& ray, HitData& hit_data) override;

	void transform_object_to_world(const Matrix44f& object_to_world) override;

	void update_edges();

	
	Vec3f get_surface_properties(HitData& hit_data) const override;

    Vec3f m_vertices[3];

    Vec3f m_vertex_normals[3];

private:

    bool geometric_triangle_intersect(const Rayf& ray, HitData& hit_data) const;
    bool moller_trumbore_intersect(const Rayf& ray, HitData& hit_data) const;

	
    //Vec3f m_vertices0;
    //Vec3f m_vertices1;
    //Vec3f m_vertices2;

    

    Vec3f m_edge0;
    Vec3f m_edge0_2; // from vertx 0 to vertx 2. used for normal
    Vec3f m_edge1;
    Vec3f m_edge2;
    
    Vec3f m_normal;
    float m_dist_origin_to_plane;

    bool m_is_single_sided;

    float m_epsilon = 1e-8;
};

