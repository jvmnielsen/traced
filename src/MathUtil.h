#pragma once
#include <iostream>
#include <vector>
#include <cmath>
#include <cstring>
#include <iomanip>

#define _USE_MATH_DEFINES
#include <math.h>
#include <array>

namespace Math
{
    const static float Infinity = std::numeric_limits<float>::max();

    template<typename T>
    constexpr T Clamp(const T& low, const T& high, const T& value)
    {
        return std::max(low, std::min(high, value));
    }

    template<typename T>
    float DegreeToRadian(const T degree)
    {
        return (T)(degree * M_PI / 180.0);
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

    bool SolveQuadratic(
            const float a,
            const float b,
            const float c,
            float& solutionOne,
            float& solutionTwo);

}

template<typename T>
struct Vec2 {
    T x, y;

    constexpr Vec2() = default;
    constexpr Vec2(const Vec2& other) = default;
    constexpr Vec2& operator=(const Vec2& other)
    {
        x = other.x;
        y = other.y;
        return *this;
    }

    constexpr explicit Vec2(T val) : x(val), y(val) {}

    constexpr Vec2(T x_, T y_, T z_) : x(x_), y(y_) {}

    constexpr T LengthSquared() const
    {
        return x * x + y * y;
    }

    constexpr T Length() const
    {
        return std::sqrt(LengthSquared());
    }

    constexpr Vec2 operator*(const T factor) const
    {
        return Vec2{x * factor, y * factor };
    }

    constexpr Vec2& operator*=(const T factor)
    {
        x *= factor;
        y *= factor;
        return *this;
    }

    constexpr Vec2 operator/(const T factor) const
    {
        return Vec2{ x / factor, y / factor };
    }

    constexpr Vec2& operator/=(const T factor)
    {
        x /= factor;
        y /= factor;
        return *this;
    }

    constexpr Vec2 operator+(const Vec2 &other) const
    {
        return Vec2{x + other.x, y + other.y };
    }

