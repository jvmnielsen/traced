#pragma once
#include "Ray.h"

class Material;

class Renderable
{
public:
    //virtual ~Renderable() = default;
    virtual bool is_hit_by( const Rayf& ray, float& t ) const = 0;

    Vec3f m_surface_color = { 150, 20, 100 };
};

class Plane
    : public Renderable
{
public:
    Plane( const Vec3f& normal, const Vec3f& point )
        : m_normal( normal )
        , m_point_on_plane( point )
    {}

    bool is_hit_by( const Rayf& ray, float& t ) const override
    {
        const auto denom = dot( m_normal, ray.direction() );
        
        if ( denom > 1e-6 )
        {
            const auto center_to_intercept = m_point_on_plane - ray.origin();

            t = dot( center_to_intercept, m_normal ) / denom;

            return ( t >= 0 );
        }

        return false;
    }

private:
    Vec3f m_normal;
    Vec3f m_point_on_plane;
};


class Disk
    : public Renderable
{
public:
    Disk( const Vec3f& normal, const Vec3f& point, const float radius )
        : m_normal( normal )
        , m_point_on_plane( point )
        , m_radius( radius )
        , m_radius_squared( radius * radius )
    {}

    bool is_hit_by( const Rayf& ray, float& t ) const override
    {

        // first part is, unsurprisingly, the same as for a disk
        // consider inheriting from disk

        const auto denom = dot( m_normal, ray.direction() );

        if ( !denom > 1e-6 )
            return false;

        const auto recenter = m_point_on_plane - ray.origin();

        t = dot( recenter, m_normal ) / denom;

        if ( t < 0 )
            return false;

        const auto supposed_intercept = ray.point_at_parameter( t );
        const auto center_to_intercept = supposed_intercept - m_point_on_plane;
        const auto difference = dot( center_to_intercept, center_to_intercept );

        return (difference <= m_radius_squared);
    }

private:
    Vec3f m_normal;
    Vec3f m_point_on_plane;
    float m_radius;
    float m_radius_squared;
};
 

