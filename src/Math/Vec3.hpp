#pragma once
#include <iostream>
#include <cmath>

template<typename T>
struct Vec3 {
    T x, y, z;

    Vec3() = default;
    Vec3(const Vec3& other) = default;
    Vec3& operator=(const Vec3& other)
    {
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }

    explicit Vec3(T val) : x(val), y(val), z(val) {}

    Vec3(T x_, T y_, T z_) : x(x_), y(y_), z(z_) {}

    T LengthSquared() const
    {
        return x * x + y * y + z * z;
    }

    T Length() const
    {
        return std::sqrt(LengthSquared());
    }

    Vec3 operator*(const T factor) const
    {
        return Vec3{x * factor, y * factor, z * factor};
    }

    Vec3& operator*=(const T factor)
    {
        x *= factor;
        y *= factor;
        z *= factor;
        return *this;
    }

    Vec3 operator/(const T factor) const
    {
        return Vec3{x / factor, y / factor, z / factor};
    }

    Vec3& operator/=(const T factor)
    {
        x /= factor;
        y /= factor;
        z /= factor;
        return *this;
    }

    Vec3 operator+(const Vec3 &other) const
    {
        return Vec3{x + other.x, y + other.y, z + other.z};
    }

    Vec3 operator+=(const Vec3 &other)
    {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    Vec3 operator-(const Vec3 &other) const
    {
        return Vec3<T>{x - other.x, y - other.y, z - other.z};
    }

    Vec3& operator-=(const Vec3 &other)
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    T DotProduct(const Vec3& other) const
    {
        return x * other.x + y * other.y + z * other.z;
    }

    Vec3 CrossProduct(const Vec3& other) const
    {
        return Vec3<T>{
            y * other.z - z * other.y,
                z * other.x - x * other.z,
                x * other.y - y * other.x};
    }

    Vec3& Normalize()
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

    Vec3 operator-() const
    {
        return Vec3{-x, -y, -z};
    }

    
    bool operator==(const Vec3& other)
    {
        return x == other.x && y == other.y && z == other.z;
    }

    bool operator!=(const Vec3& other)
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
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
template< typename T > auto
Cross(const Vec3<T>& v1, const Vec3<T>& v2) -> Vec3<T>
{
    return Vec3<T>{
        v1.y * v2.z - v1.z * v2.y,
            v1.z * v2.x - v1.x * v2.z,
            v1.x * v2.y - v1.y * v2.x};
}


typedef Vec3<float> Vec3f;
typedef Vec3<int> Vec3i;
typedef Vec3<double> Vec3d;
