#pragma once
#include "Hitable.h"
class Sphere :
    public Hitable
{
public:
    Sphere();

    Sphere( Vec3f center, float radius ) 
        : m_center( center ) 
        , m_radius( radius )
    {}
    
    virtual bool hit( const Rayf& ray, const float tmin, const float tmax, hit_record& record ) const;

private:
    Vec3f m_center;
    float m_radius;
};

