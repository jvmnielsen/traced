#pragma once
#include "vec3.hpp"

template<typename T>
struct point3
{
    T x, y, z;

    point3() : x(0), y(0), z(0) { }
    explicit point3(T val) : x(val), y(val), z(val) { }
    point3(T x_, T y_, T z_) : x(x_), y(y_), z(z_) { }

    vec3<T> operator-(const point3& other) const
    {
        return vec3<T>{ x - other.x, y - other.y, z - other.z };
    }

    point3& operator-()
    {
        x = -x;
        y = -y;
        z = -z;
        return *this;
    }

    point3 operator+(const vec3<T>& vec) const
    {
        return {x + vec.x, y + vec.y, z + vec.z};
    }

    point3 operator-(const vec3<T>& vec) const
    {
        return {x - vec.x, y - vec.y, z - vec.z};
    }

    bool operator==(const point3& other) const
    {
        return x == other.x && y == other.y && z == other.z;
    }

    bool operator!=(const point3& other) const
    {
        return x != other.x || y != other.y || z != other.z;
    }

    // Accessors
    T operator[](const uint8_t i) const
    {
        return (&x)[i];
    }

    T& operator[](const uint8_t i)
    {
        return (&x)[i];
    }

    void PrettyPrint() const
    {
        std::cout << "( " << x << ", " << y << ", " << z << " )\n";
    }
};


template<typename T>
point3<T> operator*(const T factor, const point3<T>& point)
{
    return point3<T>{point.x * factor, point.y * factor, point.z * factor};
}

template<typename T>
point3<T> operator/(const T factor, const point3<T>& point)
{
    return point3<T>{point.x / factor, point.y / factor, point.z / factor};
}


typedef point3<float> Point3f;
typedef point3<int> Point3i;
typedef point3<double> Point3d;