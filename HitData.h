#pragma once
#include <memory>
#include "Renderable.h"

class Renderable;

class HitData
{
public:
    HitData() 
        : m_t( -1 ), m_t_closest( std::numeric_limits<float>::max() )
        , m_closest_ptr( nullptr )
        , m_coord( 0 )
        , m_has_been_hit( false )
    {}

    HitData( const float t, std::shared_ptr<Renderable> ptr ) 
        : m_t( t )
        , m_t_closest( std::numeric_limits<float>::max() )
        , m_closest_ptr( std::move( ptr ) )
        , m_coord( 0 )
        , m_has_been_hit( false )
    {}
    
    //~HitData();
    void update_closest(const std::shared_ptr<Renderable>& ptr, const Rayf& ray)
    {
        if (m_t < m_t_closest && m_t > 0)
        {
			m_barycentric_closest = m_barycentric_coord;
            m_t_closest = m_t;
			m_coord = ray.point_at_parameter(m_t_closest);
            m_closest_ptr = ptr;
            m_has_been_hit = true; // optimise, is done every time
        }
            
    }

    bool has_been_hit() const
    {
        return m_has_been_hit;
    }

	Vec3f coord() const
    {
		return m_coord;
    }

	Vec3f barycentric() const
    {
		return m_barycentric_closest; 
    }

    float m_t;
    float m_t_closest;
	//float m_t_closest_global;

    std::shared_ptr<Renderable> m_closest_ptr;
	//std::shared_ptr<Renderable> m_mesh_ptr;
	//std::shared_ptr<Renderable> m_recent_sub_ptr;
	//std::shared_ptr<Renderable> m_recent_global_ptr;

    Vec3f m_coord;
    //Vec3f m_coord_closest;

	Vec3f m_barycentric_coord;
	Vec3f m_barycentric_closest;

private:
    bool m_has_been_hit;
    
};

