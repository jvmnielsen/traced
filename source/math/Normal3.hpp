#pragma once
#include "vec3.hpp"

template<typename T>
struct Normal3 {
    T x, y, z;

    Normal3() = default;

    Normal3(Vec3<T> vec) {
        *this = Normalize(vec);
    }

    Normal3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) { }

    auto LengthSquared() const -> T {
        return x * x + y * y + z * z;
    }

    auto Length() const -> T {
        return std::sqrt(LengthSquared());
    }

    
    auto Dot(const Normal3& other) const -> T {
        return x * other.x + y * other.y + z * other.z;
    }

    auto operator-() const -> Normal3 {
        return Normal3{-x, -y, -z};
    }


    bool operator==(const Normal3& other) {
        return x == other.x && y == other.y && z == other.z;
    }

    bool operator!=(const Normal3& other) {
        return x != other.x || y != other.y || z != other.z;
    }

    // Accessors
    T operator[](const uint8_t i) const {
        return (&x)[i];
    }

    T& operator[](const uint8_t i) {
        return (&x)[i];
    }

    auto operator*(const T factor) const -> Vec3<T> {
        return Vec3<T>{x * factor, y * factor, z * factor};
    }

    auto operator/(const T factor) const -> Vec3<T> {
        return Vec3<T>{x / factor, y / factor, z / factor};
    }
};


template<typename T> auto
Normalize(const Vec3<T>& vec) -> Normal3<T> {
    const T length = vec.Length();
    if (length > 0) {
        T invertedLength = 1 / length;
        return {vec.x * invertedLength, vec.y * invertedLength, vec.z * invertedLength};
    }
    return {0,0,0};
}

template<typename T> auto
Dot(const Normal3<T>& a, const Normal3<T>& b) -> T {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

template< typename T> auto
Cross(const Normal3<T>& v1, const Normal3<T>& v2) -> Normal3<T> {
    return Normal3<T>{
            v1.y * v2.z - v1.z * v2.y,
            v1.z * v2.x - v1.x * v2.z,
            v1.x * v2.y - v1.y * v2.x};
}

template<typename T>
auto SameHemisphere(const Normal3<T>& u, const Normal3<T>& v) -> bool {
    return u.z * v.z > 0;
}


typedef Normal3<FLOAT> Normal3f;
typedef Normal3<int> Normal3i;
//typedef Normal3<double> Normal3d;