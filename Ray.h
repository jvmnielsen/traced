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
        //, m_inv_dir(1 / direction)
    {
        /*
        m_sign[0] = m_inv_dir.x() < 0;
        m_sign[1] = m_inv_dir.y() < 0;
        m_sign[2] = m_inv_dir.z() < 0; */
    }

    Vec3<T> origin() const { return m_origin; }
    Vec3<T> direction() const { return m_direction; }
    Vec3<T> point_at_parameter( const float t ) const { return m_origin + m_direction * t; }

    int m_sign[3]; // used for AABB optimisation
    Vec3f m_inv_dir;
    //~Ray();

private:
    Vec3<T> m_origin;
    Vec3<T> m_direction;
    
    
};

typedef Ray<float> Rayf;
typedef Ray<int> Rayi;
typedef Ray<double> Rayd;

