#pragma once
#include <iostream>
#include <vector>


float clamp(const float &lo, const float &hi, const float &v);

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
struct Vec
{
    T x, y, z;

    Vec();
    explicit Vec(T val);
    Vec(T x_, T y_, T z_);

    T MagnitudeSquared() const;
    T Magnitude() const;

    Vec operator *  (const T factor) const;
    Vec& operator *= (const T factor);

    Vec operator /  (const T factor) const;
    Vec& operator /= (const T factor);

    Vec operator + (const Vec& other) const;
    Vec& operator += (const Vec& other);

    Vec operator - (const Vec& other) const;
    Vec& operator -= (const Vec& other);

    Vec operator * (const Vec& other) const;
    Vec& operator *= (const Vec& other);

    Vec operator / (const Vec& other) const;
    Vec& operator /= (const Vec& other);

    T DotProduct(const Vec& other) const;
    
    Vec CrossProduct(const Vec& other) const;
    
    Vec& Normalize();

    // Accessors 
    T operator [] (const uint8_t i) const;
    T& operator [] (const uint8_t i);


};
// ---------------------------------------------------------------------------
template< typename T >
inline T DotProduct(const Vec<T>& vec1, const Vec<T>& vec2)
{
    return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
}
// ---------------------------------------------------------------------------
template< typename T >
Vec<T> CrossProduct( const Vec<T>& vec1, const Vec<T>& vec2 ) 
{
    return Vec<T>(
        vec1.y * vec2.z - vec1.z * vec2.y,
        vec1.z * vec2.x - vec1.x * vec2.z,
        vec1.x * vec2.y - vec1.y * vec2.x );
}
// ---------------------------------------------------------------------------
template< typename T >
inline std::istream& operator >> ( std::istream &is, Vec<T> &vec )
{
    is >> vec.x >> vec.y >> vec.z;
    return is;
}
// ---------------------------------------------------------------------------
template< typename T >
inline std::ostream& operator << ( std::ostream &os, Vec<T> &vec )
{
    os << vec.x << vec.y << vec.z;
    return os;
}
// ---------------------------------------------------------------------------
template< typename T >
inline Vec<T> UnitVector( Vec<T> vec )
{
    return vec / vec.Magnitude();
}
// ---------------------------------------------------------------------------
template< typename T >
inline Vec<T> operator * ( const float t, const Vec<T> &vec )
{
    return Vec<T>( t * vec.x, t*vec.y, t*vec.z );
}
// ---------------------------------------------------------------------------


template<typename T>
Vec<T> Normalize(const Vec<T>& vec)
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
typedef Vec<float> Vecf;
typedef Vec<int> Veci;
typedef Vec<double> Vecd;
// ---------------------------------------------------------------------------
template<typename T>
class Matrix44
{
public:
    T m_arr[4][4] = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};

    Matrix44() {}

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

    // accessors
    const T* operator [] (uint8_t i) const { return m_arr[i]; }
    T* operator [] (uint8_t i) { return m_arr[i]; }

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
    Vec<S> multiply_with_point(const Vec<S>& src) const
    {
        S a = src[0] * m_arr[0][0] + src[1] * m_arr[1][0] + src[2] * m_arr[2][0] + m_arr[3][0];
        S b = src[0] * m_arr[0][1] + src[1] * m_arr[1][1] + src[2] * m_arr[2][1] + m_arr[3][1];
        S c = src[0] * m_arr[0][2] + src[1] * m_arr[1][2] + src[2] * m_arr[2][2] + m_arr[3][2];
        S w = src[0] * m_arr[0][3] + src[1] * m_arr[1][3] + src[2] * m_arr[2][3] + m_arr[3][3];

        Vec<S> tmp;

        return  tmp = {a / w, b / w, c / w};
    }

    // multiply direction and matrix (don't divide coefficients)
    template< typename S >
    Vec<S> multiply_with_dir(const Vec<S>& src)
    {
        S a = src[0] * m_arr[0][0] + src[1] * m_arr[1][0] + src[2] * m_arr[2][0] + m_arr[3][0];
        S b = src[0] * m_arr[0][1] + src[1] * m_arr[1][1] + src[2] * m_arr[2][1] + m_arr[3][1];
        S c = src[0] * m_arr[0][2] + src[1] * m_arr[1][2] + src[2] * m_arr[2][2] + m_arr[3][2];
        S w = src[0] * m_arr[0][3] + src[1] * m_arr[1][3] + src[2] * m_arr[2][3] + m_arr[3][3];

        Vec<S> tmp;

        return tmp = {a, b, c};
    }

    Matrix44 inverse() const
    {
        int i, j, k;
        Matrix44 s;
        Matrix44 t(*this);

        for (i = 0; i < 3; i++)
        {
            auto pivot = i;

            T pivotsize = t[i][i];

            if (pivotsize < 0)
            {
                pivotsize = -pivotsize;
            }

            for (j = i + 1; j < 4; j++)
            {
                T tmp = t[j][i];

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
                    T tmp = t[i][j];
                    t[i][j] = t[pivot][j];
                    t[pivot][j] = tmp;

                    tmp = s[i][j];
                    s[i][j] = s[pivot][j];
                    s[pivot][j] = tmp;
                }
            }

            for (j = i + 1; j < 4; j++)
            {
                T f = t[j][i] / t[i][i];

                for (k = 0; k < 4; k++)
                {
                    t[j][k] -= f * t[i][k];
                    s[j][k] -= f * s[i][k];
                }
            }
        }

        // Backward substitution
        for (i = 3; i >= 0; --i)
        {
            T f;

            if ((f = t[i][i]) == 0)
            {
                // Cannot invert singular matrix
                return Matrix44();
            }

            for (j = 0; j < 4; j++)
            {
                t[i][j] /= f;
                s[i][j] /= f;
            }

            for (j = 0; j < i; j++)
            {
                f = t[j][i];

                for (k = 0; k < 4; k++)
                {
                    t[j][k] -= f * t[i][k];
                    s[j][k] -= f * s[i][k];
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
template< typename T >
class Ray
{
public:
    Ray() {}
    Ray(const Vec<T>& origin,
        const Vec<T>& direction)
        : m_origin(origin)
        , m_direction(direction)
    {
    }

    Vec<T> origin() const { return m_origin; }
    Vec<T> direction() const { return m_direction; }
    Vec<T> point_at_parameter(const float t) const { return m_origin + m_direction * t; }

private:
    Vec<T> m_origin;
    Vec<T> m_direction;
};

typedef Ray<float> Rayf;
typedef Ray<int> Rayi;
typedef Ray<double> Rayd;