#pragma once
#include <iostream>
#include <vector>
#include <cmath>
#include <cstring>

#define _USE_MATH_DEFINES
#include <math.h>

namespace Math
{
    template<typename T>
    constexpr T Clamp(const T& low, const T& high, const T& value)
    {
        return std::max(low, std::min(high, value));
    }

    template<typename T>
    constexpr T DegreeToRadian(const T degree)
    {
        return (T)(degree * M_PI / 180);
    }
}

template<typename T>
struct Vec3 {
    T x, y, z;

    constexpr Vec3() = default;

    constexpr explicit Vec3(T val) : x(val), y(val), z(val) {}

    constexpr Vec3(T x_, T y_, T z_) : x(x_), y(y_), z(z_) {}

    constexpr T LengthSquared() const
    {
        return x * x + y * y + z * z;
    }

    constexpr T Length() const
    {
        return std::sqrt(LengthSquared());
    }

    constexpr Vec3 operator*(const T factor) const
    {
        return Vec3{x * factor, y * factor, z * factor};
    }

    constexpr Vec3& operator*=(const T factor)
    {
        x *= factor;
        y *= factor;
        z *= factor;
        return *this;
    }

    constexpr Vec3 operator/(const T factor) const
    {
        return Vec3{x / factor, y / factor, z / factor};
    }

    constexpr Vec3& operator/=(const T factor)
    {
        x /= factor;
        y /= factor;
        z /= factor;
        return *this;
    }

    constexpr Vec3 operator+(const Vec3 &other) const
    {
        return Vec3{x + other.x, y + other.y, z + other.z};
    }

    constexpr Vec3 operator+=(const Vec3 &other)
    {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    constexpr Vec3 operator-(const Vec3 &other) const
    {
        return Vec3{x - other.x, y - other.y, z - other.z};
    }

    constexpr Vec3& operator-=(const Vec3 &other)
    {
        x -= other.x;
        y -= other.y;
        z - +other.z;
        return *this;
    }

    constexpr T DotProduct(const Vec3& other) const
    {
        return x * other.x + y * other.y + z * other.z;
    }

    constexpr Vec3 CrossProduct(const Vec3& other) const
    {
        return Vec3<T>{
                y * other.z - z * other.y,
                z * other.x - x * other.z,
                x * other.y - y * other.x};
    }

    constexpr Vec3& Normalize()
    {
        T length = this->Length();
        if (length > 0) // avoid division by 0
        {
            T invertedLength = 1 / length;
            x *= invertedLength;
            y *= invertedLength;
            z *= invertedLength;
        }
        return *this;
    }

    constexpr Vec3 operator-() const
    {
        return Vec3{ -x, -y, -z };
    }

    constexpr bool operator==(const Vec3& other)
    {
        return x == other.x && y == other.y && z == other.z;
    }

    constexpr bool operator!=(const Vec3& other)
    {
        return x != other.x || y != other.y || z != other.z;
    }

    // Accessors
    constexpr T operator[](const uint8_t i) const
    {
        return (&x)[i];
    }

    constexpr T& operator[](const uint8_t i)
    {
        return (&x)[i];
    }
};

template<typename T>
constexpr Vec3<T> Normalize(const Vec3<T>& vec)
{
    Vec3<T> tmp;

    T invertedLength = 1 / vec.Length();

    tmp.x *= invertedLength;
    tmp.y *= invertedLength;
    tmp.z *= invertedLength;

    return tmp;
    //return vec / vec.Length();
}

template<typename T>
constexpr Vec3<T> operator*(const T factor, const Vec3<T>& vec)
{
    return Vec3{vec.x * factor, vec.y * factor, vec.z * factor};
}

template<typename T>
constexpr Vec3<T> operator/(const T factor, const Vec3<T>& vec)
{
    return Vec3{vec.x / factor, vec.y / factor, vec.z / factor};
}


typedef Vec3<float> Vec3f;
typedef Vec3<int> Vec3i;
typedef Vec3<double> Vec3d;
// ---------------------------------------------------------------------------
template<typename T>
struct Point3
{
    T x, y, z;

    constexpr Point3() : x(0), y(0), z(0) { }
    constexpr explicit Point3(T val) : x(val), y(val), z(val) { }
    constexpr Point3(T x_, T y_, T z_) : x(x_), y(y_), z(z_) { }

    constexpr Vec3<T> operator-(const Point3& other) const
    {
        return Vec3{ x - other.x, y - other.y, z - other.z };
    }

    constexpr Point3& operator-()
    {
        x = -x;
        y = -y;
        z = -z;
        return *this;
    }

    constexpr bool operator==(const Point3& other)
    {
        return x == other.x && y == other.y && z == other.z;
    }

    // Accessors
    constexpr T operator[](const uint8_t i) const
    {
        return (&x)[i];
    }

    constexpr T& operator[](const uint8_t i)
    {
        return (&x)[i];
    }
};

typedef Point3<float> Point3f;
typedef Point3<int> Point3i;
typedef Point3<double> Point3d;

// ---------------------------------------------------------------------------
template<unsigned int M, unsigned int N, typename T>
class Matrix
{
public:
    T m[M][N];

