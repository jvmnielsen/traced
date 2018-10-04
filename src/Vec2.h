#pragma once

#pragma once
#include <iostream>


template<typename T>
class Vec2
{
public:
    Vec2() : m_x( T( 0 ) ), m_y( T( 0 ) ) {}
    Vec2( T val ) : m_x( val ), m_y( val ) {}
    Vec2( T x, T y ) : m_x( x ), m_y( y ) {}

    Vec2 operator + ( const Vec2& v ) const
    {
        return Vec2( x + v.m_x, y + v.m_y );
    }

    Vec2 operator / ( const T f ) const
    {
        return Vec2( m_x / f, m_y / f );
    }

    Vec2 operator * ( const T f ) const
    {
        return Vec2( m_x * f, m_y * f );
    }

    Vec2& operator /= ( const T f )
    {
        m_x /= f;
        m_y /= f;
        return *this;
    }

    Vec2& operator *= ( const T f )
    {
        m_x *= f;
        m_y *= f;
        return *this;
    }


    T m_x, m_y;
};

template< typename T >
Vec2<T> operator * ( const T f, const Vec2<T> &vec )
{
    return Vec2<T>( vec.m_x * f, vec.m_y * f );
}

typedef Vec2<float> Vec2f;
typedef Vec2<double> Vec2d;