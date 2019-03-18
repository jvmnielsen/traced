#pragma once
#include "vec3.hpp"

template<typename T>
class Normal3 {
public:

    Normal3() = default;

    Normal3(const Vec3<T>& vec) {
        const T length = vec.length();
        if (length > 0) {
            T inverted_length = 1 / length;
            m_elements = std::array<T, 3>{vec.x() * inverted_length, vec.y() * inverted_length, vec.z() * inverted_length};
        }
        m_elements = {0,0,0};
    }

    auto x() const -> T { return m_elements[0]; }
    auto y() const -> T { return m_elements[1]; }
    auto z() const -> T { return m_elements[2]; }

    auto length_squared() const -> T {
        return x * x + y * y + z * z;
    }

    auto length() const -> T {
        return std::sqrt(length_squared());
    }

    auto operator-() const -> Normal3 {
        return Normal3{-x(), -y(), -z()};
    }

    auto operator==(const Normal3& other) const -> bool {
        return x() == other.x() && y() == other.y() && z() == other.z();
    }

    auto operator!=(const Normal3& other) const -> bool {
        return x != other.x() || y != other.y() || z != other.z();
    }

    auto operator[](const std::size_t i) const -> T {
        return m_elements.at(i);
    }

    auto operator*(const T factor) const -> Vec3<T> {
        return Vec3<T>{x() * factor, y() * factor, z() * factor};
    }

    auto operator/(const T factor) const -> Vec3<T> {
        return Vec3<T>{x() / factor, y() / factor, z() / factor};
    }

private:

    Normal3(T x, T y, T z) : m_elements({x, y, z}) {}

    std::array<T, 3> m_elements;
};

template<typename T> auto
dot(const Normal3<T>& a, const Normal3<T>& b) -> T {
    return a.x() * b.x() + a.y() * b.y() + a.z() * b.z();
}

template< typename T> auto
cross(const Normal3<T>& v1, const Normal3<T>& v2) -> Vec3<T> {
    return Vec3<T>{
            v1.y() * v2.z() - v1.z() * v2.y(),
            v1.z() * v2.x() - v1.x() * v2.z(),
            v1.x() * v2.y() - v1.y() * v2.x()};
}

template<typename T>
auto same_hemisphere(const Normal3<T>& u, const Normal3<T>& v) -> bool {
    return u.z() * v.z() > 0;
}


typedef Normal3<FLOAT> Normal3f;
typedef Normal3<int> Normal3i;
//typedef Normal3<double> Normal3d;