#pragma once

#include <cstdint>

template<typename T>
class Vec3
{
public:
    T m_x, m_y, m_z;

    Vec3();
    Vec3( T x, T y, T z );

    inline T magnitudeSquared() const;
    inline T magnitude() const;

    inline Vec3 operator *  ( const T factor ) const;
    inline Vec3& operator *= ( const T factor );

    inline Vec3 operator /  ( const T factor ) const;
    inline Vec3& operator /= ( const T factor );

    inline Vec3 operator + ( const Vec3& vec2 ) const;
    inline Vec3& operator += ( const Vec3& vec2 );

    inline Vec3 operator - ( const Vec3& vec2 ) const;
    inline Vec3& operator -= ( const Vec3& vec2 );

    inline Vec3 operator * ( const Vec3& vec2 ) const;
    inline Vec3& operator *= ( const Vec3& vec2 );

    inline Vec3 operator / ( const Vec3& vec2 ) const;
    inline Vec3& operator /= ( const Vec3& vec2 );

    inline T dotProduct( const Vec3& vec2 ) const;
    //static const inline T dotProduct( const Vec3& vec1, const Vec3& vec2 );
    
    inline Vec3& crossProduct( const Vec3& vec2 );
    //static inline Vec3 crossProduct( const Vec3& vec1, const Vec3& vec2 );
    
    inline void normalize();

    // Accessors 
    const T operator [] ( const uint8_t i ) const;
    T& operator [] ( const uint8_t i );


};

template< typename T >
inline T dotProduct( const Vec3<T>& vec1, const Vec3<T>& vec2 )
{
    return vec1.m_x * vec2.m_x + vec1.m_y * vec2.m_y + vec1.m_z * vec2.m_z;
}

template< typename T >
Vec3<T> crossProduct( const Vec3<T>& vec1, const Vec3<T>& vec2 ) 
{
    return Vec3<T>(
        vec1.m_y * vec2.m_z - vec1.m_z * vec2.m_y,
        vec1.m_z * vec2.m_x - vec1.m_x * vec2.m_z,
        vec1.m_x * vec2.m_y - vec1.m_y * vec2.m_x );
}


typedef Vec3<float> Vec3f;
typedef Vec3<int> Vec3i;
typedef Vec3<double> Vec3d;

