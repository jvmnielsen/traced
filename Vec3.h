#pragma once
#include <iostream>

template<typename T>
class Vec3
{
public:
    T m_x, m_y, m_z;

    Vec3();
    Vec3( T val );
    Vec3( T x, T y, T z );

    T x() const { return m_x; }
    T y() const { return m_y; }
    T z() const { return m_z; }

    T r() const { return m_x; }
    T g() const { return m_y; }
    T b() const { return m_z; }

    T length_squared() const;
    T length() const;

    Vec3 operator *  ( const T factor ) const;
    Vec3& operator *= ( const T factor );

    Vec3 operator /  ( const T factor ) const;
    Vec3& operator /= ( const T factor );

    Vec3 operator + ( const Vec3& vec2 ) const;
    Vec3& operator += ( const Vec3& vec2 );

    Vec3 operator - ( const Vec3& vec2 ) const;
    Vec3& operator -= ( const Vec3& vec2 );

    Vec3 operator * ( const Vec3& vec2 ) const;
    Vec3& operator *= ( const Vec3& vec2 );

    Vec3 operator / ( const Vec3& vec2 ) const;
    Vec3& operator /= ( const Vec3& vec2 );

    T dot( const Vec3& vec2 ) const;
    //static const inline T dotProduct( const Vec3& vec1, const Vec3& vec2 );
    
    Vec3 cross( const Vec3& vec2 ) const;
    //static inline Vec3 crossProduct( const Vec3& vec1, const Vec3& vec2 );
    
    Vec3& normalize();

	T norm() const
	{
		return m_x * m_x + m_y * m_y + m_z * m_z;
	}

    // Accessors 
    T operator [] ( const uint8_t i ) const;
    T& operator [] ( const uint8_t i );


};

template< typename T >
inline T dot( const Vec3<T>& vec1, const Vec3<T>& vec2 )
{
    return vec1.m_x * vec2.m_x + vec1.m_y * vec2.m_y + vec1.m_z * vec2.m_z;
}

template< typename T >
Vec3<T> cross( const Vec3<T>& vec1, const Vec3<T>& vec2 ) 
{
    return Vec3<T>(
        vec1.m_y * vec2.m_z - vec1.m_z * vec2.m_y,
        vec1.m_z * vec2.m_x - vec1.m_x * vec2.m_z,
        vec1.m_x * vec2.m_y - vec1.m_y * vec2.m_x );
}

template< typename T >
inline std::istream& operator >> ( std::istream &is, Vec3<T> &vec )
{
    is >> vec.m_x >> vec.m_y >> vec.m_z;
    return is;
}

template< typename T >
inline std::ostream& operator >> ( std::ostream &os, Vec3<T> &vec )
{
    os << vec.m_x << vec.m_y << vec.m_z;
    return os;
}

template< typename T >
inline Vec3<T> unit_vector( Vec3<T> vec )
{
    return vec / vec.length();
}

template< typename T >
inline Vec3<T> operator * ( const float t, const Vec3<T> &vec )
{
    return Vec3<T>( t * vec.m_x, t*vec.m_y, t*vec.m_z );
}

typedef Vec3<float> Vec3f;
typedef Vec3<int> Vec3i;
typedef Vec3<double> Vec3d;

template<typename T>
Vec3<T> normalize(const Vec3<T>& vec)
{
    auto tmp = vec;
    auto length = tmp.length();

    if (length > 0)
    {
        auto inv_length = 1 / length;
        tmp.m_x *= inv_length;
        tmp.m_y *= inv_length;
        tmp.m_z *= inv_length;
    }
    return tmp;
}