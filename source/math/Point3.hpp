#pragma once
#include "normal3.hpp"
#include "math_util.hpp"


template<typename T>
struct Point3
{
    T x, y, z;

    Point3() : x(0), y(0), z(0) { }
    explicit Point3(T val) : x(val), y(val), z(val) { }
    Point3(T x_, T y_, T z_) : x(x_), y(y_), z(z_) { }

    Vec3<T> operator-(const Point3& other) const
    {
        return Vec3<T>{ x - other.x, y - other.y, z - other.z };
    }

    Point3& operator-()
    {
        x = -x;
        y = -y;
        z = -z;
        return *this;
    }

    auto operator+(const Vec3<T>& vec) const -> Point3 {
        return {x + vec.x, y + vec.y, z + vec.z};
    }

    auto operator+(const Normal3<T>& n) const -> Point3 {
        return {x + n.x, y + n.y, z + n.z};
    }

    auto operator+(const Point3<T>& p) const -> Point3 {
        return {x + p.x, y + p.y, z + p.z};
    }

    auto operator-(const Vec3<T>& vec) const -> Point3 {
        return {x - vec.x, y - vec.y, z - vec.z};
    }

    auto operator-(const Normal3<T>& n) const -> Point3 {
        return {x - n.x, y - n.y, z - n.z};
    }

    bool operator==(const Point3& other) const
    {
        return x == other.x && y == other.y && z == other.z;
    }

    bool operator!=(const Point3& other) const
    {
        return x != other.x || y != other.y || z != other.z;
    }

    // Accessors
    T operator[](const uint8_t i) const {
        return (&x)[i];
    }

    T& operator[](const uint8_t i) {
        return (&x)[i];
    }
};


template<typename T>
Point3<T> operator*(const T factor, const Point3<T>& point)
{
    return Point3<T>{point.x * factor, point.y * factor, point.z * factor};
}

template<typename T>
Point3<T> operator/(const T factor, const Point3<T>& point)
{
    return Point3<T>{point.x / factor, point.y / factor, point.z / factor};
}


typedef Point3<FLOAT> Point3f;
typedef Point3<int> Point3i;
//typedef Point3<double> Point3d;