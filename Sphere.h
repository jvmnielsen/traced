#pragma once
#include "RenderPrimitive.h"

class Sphere :
    public RenderPrimitive
{
public:
    Sphere( Vec3f& center, const float radius, Material* material ) 
        : m_center( center ) 
        , m_radius( radius )
        , m_radius_squared( radius * radius )
		, m_material( material )
    {}

    bool intersects(const Rayf& ray, float& t) const override;

    bool solve_quadratic( const float a, const float b, const float c, float& solu_one, float& solu_two ) const;

private:
    Vec3f m_center;
    float m_radius;
    float m_radius_squared;
	Material* m_material;
    
};

