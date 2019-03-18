#pragma once
#include "normal3.hpp"
#include "math_util.hpp"


template<typename T>
class Point3 {
    std::array<T, 3> m_elements;// x, y, z;
public:
    //Point3() : x(0), y(0), z(0) { }
    explicit Point3(T val) : m_elements({val, val, val}) {} //x(val), y(val), z(val) {}
    Point3(T x, T y, T z) : m_elements({x, y, z}) {}// x(x_), y(y_), z(z_) {}

    auto x() const -> T {
        return m_elements[0];
    }

    auto y() const -> T {
        return m_elements[1];
    }

    auto z() const -> T {
        return m_elements[2];
    }

    auto operator-() -> Point3& {
        x() = -x();
        y() = -y();
        z() = -z();
        return *this;
    }

    Vec3<T> operator-(const Point3& other) const {
        return Vec3<T>{ x() - other.x(), y() - other.y(), z() - other.z() };
    }

    auto operator+(const Vec3<T>& vec) const -> Point3 {
        return {x() + vec.x, y() + vec.y, z() + vec.z};
    }

    auto operator+(const Normal3<T>& n) const -> Point3 {
        return {x() + n.x, y() + n.y, z() + n.z};
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
        return x == other.x() && y == other.y() && z == other.z();
    }

    bool operator!=(const Point3& other) const
    {
        return x() != other.x() || y() != other.y() || z() != other.z();
    }

    /*
    auto operator>(const Point3& other) const -> bool {
        return x() > other.x() && y() > other.y() && z() > other.z();
    }

    auto operator<(const Point3& other) const -> bool {
        return x() < other.x() && y() < other.y() && z() < other.z();
    } */

    auto ElementwiseMin(const Point3& other) const -> Point3 {
        return {std::min(x(), other.x(), std::min(y(), other.y()), std::min(z(), other.z()))};
    }

    auto ElementwiseMax(const Point3& other) const -> Point3 {
        return {std::max(x(), other.x(), std::max(y(), other.y()), std::max(z(), other.z()))};
    }

    auto Elements() const { return m_elements; }
    //auto begin() const { m_elements.begin(); }
    //auto end() const { m_elements.end(); }
    auto operator[](const std::size_t i) -> T& { return m_elements[i]; }
    auto operator[](const std::size_t i) const -> T { return m_elements[i]; }
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