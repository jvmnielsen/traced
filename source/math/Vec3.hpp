#pragma once
#include <cmath>
#include "math_util.hpp"

template< typename T >
struct Normal3;

template<typename T>
struct Vec3 {
    T x, y, z;

    Vec3() = default;
   
    explicit Vec3(T val) : x(val), y(val), z(val) {}

    Vec3(T x_, T y_, T z_) : x(x_), y(y_), z(z_) {}

    Vec3(const Normal3<T>& n) : x(n.x), y(n.y), z(n.z) { }

    auto LengthSquared() const -> T {
        return x * x + y * y + z * z;
    }

    auto Length() const -> T {
        return std::sqrt(LengthSquared());
    }

    auto operator*(const T factor) const -> Vec3 {
        return Vec3{x * factor, y * factor, z * factor};
    }

    auto operator*=(const T factor) -> Vec3& {
        x *= factor;
        y *= factor;
        z *= factor;
        return *this;
    }

    auto operator/(const T factor) const -> Vec3 {
        return Vec3{x / factor, y / factor, z / factor};
    }

    auto operator/=(const T factor) -> Vec3& {
        x /= factor;
        y /= factor;
        z /= factor;
        return *this;
    }

    auto operator+(const Vec3 &other) const -> Vec3 {
        return Vec3{x + other.x, y + other.y, z + other.z};
    }

    auto operator+=(const Vec3 &other) -> Vec3 {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    auto operator-(const Vec3 &other) const -> Vec3 {
        return Vec3<T>{x - other.x, y - other.y, z - other.z};
    }

    auto operator-=(const Vec3 &other) -> Vec3& {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    auto operator-() const -> Vec3 {
        return Vec3{-x, -y, -z};
    }

    auto operator==(const Vec3& other) -> bool {
        return x == other.x && y == other.y && z == other.z;
    }

    auto operator!=(const Vec3& other) -> bool {
        return x != other.x || y != other.y || z != other.z;
    }

    // Accessors
    auto operator[](const uint8_t i) const -> T {
        return (&x)[i];
    }

    auto operator[](const uint8_t i) -> T& {
        return (&x)[i];
    }
};

/*
template< typename T > auto
operator==(const Vec3<T>& v1, const Vec3<T>& v2) -> bool
{
    return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z;
}
*/

template<typename T>
Vec3<T> operator*(const T factor, const Vec3<T>& vec)
{
    return Vec3<T>{vec.x * factor, vec.y * factor, vec.z * factor};
}

template<typename T>
Vec3<T> operator/(const T factor, const Vec3<T>& vec)
{
    return Vec3<T>{vec.x / factor, vec.y / factor, vec.z / factor};
}

template<typename T> auto
Dot(const Vec3<T>& v1, const Vec3<T>& v2) -> T
{
    return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

template< typename T > auto
Cross(const Vec3<T>& v1, const Vec3<T>& v2) -> Vec3<T>
{
    return Vec3<T>{
            v1.y * v2.z - v1.z * v2.y,
            v1.z * v2.x - v1.x * v2.z,
            v1.x * v2.y - v1.y * v2.x};
}

template<typename T>
auto SameHemisphere(const Vec3<T>& u, const Vec3<T>& v) -> bool {
    return u.z * v.z > 0;
}

typedef Vec3<FLOAT> Vec3f;
typedef Vec3<int> Vec3i;