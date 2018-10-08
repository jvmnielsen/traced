#pragma once
#include <iostream>
#include <vector>

#define _USE_MATH_DEFINES
#include <math.h>

inline float DegToRad(const float deg)
{
    return (float)(deg * M_PI / 180);
}

template<typename T>
struct Point
{
	T x, y, z;

	Point();
	explicit Point(T val);
	Point(T x_, T y_, T z_);

	Point operator + (const Point& other) const;
	Point& operator += (const Point& other);

	Point operator - (const Point& other) const;
	Point& operator -= (const Point& other);
};

typedef Point<float> Pointf;
typedef Point<int> Pointi;
typedef Point<double> Pointd;

// ---------------------------------------------------------------------------
template<typename T>
struct Vec3
{
    T x, y, z;

    Vec3();
    explicit Vec3(T val);
    Vec3(T x_, T y_, T z_);

	T& r() { return x; }
	T& g() { return y; }
	T& b() { return z; }

    T MagnitudeSquared() const;
    T Magnitude() const;

    Vec3 operator *  (const T factor) const;
    Vec3& operator *= (const T factor);

    Vec3 operator /  (const T factor) const;
    Vec3& operator /= (const T factor);

    Vec3 operator + (const Vec3& other) const;
    Vec3& operator += (const Vec3& other);

    Vec3 operator - (const Vec3& other) const;
    Vec3& operator -= (const Vec3& other);

    Vec3 operator * (const Vec3& other) const;
    Vec3& operator *= (const Vec3& other);

    Vec3 operator / (const Vec3& other) const;
    Vec3& operator /= (const Vec3& other);

    T DotProduct(const Vec3& other) const;
    
    Vec3 CrossProduct(const Vec3& other) const;
    
    Vec3& Normalize();

    // Accessors 
    T operator [] (const uint8_t i) const;
    T& operator [] (const uint8_t i);


};

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
	return (T)sqrt(x * x + y * y + z * z);
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
	return Vec3(x / factor, y / factor, z / factor);
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
Vec3<T> Vec3<T>::operator + (const Vec3<T>& vec2) const
{
	return Vec3<T>(x + vec2.x, y + vec2.y, z + vec2.z);
}

template<typename T>
Vec3<T>& Vec3<T>::operator += (const Vec3<T>& vec2)
{
	x += vec2.x;
	y += vec2.y;
	z += vec2.z;
	return *this;
}

template< typename T >
Vec3<T> Vec3<T>::operator - (const Vec3<T>& vec2) const
{
	return Vec3<T>(x - vec2.x, y - vec2.y, z - vec2.z);
}

template<typename T>
Vec3<T>& Vec3<T>::operator -= (const Vec3<T>& vec2)
{
	x -= vec2.x;
	y -= vec2.y;
	z -= vec2.z;
	return *this;
}

template< typename T >
Vec3<T> Vec3<T>::operator * (const Vec3<T>& vec2) const
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
	return Vec3<T>(x / vec2.x, y / vec2.y, z / vec2.z);
}

template< typename T >
Vec3<T>& Vec3<T>::operator /= (const Vec3<T>& vec2)
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

// ---------------------------------------------------------------------------
template<typename T>
inline T DotProduct(const Vec3<T>& vec1, const Vec3<T>& vec2)
{
    return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
}
// ---------------------------------------------------------------------------
template< typename T >
Vec3<T> CrossProduct(const Vec3<T>& vec1, const Vec3<T>& vec2) 
{
    return Vec3<T>(
        vec1.y * vec2.z - vec1.z * vec2.y,
        vec1.z * vec2.x - vec1.x * vec2.z,
        vec1.x * vec2.y - vec1.y * vec2.x );
}
// ---------------------------------------------------------------------------
template<typename T>
inline std::istream& operator >> (std::istream &is, Vec3<T> &vec)
{
    is >> vec.x >> vec.y >> vec.z;
    return is;
}
// ---------------------------------------------------------------------------
template<typename T>
inline std::ostream& operator << (std::ostream &os, Vec3<T> &vec)
{
    os << vec.x << vec.y << vec.z;
    return os;
}
// ---------------------------------------------------------------------------
template< typename T >
inline Vec3<T> UnitVector(Vec3<T> vec)
{
    return vec / vec.Magnitude();
}
// ---------------------------------------------------------------------------
template< typename T >
inline Vec3<T> operator * (const float t, const Vec3<T> &vec)
{
    return Vec3<T>(t * vec.x, t*vec.y, t*vec.z);
}
// ---------------------------------------------------------------------------


