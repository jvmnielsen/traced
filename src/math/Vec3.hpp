#pragma once
#include <cmath>
#include "math_util.hpp"

template< typename T >
struct Normal3;

template<typename T>
class Vec3 {
public:

    Vec3() = default;
   
    explicit Vec3(T val) : m_x(val), m_y(val), m_z(val) {}

    Vec3(T x, T y, T z) : m_x(x), m_y(y), m_z(z) {}

    //Vec3(const Normal3<T>& n) : m_elements({n.x(), n.y(), n.z()}) { }

    auto x() const -> T { return m_x; }
    auto y() const -> T { return m_y; }
    auto z() const -> T { return m_z; }

    auto length_squared() const -> T {
        return x() * x() + y() * y() + z() * z();
    }

    auto length() const -> T {
        return std::sqrt(length_squared());
    }

    auto operator*(const T factor) const -> Vec3 {
        return Vec3{x() * factor, y() * factor, z() * factor};
    }

    auto operator/(const T factor) const -> Vec3 {
        return Vec3{x() / factor, y() / factor, z() / factor};
    }

    auto operator+(const Vec3 &other) const -> Vec3 {
        return Vec3{x() + other.x(), y() + other.y(), z() + other.z()};
    }

    auto operator-(const Vec3 &other) const -> Vec3 {
        return Vec3<T>{x() - other.x(), y() - other.y(), z() - other.z()};
    }

    auto operator-() const -> Vec3 {
        return Vec3{-x(), -y(), -z()};
    }

    auto operator==(const Vec3& other) -> bool {
        return x() == other.x() && y() == other.y() && z() == other.z();
    }

    auto operator!=(const Vec3& other) -> bool {
        return x() != other.x() || y() != other.y() || z() != other.z();
    }

    // Accessors
    auto operator[](const uint8_t i) const -> T {
        return (&m_x)[i];
    }

    auto operator[](const uint8_t i) -> T& {
        return (&m_x)[i];
    }

    auto normalize() const -> Vec3 {
        return *this / length();
    }

    auto is_normalized() -> bool {
        return std::abs(1 - length()) < Math::Constants::Epsilon;
    }

    static auto zero() -> Vec3 {
        return Vec3{0};
    }

private:
    T m_x, m_y, m_z;
};

template<typename T>
auto normalize(const Vec3<T>& vec) -> Vec3<T> {
    return vec.length() == 0 ? Vec3<T>::zero() : vec / vec.length();
}

template<typename T>
auto operator*(const T factor, const Vec3<T>& vec) -> Vec3<T>
{
    return Vec3<T>{vec.x() * factor, vec.y() * factor, vec.z() * factor};
}

template<typename T>
auto operator/(const T factor, const Vec3<T>& vec) -> Vec3<T> {
    return Vec3<T>{vec.x() / factor, vec.y() / factor, vec.z() / factor};
}

template<typename T> 
auto dot(const Vec3<T>& v1, const Vec3<T>& v2) -> T {
    return v1.x() * v2.x() + v1.y() * v2.y() + v1.z() * v2.z();
}

template< typename T > 
auto cross(const Vec3<T>& v1, const Vec3<T>& v2) -> Vec3<T>
{
    return Vec3<T>{
            v1.y() * v2.z() - v1.z() * v2.y(),
            v1.z() * v2.x() - v1.x() * v2.z(),
            v1.x() * v2.y() - v1.y() * v2.x()};
}

template<typename T>
auto same_hemisphere(const Vec3<T>& u, const Vec3<T>& v) -> bool {
    return dot(u, v) >= 0;
}

typedef Vec3<FLOAT> Vec3f;
typedef Vec3<int> Vec3i;