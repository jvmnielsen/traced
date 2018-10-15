#pragma once
#include <iostream>
#include <vector>
#include <cmath>
#include <cstring>

#define _USE_MATH_DEFINES
#include <math.h>
#include <array>

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

    template<typename T>
    constexpr T Abs(const T& value)
    {
        return (T{} > value) ? -value : value;
    }

    template<typename T>
    constexpr void Swap(T& a, T& b)
    {
        T tmp = a;
        a = b;
        b = tmp;
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
/*
template<unsigned int M, unsigned int N, typename T>
class Matrix
{
public:
    //T m[M][N];
    std::array<std::array<T, N>, M> m;

    constexpr Matrix() = default;

    constexpr explicit Matrix(T val)
    {
        for (auto& subArray : m)
            for (auto& value : subArray)
                value = val;
    }

    constexpr explicit Matrix(const std::array<std::array<T, N>, M>& m_) : m(m_) { }

    constexpr Matrix(const Matrix& mtx) : m(mtx.m) { }

    constexpr Matrix<M, N, T> Transpose() const
    {
        Matrix tmp;
        for (int i = 0; i < M; i++)
            for (int j = 0; j < N; j++)
                tmp.m[N][M] = m[M][N];

        return tmp;
    }

    constexpr Matrix<M, N, T> Identity() const
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

    //virtual constexpr Point3f Multiply(const Point3f& point) const = 0;
    //virtual constexpr Vec3f Multiply(const Vec3f& vec) const = 0;

    constexpr Matrix<M, N, T> Invert() const
    {
        static_assert(N == M, "Matrix is not square!");

        auto tmp = *this;
        auto invTmp = tmp.Identity();

        // Switch rows around so there is a leading number in each
        // Set the diagonal to 1 by dividing the row with the leading element
        // Reduce upper right-hand entries to zero by subtracting fractions of below rows
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
                            tmp.m[j][k] = 0;
                            invTmp.m[j][k] -= coefficient * invTmp.m[i][k];
                        }

                    }
                }
            }

        }

        return invTmp;
    }
}; */
template< typename T >
class Matrix4x4
{
public:
    std::array<std::array<T, 4>, 4> m;

    constexpr Matrix4x4()
    {
        constexpr auto tmp = std::array<std::array<T, 4>, 4>
                { 0, 0, 0, 0,
                  0, 0, 0, 0,
                  0, 0, 0, 0,
                  0, 0, 0, 0 };

        m = tmp;
    }

    constexpr explicit Matrix4x4(const std::array<std::array<float, 4>, 4>& _m) : m(_m){}

    constexpr explicit Matrix4x4(T val)
    {
        for (auto& subArray : m)
            for (auto& value : subArray)
                value = val;
    }

    constexpr Matrix4x4(const Matrix4x4& mtx) : m(mtx.m) { }

    constexpr Matrix4x4 Transpose() const
    {
        Matrix4x4 tmp;
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                tmp.m[j][i] = m[i][j];

        return tmp;
    }

    constexpr Matrix4x4 Identity() const
    {
        constexpr auto identity = std::array<std::array<T, 4>, 4>
        { 1, 0, 0, 0,
          0, 1, 0, 0,
          0, 0, 1, 0,
          0, 0, 0, 1 };

        constexpr Matrix4x4 tmp{ identity };

        return tmp;
    }

    constexpr Matrix4x4 Invert() const
    {
        //static_assert(N == M, "Matrix is not square!");

        Matrix4x4 tmp = *this;
         auto invTmp = tmp.Identity();

        // Switch rows around so there is a leading number in each
        // Set the diagonal to 1 by dividing the row with the leading element
        // Reduce upper right-hand entries to zero by subtracting fractions of below rows
        // This is all done in parallel to the adjoined identity matrix

        for (int i = 0; i < 4; ++i)
        {
            if (tmp.m[i][i] == 0) // Our row had a leading zero
            {
                auto big = i;
                for (int j = 0; j < 4; ++j) // Check remaining rows
                {
                    if (Math::Abs(tmp.m[j][i]) > Math::Abs(tmp.m[big][i]))
                    {
                        big = j; // Another row has a (absolute) larger leading value
                    }
                }

                if (big != i)  // The matrix wasn't singular, i.e. another row could be switched in
                {
                    for (int k = 0; k < 4; ++k)
                    {
                        Math::Swap(tmp.m[i][k], tmp.m[big][k]); // swap rows
                        Math::Swap(invTmp.m[i][k], invTmp.m[big][k]);
                    }
                }
            }

            auto divisor = tmp.m[i][i];
            for (int j = 0; j < 4; ++j)
            {
                tmp.m[i][j] /= divisor;
                invTmp.m[i][j] /= divisor;
            }

            for (int j = 0; j < 4; ++j)
            {
                if (j != i && tmp.m[i][i] != 0)
                {
                    auto coefficient = tmp.m[j][i]/tmp.m[i][i];
                    if (coefficient != 0)
                    {
                        for (int k = 0; k < 4; ++k)
                        {
                            tmp.m[j][k] -= coefficient * tmp.m[i][k];
                            tmp.m[j][k] = 0;
                            invTmp.m[j][k] -= coefficient * invTmp.m[i][k];
                        }

                    }
                }
            }

        }

        return invTmp;
    }


    constexpr static void Multiply(const Matrix4x4& a, const Matrix4x4& b, Matrix4x4& c)
    {
        // rolled up version rather than writing out the arguments
        for (uint8_t i = 0; i < 4; ++i) {
            for (uint8_t j = 0; j < 4; ++j) {
                c.m[i][j] = a.m[i][0] * b.m[0][j] + a.m[i][1] * b.m[1][j] +
                    a.m[i][2] * b.m[2][j] + a.m[i][3] * b.m[3][j];
            }
        }
    }

    constexpr Matrix4x4 operator*(const Matrix4x4& other) const
    {
        Matrix4x4 tmp;
        Multiply(*this, other, tmp);
        return tmp;
    }

    constexpr Point3f Multiply(const Point3f& point) const
    {
        //using Matrix<4, 4, T>::m; // tell the compiler that we are referring to a member of the class
                                  // alternatively use this->m
        auto a = point[0] * m[0][0] + point[1] * m[1][0] + point[2] * m[2][0] + m[3][0];
        auto b = point[0] * m[0][1] + point[1] * m[1][1] + point[2] * m[2][1] + m[3][1];
        auto c = point[0] * m[0][2] + point[1] * m[1][2] + point[2] * m[2][2] + m[3][2];
        auto w = point[0] * m[0][3] + point[1] * m[1][3] + point[2] * m[2][3] + m[3][3];

        return {a / w, b / w, c / w};
    }

    constexpr Vec3f Multiply(const Vec3f& vec) const
    {
        auto a = vec[0] * m[0][0] + vec[1] * m[1][0] + vec[2] * m[2][0] + m[3][0];
        auto b = vec[0] * m[0][1] + vec[1] * m[1][1] + vec[2] * m[2][1] + m[3][1];
        auto c = vec[0] * m[0][2] + vec[1] * m[1][2] + vec[2] * m[2][2] + m[3][2];
        auto w = vec[0] * m[0][3] + vec[1] * m[1][3] + vec[2] * m[2][3] + m[3][3];

        return {a, b, c};
    }
};
typedef Matrix4x4<float> Matrix4x4f;

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