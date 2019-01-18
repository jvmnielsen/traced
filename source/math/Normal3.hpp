#pragma once
#include "vec3.hpp"


template< typename T>
class Normal3 : public Vec3<T>
{
public:
    Normal3() = default;
    Normal3(const Vec3<T>& other) : Vec3<T>(other) { }
    explicit Normal3(T val) : Vec3<T>(val) {}
    Normal3(T x, T y, T z) : Vec3<T>({x, y, z}) {}
};


template<typename T>
Normal3<T> Normalize(const Vec3<T>& vec)
{
    return vec / vec.Length();
}

typedef Normal3<float> Normal3f;
typedef Normal3<int> Normal3i;
typedef Normal3<double> Normal3d;