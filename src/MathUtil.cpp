#include "MathUtil.h"
#include <math.h>
#include <algorithm>



template<typename T>
Point<T>::Point(T val)
	: x(val)
	, y(val)
	, z(val)
{
}

template<typename T>
Point<T> ::Point()
	: x(T(0))
	, y(T(0))
	, z(T(0))
{
}

template<typename T>
Point<T>::Point(T x, T y, T z)
	: x(x)
	, y(y)
	, z(z)
{
}

template<typename T>
Point<T> Point<T>::operator + (const Point<T>& other) const
{
	return Vec<T>(x + other.x, y + other.y, z + other.z);
}

template<typename T>
Point<T>& Point<T>::operator += (const Point<T>& other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}

template<typename T>
Point<T> Point<T>::operator - (const Point<T>& other) const
{
	return Vec<T>(x - other.x, y - other.y, z - other.z);
}

template<typename T>
Point<T>& Point<T>::operator -= (const Point<T>& other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	return *this;
}

template class Point<int>;
template class Point<float>;
template class Point<double>;
/*
template<typename T>
T Vec3<T>::MagnitudeSquared() const
{
    return x * x + y * y + z * z;
}

template<typename T>
T Vec3<T>::Magnitude() const
{
    // sqrt returns a double, so we can only cast 
    // 'down' to float or int -- no loss in precision
    // from sqrt itself
    return (T)sqrt( x * x + y * y + z * z );
}

template<typename T>
Vec3<T>::Vec3(T val)
    : x(val)
    , y(val)
    , z(val)
{
}

template<typename T>
Vec3<T> ::Vec3()
    : x(T(0))
    , y(T(0))
    , z(T(0))
{
}

template<typename T>
Vec3<T>::Vec3(T x, T y, T z)
    : x(x)
    , y(y)
    , z(z)
{
}

template<typename T>
Vec3<T> Vec3<T>::operator * (const T factor) const
{
    return Vec3<T>(x * factor, y * factor, z * factor);
}

template<typename T>
Vec3<T>& Vec3<T>::operator *= (const T factor)
{
    x *= factor;
    y *= factor;
    z *= factor;
    return *this;
}

template<typename T>
Vec3<T> Vec3<T>::operator / (const T factor) const
{
    return Vec(x / factor, y / factor, z / factor);
}

template<typename T>
Vec3<T>& Vec3<T>::operator /= (const T factor)
{
    x /= factor;
    y /= factor;
    z /= factor;
    return *this;
}

template<typename T>
Vec3<T> Vec3<T>::operator + (const Vec<T>& vec2) const
{
    return Vec<T>(x + vec2.x, y + vec2.y, z + vec2.z);
}

template<typename T>
Vec3<T>& Vec3<T>::operator += (const Vec& vec2)
{
    x += vec2.x;
    y += vec2.y;
    z += vec2.z;
    return *this;
}

template< typename T >
Vec3<T> Vec3<T>::operator - (const Vec<T>& vec2) const
{
    return Vec3<T>(x - vec2.x, y - vec2.y, z - vec2.z);
}

template<typename T>
Vec3<T>& Vec3<T>::operator -= (const Vec<T>& vec2)
{
    x -= vec2.x;
    y -= vec2.y;
    z -= vec2.z;
    return *this;
}

template< typename T >
Vec3<T> Vec3<T>::operator * (const Vec<T>& vec2) const
{
    return Vec3<T>(x * vec2.x, y * vec2.y, z * vec2.z);
}

template<typename T>
Vec3<T>& Vec3<T>::operator *= (const Vec3<T>& vec2)
{
    x *= vec2.x;
    y *= vec2.y;
    z *= vec2.z;
    return *this;
}

template<typename T>
Vec3<T> Vec3<T>::operator / (const Vec3<T>& vec2) const
{
    return Vec3<T>( x / vec2.x, y / vec2.y, z / vec2.z );
}

template< typename T >
Vec3<T>& Vec3<T>::operator /= ( const Vec3<T>& vec2 )
{
    x /= vec2.x;
    y /= vec2.y;
    z /= vec2.z;
    return *this;
}

template<typename T>
T Vec3<T>::DotProduct(const Vec3<T>& vec2) const
{
    return x * vec2.x + y * vec2.y + z * vec2.z;
}

template< typename T >
Vec3<T> Vec3<T>::CrossProduct(const Vec3<T>& vec2) const
{
    
    return Vec3<T>{
        y * vec2.z - z * vec2.y,
        z * vec2.x - x * vec2.z,
        x * vec2.y - y * vec2.x };
}

template<typename T>
Vec3<T>& Vec3<T>::Normalize()
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
T Vec3<T>::operator [] (const uint8_t i) const
{
    return (&x)[i];
}

template<typename T>
T& Vec3<T>::operator [] (const uint8_t i)
{
    return (&x)[i];
}

template class Vec3<int>;
template class Vec3<float>;
template class Vec3<double>; */