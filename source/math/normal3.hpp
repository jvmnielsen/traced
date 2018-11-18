#pragma once
#include "vec3.hpp"


template< typename T>
class normal3 : public vec3<T>
{
public:
    normal3() = default;
    normal3(const vec3<T>& other) : vec3<T>(other) { }
    explicit normal3(T val) : vec3<T>(val) {}
    normal3(T x, T y, T z) : vec3<T>({x, y, z}) {}
};


template<typename T>
normal3<T> Normalize(const vec3<T>& vec)
{
    return vec / vec.Length();
}

typedef normal3<float> Normal3f;
typedef normal3<int> Normal3i;
typedef normal3<double> Normal3d;