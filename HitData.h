#pragma once
#include <memory>
#include "RenderPrimitive.h"

class HitData
{
public:
    HitData() 
        : m_t( -1.0 ), m_t_closest( std::numeric_limits<float>::max() )
        , m_renderable_ptr( nullptr )
        , m_has_been_hit( false )
    {}

    HitData( const float t, std::shared_ptr<RenderPrimitive> ptr ) 
        : m_t( t )
        , m_t_closest( std::numeric_limits<float>::max() )
        , m_renderable_ptr( std::move( ptr ) )
        , m_has_been_hit( false )
    {}
    
    //~HitData();
    void update_closest_and_assign( const std::shared_ptr<RenderPrimitive>& ptr )
    {
        if ( m_t < m_t_closest )
        {
            m_t_closest = m_t;
            m_renderable_ptr = ptr;
            m_has_been_hit = true; // optimise, is done every time
        }
            
    }

    bool has_been_hit() const
    {
        return m_has_been_hit;
    }

    

    float m_t;
    float m_t_closest;
    std::shared_ptr<RenderPrimitive> m_renderable_ptr;
private:
    bool m_has_been_hit;
    
};

