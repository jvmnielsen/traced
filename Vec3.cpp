#include "Vec3.h"
#include <math.h>

template< typename T >
T Vec3< T >::magnitudeSquared() const
{
    return m_x * m_x + m_y * m_y + m_z * m_z;
}

template< typename T >
T Vec3<T>::magnitude() const
{
    // sqrt returns a double, so we can only cast 
    // 'down' to float or int -- no loss in precision
    // from sqrt itself
    return (T)sqrt( m_x * m_x + m_y * m_y + m_z * m_z );
}

template< typename T >
Vec3<T> ::Vec3()
    : m_x( T( 0 ) )
    , m_y( T( 0 ) )
    , m_z( T( 0 ) )
{
}

template< typename T >
Vec3<T>::Vec3( T x, T y, T z )
    : m_x( x )
    , m_y( y )
    , m_z( z )
{
}

template<typename T >
Vec3<T> Vec3<T>::operator * ( const T factor ) const
{
    return Vec3<T>( m_x * factor, m_y * factor, m_z * factor );
}

template< typename T >
Vec3<T>& Vec3<T>::operator *= ( const T factor )
{
    m_x *= factor;
    m_y *= factor;
    m_z *= factor;
    return *this;
}

template< typename T >
Vec3<T> Vec3<T>::operator / ( const T factor ) const
{
    return Vec3( m_x / factor, m_y / factor, m_z / factor );
}

template< typename T >
Vec3<T>& Vec3<T>::operator /= ( const T factor )
{
    m_x /= factor;
    m_y /= factor;
    m_z /= factor;
    return *this;
}

template< typename T >
Vec3<T> Vec3<T>::operator + ( const Vec3<T>& vec2 ) const
{
    return Vec3<T>( m_x + vec2.m_x, m_y + vec2.m_y, m_z + vec2.m_z );
}

template< typename T >
Vec3<T>& Vec3<T>::operator += ( const Vec3& vec2 )
{
    m_x += vec2.m_x;
    m_y += vec2.m_y;
    m_z += vec2.m_z;
    return *this;
}

template< typename T >
Vec3<T> Vec3<T>::operator - ( const Vec3<T>& vec2 ) const
{
    return Vec3<T>( m_x - vec2.m_x, m_y - vec2.m_y, m_z - vec2.m_z );
}

template< typename T >
Vec3<T>& Vec3<T>::operator -= ( const Vec3<T>& vec2 )
{
    m_x -= vec2.m_x;
    m_y -= vec2.m_y;
    m_z -= vec2.m_z;
    return *this;
}

template< typename T >
Vec3<T> Vec3<T>::operator * ( const Vec3<T>& vec2 ) const
{
    return Vec3<T>( m_x * vec2.m_x, m_y * vec2.m_y, m_z * vec2.m_z );
}

template< typename T >
Vec3<T>& Vec3<T>::operator *= ( const Vec3<T>& vec2 )
{
    m_x *= vec2.m_x;
    m_y *= vec2.m_y;
    m_z *= vec2.m_z;
    return *this;
}

template< typename T >
Vec3<T> Vec3<T>::operator / ( const Vec3<T>& vec2 ) const
{
    return Vec3<T>( m_x / vec2.m_x, m_y / vec2.m_y, m_z / vec2.m_z );
}

template< typename T >
Vec3<T>& Vec3<T>::operator /= ( const Vec3<T>& vec2 )
{
    m_x /= vec2.m_x;
    m_y /= vec2.m_y;
    m_z /= vec2.m_z;
    return *this;
}

template< typename T >
T Vec3<T>::dotProduct( const Vec3<T>& vec2 ) const
{
    return m_x * vec2.m_x + m_y * vec2.m_y + m_z * vec2.m_z;
}

template< typename T >
Vec3<T>& Vec3<T>::crossProduct( const Vec3<T>& vec2 )
{
    m_x = m_y * vec2.m_z - m_z * vec2.m_y;
    m_y = m_z * vec2.m_x - m_x * vec2.m_z;
    m_z = m_x * vec2.m_y - m_y * vec2.m_x;
    return *this;
}

template< typename T >
void Vec3<T>::normalize()
{
    T length = this->magnitude();
    if (length > 0) // avoid division by 0
    {
        T invertedLength = 1 / length;
        m_x *= invertedLength;
        m_y *= invertedLength;
        m_z *= invertedLength;
    }
}

template<typename T>
const T Vec3<T>::operator [] ( const uint8_t i ) const
{
    return (&m_x)[i];
}

template<typename T>
T& Vec3<T>::operator [] ( const uint8_t i )
{
    return (&m_x)[i];
}

template class Vec3<int>;
template class Vec3<float>;
template class Vec3<double>;