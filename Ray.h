#pragma once
#include "Vec3.h"

template< typename T >
class Ray
{
public:
    Ray() {}
    Ray( const Vec3<T>& origin,
         const Vec3<T>& direction )
         : m_origin( origin )
         , m_direction( direction )
    {}

    Vec3<T> origin() const { return m_origin; }
    Vec3<T> direction() const { return m_direction; }
    Vec3<T> point_at_parameter( const float t ) const { return m_origin + m_direction * t; }

    //~Ray();

private:
    Vec3<T> m_origin;
    Vec3<T> m_direction;

    
};

typedef Ray<float> Rayf;
typedef Ray<int> Rayi;
typedef Ray<double> Rayd;

