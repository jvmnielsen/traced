#pragma once
#include <memory>
#include "RenderPrimitive.h"

class RenderPrimitive;

class HitData
{
public:
    HitData() 
        : m_t( -1.0 ), m_t_closest( std::numeric_limits<float>::max() )
        , m_closest_ptr( nullptr )
        , m_coordinates( 0 )
        , m_has_been_hit( false )
    {}

    HitData( const float t, std::shared_ptr<RenderPrimitive> ptr ) 
        : m_t( t )
        , m_t_closest( std::numeric_limits<float>::max() )
        , m_closest_ptr( std::move( ptr ) )
        , m_coordinates( 0 )
        , m_has_been_hit( false )
    {}
    
    //~HitData();
    void update_closest( const std::shared_ptr<RenderPrimitive>& ptr )
    {
        if (m_t < m_t_closest)
        {
            m_t_closest = m_t;
            m_closest_ptr = ptr;
            m_has_been_hit = true; // optimise, is done every time
        }
            
    }

	/*
	void update_closest_global(const std::shared_ptr<RenderPrimitive>& ptr)
	{
		m_recent_global_ptr = ptr;

		if (m_t_closest_sub < m_t_closest_global)
		{
			m_t_closest_global = m_t_closest_sub;
			m_closest_global_ptr = m_recent_sub_ptr;
		}

	} */

    bool has_been_hit() const
    {
        return m_has_been_hit;
    }

    
    
    float m_t;
    float m_t_closest;
	float m_t_closest_global;

    std::shared_ptr<RenderPrimitive> m_closest_ptr;
	//std::shared_ptr<RenderPrimitive> m_closest_global_ptr;
	//std::shared_ptr<RenderPrimitive> m_recent_sub_ptr;
	//std::shared_ptr<RenderPrimitive> m_recent_global_ptr;

    Vec3f m_coordinates;
private:
    bool m_has_been_hit;
    
};

