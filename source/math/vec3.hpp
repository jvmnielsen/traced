#pragma once
#include <iostream>
#include <cmath>

template<typename T>
struct vec3 {
    T x, y, z;

    vec3() = default;
   
    explicit vec3(T val) : x(val), y(val), z(val) {}

    vec3(T x_, T y_, T z_) : x(x_), y(y_), z(z_) {}

    T LengthSquared() const
    {
        return x * x + y * y + z * z;
    }

    T Length() const
    {
        return std::sqrt(LengthSquared());
    }

    vec3 operator*(const T factor) const
    {
        return vec3{x * factor, y * factor, z * factor};
    }

    vec3& operator*=(const T factor)
    {
        x *= factor;
        y *= factor;
        z *= factor;
        return *this;
    }

    vec3 operator/(const T factor) const
    {
        return vec3{x / factor, y / factor, z / factor};
    }

    vec3& operator/=(const T factor)
    {
        x /= factor;
        y /= factor;
        z /= factor;
        return *this;
    }

    vec3 operator+(const vec3 &other) const
    {
        return vec3{x + other.x, y + other.y, z + other.z};
    }

    vec3 operator+=(const vec3 &other)
    {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    vec3 operator-(const vec3 &other) const
    {
        return vec3<T>{x - other.x, y - other.y, z - other.z};
    }

    vec3& operator-=(const vec3 &other)
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    T DotProduct(const vec3& other) const
    {
        return x * other.x + y * other.y + z * other.z;
    }

    vec3 CrossProduct(const vec3& other) const
    {
        return vec3<T>{
            y * other.z - z * other.y,
                z * other.x - x * other.z,
                x * other.y - y * other.x};
    }

    vec3& Normalize()
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

    vec3 operator-() const
    {
        return vec3{-x, -y, -z};
    }

    
    bool operator==(const vec3& other)
    {
        return x == other.x && y == other.y && z == other.z;
    }

    bool operator!=(const vec3& other)
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
        std::cout << "[ " << x << ", " << y << ", " << z << " ]\n";
    }
};

template< typename T > auto
operator==(const vec3<T>& v1, const vec3<T>& v2) -> bool
{
    return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z;
}


template<typename T>
vec3<T> operator*(const T factor, const vec3<T>& vec)
{
    return vec3<T>{vec.x * factor, vec.y * factor, vec.z * factor};
}

template<typename T>
vec3<T> operator/(const T factor, const vec3<T>& vec)
{
    return vec3<T>{vec.x / factor, vec.y / factor, vec.z / factor};
}

template<typename T> auto
Dot(const vec3<T>& v1, const vec3<T>& v2) -> T
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
template< typename T > auto
Cross(const vec3<T>& v1, const vec3<T>& v2) -> vec3<T>
{
    return vec3<T>{
        v1.y * v2.z - v1.z * v2.y,
            v1.z * v2.x - v1.x * v2.z,
            v1.x * v2.y - v1.y * v2.x};
}

typedef vec3<float> Vec3f;
typedef vec3<int> Vec3i;
typedef vec3<double> Vec3d;