#include "MathUtil.h"
#include <math.h>

template< typename T >
T Vec< T >::MagnitudeSquared() const
{
    return x * x + y * y + z * z;
}

template< typename T >
T Vec<T>::Magnitude() const
{
    // sqrt returns a double, so we can only cast 
    // 'down' to float or int -- no loss in precision
    // from sqrt itself
    return (T)sqrt( x * x + y * y + z * z );
}

template< typename T >
Vec<T>::Vec( T val )
    : x( val )
    , y( val )
    , z( val )
{
}

template< typename T >
Vec<T> ::Vec()
    : x( T( 0 ) )
    , y( T( 0 ) )
    , z( T( 0 ) )
{
}

template< typename T >
Vec<T>::Vec( T x, T y, T z )
    : x( x )
    , y( y )
    , z( z )
{
}

template<typename T >
Vec<T> Vec<T>::operator * ( const T factor ) const
{
    return Vec<T>( x * factor, y * factor, z * factor );
}

template< typename T >
Vec<T>& Vec<T>::operator *= ( const T factor )
{
    x *= factor;
    y *= factor;
    z *= factor;
    return *this;
}

template< typename T >
Vec<T> Vec<T>::operator / ( const T factor ) const
{
    return Vec( x / factor, y / factor, z / factor );
}

template< typename T >
Vec<T>& Vec<T>::operator /= ( const T factor )
{
    x /= factor;
    y /= factor;
    z /= factor;
    return *this;
}

template< typename T >
Vec<T> Vec<T>::operator + ( const Vec<T>& vec2 ) const
{
    return Vec<T>( x + vec2.x, y + vec2.y, z + vec2.z );
}

template< typename T >
Vec<T>& Vec<T>::operator += ( const Vec& vec2 )
{
    x += vec2.x;
    y += vec2.y;
    z += vec2.z;
    return *this;
}

template< typename T >
Vec<T> Vec<T>::operator - ( const Vec<T>& vec2 ) const
{
    return Vec<T>( x - vec2.x, y - vec2.y, z - vec2.z );
}

template< typename T >
Vec<T>& Vec<T>::operator -= ( const Vec<T>& vec2 )
{
    x -= vec2.x;
    y -= vec2.y;
    z -= vec2.z;
    return *this;
}

template< typename T >
Vec<T> Vec<T>::operator * ( const Vec<T>& vec2 ) const
{
    return Vec<T>( x * vec2.x, y * vec2.y, z * vec2.z );
}

template< typename T >
Vec<T>& Vec<T>::operator *= ( const Vec<T>& vec2 )
{
    x *= vec2.x;
    y *= vec2.y;
    z *= vec2.z;
    return *this;
}

template< typename T >
Vec<T> Vec<T>::operator / ( const Vec<T>& vec2 ) const
{
    return Vec<T>( x / vec2.x, y / vec2.y, z / vec2.z );
}

template< typename T >
Vec<T>& Vec<T>::operator /= ( const Vec<T>& vec2 )
{
    x /= vec2.x;
    y /= vec2.y;
    z /= vec2.z;
    return *this;
}

template< typename T >
T Vec<T>::DotProduct( const Vec<T>& vec2 ) const
{
    return x * vec2.x + y * vec2.y + z * vec2.z;
}

template< typename T >
Vec<T> Vec<T>::CrossProduct( const Vec<T>& vec2 ) const
{
    
    return Vec<T>{
        y * vec2.z - z * vec2.y,
        z * vec2.x - x * vec2.z,
        x * vec2.y - y * vec2.x };
}

template< typename T >
Vec<T>& Vec<T>::Normalize()
{
    T length = this->Magnitude();
    if (length > 0) // avoid division by 0
    {
        T invertedLength = 1 / length;
        x *= invertedLength;
        y *= invertedLength;
        z *= invertedLength;
    }

    return *this;
}

template<typename T>
T Vec<T>::operator [] ( const uint8_t i ) const
{
    return (&x)[i];
}

template<typename T>
T& Vec<T>::operator [] ( const uint8_t i )
{
    return (&x)[i];
}

template class Vec<int>;
template class Vec<float>;
template class Vec<double>;