    constexpr Matrix() = default;

    constexpr explicit Matrix(T val)
    {
        for (auto& subArray : m)
            for (auto& value : subArray)
                value = val;
    }

    constexpr Matrix Transpose() const
    {
        Matrix tmp;
        for (int i = 0; i < M; i++)
            for (int j = 0; j < N; j++)
                tmp[N][M] = m[M][N];

        return tmp;
    }

    constexpr Matrix Identity() const
    {
        Matrix tmp;
        for (int i = 0; i < M; i++)
        {
            for (int j = 0; j < N; j++)
            {
                if (i == j)
                    tmp.m[i][j] = static_cast<T>(1);
                else
                    tmp.m[i][j] = static_cast<T>(0);
            }
        }
        return tmp;
    }

    constexpr Matrix Invert() const
    {
        static_assert(N == M, "Matrix is not square!");

        Matrix tmp = *this;
        Matrix invTmp = tmp.Identity();

        // Switch rows around so there is a leading number in each
        // Reduce upper right-hand entries to zero by subtracting fractions of below rows
        // Set the diagonal to 1 by dividing the row with the leading element
        // This is all done in parallel to the adjoined identity matrix

        for (int i = 0; i < M; ++i)
        {
            if (tmp.m[i][i] == 0) // Our row had a leading zero
            {
                auto big = i;
                for (int j = 0; j < M; ++j) // Check remaining rows
                {
                    if (std::abs(tmp.m[j][i]) > std::abs(tmp.m[big][i]))
                    {
                        big = j; // Another row has a (absolute) larger leading value
                    }
                }

                if (big != i)  // The matrix wasn't singular, i.e. another row could be switched in
                {
                    for (int k = 0; k < M; ++k)
                    {
                        std::swap(tmp.m[i][k], tmp.m[big][k]); // swap rows
                        std::swap(invTmp.m[i][k], invTmp.m[big][k]);
                    }
                }
            }

            auto divisor = tmp.m[i][i];
            for (int j = 0; j < M; ++j)
            {
                tmp.m[i][j] /= divisor;
                invTmp.m[i][j] /= divisor;
            }

            for (int j = 0; j < M; ++j)
            {
                if (j != i && tmp.m[i][i] != 0)
                {
                    auto coefficient = tmp.m[j][i]/tmp.m[i][i];
                    if (coefficient != 0)
                    {
                        for (int k = 0; k < M; ++k)
                        {
                            tmp.m[j][k] -= coefficient * tmp.m[i][k];
                            invTmp.m[j][k] -= coefficient * invTmp.m[i][k];
                        }
                    }
                }
            }

        }

        return invTmp;
    }
};


template<typename T>
class Matrix4x4 : public Matrix<4, 4, T>
{
public:


    constexpr Matrix4x4(T a, T b, T c, T d, T e, T f, T g, T h,
             T i, T j, T k, T l, T m, T n, T o, T p)
    {
        m[0][0] = a;
        m[0][1] = b;
        m[0][2] = c;
        m[0][3] = d;
        m[1][0] = e;
        m[1][1] = f;
        m[1][2] = g;
        m[1][3] = h;
        m[2][0] = i;
        m[2][1] = j;
        m[2][2] = k;
        m[2][3] = l;
        m[3][0] = m;
        m[3][1] = n;
        m[3][2] = o;
        m[3][3] = p;
    }

    static void multiply(const Matrix4x4& a, const Matrix4x4& b, Matrix4x4& c)
    {
        // rolled up version rather than writing out the arguments
        for (uint8_t i = 0; i < 4; ++i) {
            for (uint8_t j = 0; j < 4; ++j) {
                c[i][j] = a[i][0] * b[0][j] + a[i][1] * b[1][j] +
                    a[i][2] * b[2][j] + a[i][3] * b[3][j];
            }
        }
    }

    Matrix4x4 operator * (const Matrix4x4& m) const
    {
        Matrix4x4 tmp;
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

    Matrix4x4 inverse() const
    {
        int i, j, k;
        Matrix4x4 s;
        Matrix4x4 t = *this;

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
                return Matrix4x4();
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
                return Matrix4x4();
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

    const Matrix4x4<T>& invert()
    {
        *this = inverse();
        return *this;
    }




};

typedef Matrix4x4<float> Matrix44f;
// ---------------------------------------------------------------------------

enum class RayType {PrimaryRay, ShadowRay};

template< typename T >
class Ray
{
public:
    constexpr Ray() = default;

    constexpr Ray(const Vec3<T>& origin,
                  const Vec3<T>& direction,
                  const RayType& rayType)
        : m_origin(origin)
        , m_direction(direction)
        , m_rayType(rayType)
    {
    }

    constexpr Vec3<T> origin() const { return m_origin; }
    constexpr Vec3<T> direction() const { return m_direction; }
    constexpr Vec3<T> pointAtParameter(const float t) const { return m_origin + m_direction * t; }

    RayType m_rayType;

private:
    Vec3<T> m_origin;
    Vec3<T> m_direction;
};

typedef Ray<float> Rayf;
typedef Ray<int> Rayi;
typedef Ray<double> Rayd;