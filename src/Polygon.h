#pragma once
#include "Renderable.h"
#include "MathUtil.h"

class Polygon :
    public Renderable
{
public:
    Polygon( 
			const Vecf& vertx0, 
			const Vecf& vertx1, 
			const Vecf& vertx2, 
			bool is_single_sided,
			const Vecf& albedo,
            MaterialType material)
        : Renderable(albedo, material)
		, m_vertices { vertx0, vertx1, vertx2 }
		//, m_vertices0(vertx0)
        //, m_vertices1( vertx1 )
        //, m_vertices2( vertx2 )
        , m_edge0(vertx1 - vertx0)
        , m_edge0_2(vertx2 - vertx0)
        , m_edge1(vertx2 - vertx1)
        , m_edge2(vertx0 - vertx2)
		, m_is_single_sided(is_single_sided)
        , m_normal(CrossProduct(vertx1 - vertx0, vertx2 - vertx0).Normalize()) // not normalized: area needed for baycentric coordinates
    {
        m_dist_origin_to_plane = m_normal.DotProduct(m_vertices[0]);// precompute for performance, remember every point is in reality P_vec = P - 0
    }

    Polygon(
        const Vecf& vertx0,
        const Vecf& vertx1,
        const Vecf& vertx2,
        const Vecf& vertx_normal0,
        const Vecf& vertx_normal1,
        const Vecf& vertx_normal2,
        bool is_single_sided,
		const Vecf& albedo,
        MaterialType material)
        : Renderable(albedo, material)
		, m_vertices{ vertx0, vertx1, vertx2 }
        , m_vertex_normals{ vertx_normal0, vertx_normal1, vertx_normal2 }
        , m_edge0( vertx1 - vertx0 )
        , m_edge0_2( vertx2 - vertx0 )
        , m_edge1( vertx2 - vertx1 )
        , m_edge2( vertx0 - vertx2 )
		, m_is_single_sided(is_single_sided)
        , m_normal(CrossProduct( vertx1 - vertx0, vertx2 - vertx0 ).Normalize() ) // not normalized: area needed for baycentric coordinates
    {
        m_dist_origin_to_plane = m_normal.DotProduct( m_vertices[0] );// precompute for performance, remember every point is in reality P_vec = P - 0
    }

    //~Polygon();

        
    bool intersects(const Rayf& ray, HitData& hit_data) override;

	void transform_object_to_world(const Matrix44f& object_to_world) override;

	void update_edges();


    void set_normal(HitData& hit_data) const override;

    Vecf m_vertices[3];

    Vecf m_vertex_normals[3];

private:

    bool geometric_triangle_intersect(const Rayf& ray, HitData& hit_data) const;
    bool moller_trumbore_intersect(const Rayf& ray, HitData& hit_data) const;

	
    //Vecf m_vertices0;
    //Vecf m_vertices1;
    //Vecf m_vertices2;

    

    Vecf m_edge0;
    Vecf m_edge0_2; // from vertx 0 to vertx 2. used for normal
    Vecf m_edge1;
    Vecf m_edge2;
    
    Vecf m_normal;
    float m_dist_origin_to_plane;

    bool m_is_single_sided;

    float m_epsilon = 1e-8;
};