template<typename T>
Vec3<T> Normalize(const Vec3<T>& vec)
{
    auto tmp = vec;
    auto length = tmp.Magnitude();

    if (length > 0)
    {
        auto inv_length = 1 / length;
        tmp.x *= inv_length;
        tmp.y *= inv_length;
        tmp.z *= inv_length;
    }
    return tmp;
}
// ---------------------------------------------------------------------------
typedef Vec3<float> Vecf;
typedef Vec3<int> Veci;
typedef Vec3<double> Vecd;
// ---------------------------------------------------------------------------
template<typename T>
class Matrix44
{
public:
    T m_arr[4][4] = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};

    Matrix44() = default;

    ~Matrix44()
    {
        //delete m_arr;
    }

    Matrix44(T a, T b, T c, T d, T e, T f, T g, T h,
             T i, T j, T k, T l, T m, T n, T o, T p)
    {
        m_arr[0][0] = a;
        m_arr[0][1] = b;
        m_arr[0][2] = c;
        m_arr[0][3] = d;
        m_arr[1][0] = e;
        m_arr[1][1] = f;
        m_arr[1][2] = g;
        m_arr[1][3] = h;
        m_arr[2][0] = i;
        m_arr[2][1] = j;
        m_arr[2][2] = k;
        m_arr[2][3] = l;
        m_arr[3][0] = m;
        m_arr[3][1] = n;
        m_arr[3][2] = o;
        m_arr[3][3] = p;
    }

    static void multiply(const Matrix44& a, const Matrix44& b, Matrix44& c)
    {
        // rolled up version rather than writing out the arguments
        for (uint8_t i = 0; i < 4; ++i) {
            for (uint8_t j = 0; j < 4; ++j) {
                c[i][j] = a[i][0] * b[0][j] + a[i][1] * b[1][j] +
                    a[i][2] * b[2][j] + a[i][3] * b[3][j];
            }
        }
    }

    Matrix44 operator * (const Matrix44& m) const
    {
        Matrix44 tmp;
        multiply(*this, m, tmp);
        return tmp;
    }

    // multiply point and matrix
    template< typename S >
    Vec3<S> multiply_with_point(const Vec3<S>& src) const
    {
        S a = src[0] * m_arr[0][0] + src[1] * m_arr[1][0] + src[2] * m_arr[2][0] + m_arr[3][0];
        S b = src[0] * m_arr[0][1] + src[1] * m_arr[1][1] + src[2] * m_arr[2][1] + m_arr[3][1];
        S c = src[0] * m_arr[0][2] + src[1] * m_arr[1][2] + src[2] * m_arr[2][2] + m_arr[3][2];
        S w = src[0] * m_arr[0][3] + src[1] * m_arr[1][3] + src[2] * m_arr[2][3] + m_arr[3][3];

        Vec3<S> tmp;

        return  tmp = {a / w, b / w, c / w};
    }

    // multiply direction and matrix (don't divide coefficients)
    template< typename S >
    Vec3<S> multiply_with_dir(const Vec3<S>& src) const
    {
        S a = src[0] * m_arr[0][0] + src[1] * m_arr[1][0] + src[2] * m_arr[2][0] + m_arr[3][0];
        S b = src[0] * m_arr[0][1] + src[1] * m_arr[1][1] + src[2] * m_arr[2][1] + m_arr[3][1];
        S c = src[0] * m_arr[0][2] + src[1] * m_arr[1][2] + src[2] * m_arr[2][2] + m_arr[3][2];
        S w = src[0] * m_arr[0][3] + src[1] * m_arr[1][3] + src[2] * m_arr[2][3] + m_arr[3][3];

        Vec3<S> tmp;

        return tmp = {a, b, c};
    }

    Matrix44 inverse() const
    {
        int i, j, k;
        Matrix44 s;
        Matrix44 t = *this;

        for (i = 0; i < 3; i++)
        {
            auto pivot = i;

            T pivotsize = t.m_arr[i][i];

            if (pivotsize < 0)
            {
                pivotsize = -pivotsize;
            }

            for (j = i + 1; j < 4; j++)
            {
                T tmp = t.m_arr[j][i];

                if (tmp < 0)
                {
                    tmp = -tmp;
                }

                if (tmp > pivotsize)
                {
                    pivot = j;
                    pivotsize = tmp;
                }
            }

            if (pivotsize == 0) // cannot invert
            {
                return Matrix44();
            }

            if (pivot != i)
            {
                for (j = 0; j < 4; j++)
                {
                    T tmp = t.m_arr[i][j];
                    t.m_arr[i][j] = t.m_arr[pivot][j];
                    t.m_arr[pivot][j] = tmp;

                    tmp = s.m_arr[i][j];
                    s.m_arr[i][j] = s.m_arr[pivot][j];
                    s.m_arr[pivot][j] = tmp;
                }
            }

            for (j = i + 1; j < 4; j++)
            {
                T f = t.m_arr[j][i] / t.m_arr[i][i];

                for (k = 0; k < 4; k++)
                {
                    t.m_arr[j][k] -= f * t.m_arr[i][k];
                    s.m_arr[j][k] -= f * s.m_arr[i][k];
                }
            }
        }

        // Backward substitution
        for (i = 3; i >= 0; --i)
        {
            T f;

            if ((f = t.m_arr[i][i]) == 0)
            {
                // Cannot invert singular matrix
                return Matrix44();
            }

            for (j = 0; j < 4; j++)
            {
                t.m_arr[i][j] /= f;
                s.m_arr[i][j] /= f;
            }

            for (j = 0; j < i; j++)
            {
                f = t.m_arr[j][i];

                for (k = 0; k < 4; k++)
                {
                    t.m_arr[j][k] -= f * t.m_arr[i][k];
                    s.m_arr[j][k] -= f * s.m_arr[i][k];
                }
            }
        }

        return s;
    }

    const Matrix44<T>& invert()
    {
        *this = inverse();
        return *this;
    }




};

typedef Matrix44<float> Matrix44f;
// ---------------------------------------------------------------------------

enum RayType {PrimaryRay, ShadowRay};

template< typename T >
class Ray
{
public:
    Ray() = default;

    Ray(const Vec3<T>& origin,
        const Vec3<T>& direction,
        const RayType& rayType)
        : m_origin(origin)
        , m_direction(direction)
        , m_rayType(rayType)
    {
    }

    Vec3<T> origin() const { return m_origin; }
    Vec3<T> direction() const { return m_direction; }
    Vec3<T> point_at_parameter(const float t) const { return m_origin + m_direction * t; }

    RayType m_rayType;

private:
    Vec3<T> m_origin;
    Vec3<T> m_direction;
};

typedef Ray<float> Rayf;
typedef Ray<int> Rayi;
typedef Ray<double> Rayd;