    constexpr Vec2 operator+=(const Vec2 &other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    constexpr Vec2 operator-(const Vec2 &other) const
    {
        return Vec2{x - other.x, y - other.y };
    }

    constexpr Vec2& operator-=(const Vec2 &other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    constexpr T DotProduct(const Vec2& other) const
    {
        return x * other.x + y * other.y;
    }

    constexpr Vec2 CrossProduct(const Vec2& other) const
    {
        return Vec2<T>{ x * other.y - other.x * y };
    }

    constexpr Vec2& Normalize()
    {
        T length = this->Length();
        if (length > 0) // avoid division by 0
        {
            T invertedLength = 1 / length;
            x *= invertedLength;
            y *= invertedLength;
        }
        return *this;
    }

    constexpr Vec2 operator-() const
    {
        return { -x, -y };
    }

    constexpr bool operator==(const Vec2& other)
    {
        return x == other.x && y == other.y;
    }

    constexpr bool operator!=(const Vec2& other)
    {
        return x != other.x || y != other.y;
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

    void PrettyPrint() const
    {
        std::cout << "[ " << x << ", " << y << " ]\n";
    }
};

template<typename T>
constexpr Vec2<T> Normalize(const Vec2<T>& vec)
{
    Vec2<T> tmp;

    T invertedLength = 1 / vec.Length();

    tmp.x *= invertedLength;
    tmp.y *= invertedLength;

    return tmp;
}


template<typename T>
constexpr Vec2<T> operator*(const T factor, const Vec2<T>& vec)
{
    return { vec.x * factor, vec.y * factor };
}

template<typename T>
constexpr Vec2<T> operator/(const T factor, const Vec2<T>& vec)
{
    return {vec.x / factor, vec.y / factor, };
}


typedef Vec2<float> Vec2f;
typedef Vec2<int> Vec2i;
typedef Vec2<double> Vec2d;


template<typename T>
struct Vec3 {
    T x, y, z;

    constexpr Vec3() = default;
    constexpr Vec3(const Vec3& other) = default;
    constexpr Vec3& operator=(const Vec3& other)
    {
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }

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
        z -= other.z;
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

    void PrettyPrint() const
    {
        std::cout << "[ " << x << ", " << y << ", " << z << " ]\n";
    }
};


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

template< typename T>
class Normal3 : public Vec3<T>
{
public:
    constexpr Normal3() = default;
    constexpr Normal3(const Vec3<T>& other) : Vec3<T>(other) { }
    constexpr explicit Normal3(T val) : Vec3<T>(val) {}
    constexpr Normal3(T x, T y, T z) : Vec3<T>({x, y, z}) {}
};


template<typename T>
constexpr Normal3<T> Normalize(const Vec3<T>& vec)
{
    return vec / vec.Length();
}

typedef Normal3<float> Normal3f;
typedef Normal3<int> Normal3i;
typedef Normal3<double> Normal3d;


// ---------------------------------------------------------------------------
template<typename T>
struct Point2
{
    T x, y;

    constexpr Point2() : x(0), y(0) { }
    constexpr explicit Point2(T val) : x(val), y(val) { }
    constexpr Point2(T x_, T y_) : x(x_), y(y_) { }

    constexpr Vec3<T> operator-(const Point2& other) const
    {
        return Vec3{ x - other.x, y - other.y };
    }

    constexpr Point2& operator-()
    {
        x = -x;
        y = -y;
        return *this;
    }

    constexpr Point2 operator+(const Vec3<T>& vec) const
    {
        return { x + vec.x, y + vec.y };
    }

    constexpr Point2 operator-(const Vec3<T>& vec) const
    {
        return { x - vec.x, y - vec.y };
    }

    constexpr bool operator==(const Point2& other) const
    {
        return x == other.x && y == other.y;
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

    void PrettyPrint() const
    {
        std::cout << "( " << x << ", " << y << " )\n";
    }
};

typedef Point2<float> Point2f;
typedef Point2<int> Point2i;
typedef Point2<double> Point2d;

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

    constexpr Point3 operator+(const Vec3<T>& vec) const
    {
        return { x + vec.x, y + vec.y, z + vec.z };
    }

    constexpr Point3 operator-(const Vec3<T>& vec) const
    {
        return { x - vec.x, y - vec.y, z - vec.z };
    }

    constexpr bool operator==(const Point3& other) const
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

    void PrettyPrint() const
    {
        std::cout << "( " << x << ", " << y << ", " << z << " )\n";
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
private:
    std::array<std::array<T, 4>, 4> m;
public:
    constexpr Matrix4x4
    (T a00, T a01, T a02, T a03,
     T a10, T a11, T a12, T a13,
     T a20, T a21, T a22, T a23,
     T a30, T a31, T a32, T a33)
     : m({a00, a01, a02, a03,
          a10, a11, a12, a13,
          a20, a21, a22, a23,
          a30, a31, a32, a33}) {}

    constexpr Matrix4x4()
    {
        constexpr auto tmp = std::array<std::array<T, 4>, 4>
        { 0, 0, 0, 0,
          0, 0, 0, 0,
          0, 0, 0, 0,
          0, 0, 0, 0 };

        m = tmp;
    }

    T& operator()(int i, int j) { return m[i][j]; }
    const T& operator()(int i, int j) const { return m[i][j]; }

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
                tmp.m[i][j] = m[j][i];

        return tmp;
    }

    constexpr Matrix4x4 getIdentity() const
    {
        constexpr auto identity = std::array<std::array<T, 4>, 4>
        { 1, 0, 0, 0,
          0, 1, 0, 0,
          0, 0, 1, 0,
          0, 0, 0, 1 };

        constexpr Matrix4x4 tmp{ identity };

        return tmp;
    }

    constexpr void setIdentity()
    {
        constexpr auto identity = std::array<std::array<T, 4>, 4>
        { 1, 0, 0, 0,
          0, 1, 0, 0,
          0, 0, 1, 0,
          0, 0, 0, 1 };

        m = identity;
    }

    constexpr Matrix4x4 Invert() const
    {
        //static_assert(N == M, "Matrix is not square!");

        Matrix4x4 tmp = *this;
        auto invTmp = tmp.getIdentity();

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

    void PrettyPrint() const
    {
        std::cout << "| " << std::setw(4) << m[0][0] << ", " << std::setw(4) << m[0][1] << ", " << std::setw(4)<< m[0][2] << ", " << std::setw(4) << m[0][3] << std::setw(4) << " |\n";
        std::cout << "| " << std::setw(4) << m[1][0] << ", " << std::setw(4) << m[1][1] << ", " << std::setw(4)<< m[1][2] << ", " << std::setw(4) << m[1][3] << std::setw(4) << " |\n";
        std::cout << "| " << std::setw(4) << m[2][0] << ", " << std::setw(4) << m[2][1] << ", " << std::setw(4)<< m[2][2] << ", " << std::setw(4) << m[2][3] << std::setw(4) << " |\n";
        std::cout << "| " << std::setw(4) << m[3][0] << ", " << std::setw(4) << m[3][1] << ", " << std::setw(4)<< m[3][2] << ", " << std::setw(4) << m[3][3] << std::setw(4) << " |\n";
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

    Ray(const Point3<T>& origin,
        const Vec3<T>& direction,
        const T maxParam = Math::Infinity,
        const T minParam = 0,
        const RayType& rayType = RayType::PrimaryRay)
        : m_origin(origin)
        , m_direction(direction)
        , m_maxParam(maxParam)
        , m_minParam(minParam)
        , m_rayType(rayType)
        , m_invDirection(1 / direction.x, 1 / direction.y, 1 / direction.z)
    {
        m_sign[0] = m_invDirection.x < 0;
        m_sign[1] = m_invDirection.y < 0;
        m_sign[2] = m_invDirection.z < 0;
    }

    constexpr Point3<T> Origin() const { return m_origin; }
    constexpr Vec3<T> Direction() const { return m_direction; }
    constexpr Point3<T> PointAtParameter(const float t) const { return m_origin + m_direction * t; }
    constexpr RayType getRayType() const { return m_rayType; }

    mutable T m_maxParam;
    mutable T m_minParam;

    Vec3f m_invDirection;
    int m_sign[3]; // used in AABB intersection test

private:
    Point3<T> m_origin;
    Vec3<T> m_direction;
    RayType m_rayType;
};

typedef Ray<float> Rayf;
typedef Ray<int> Rayi;
typedef Ray<double> Rayd;