#pragma once
#include "vec2.hpp"

template<typename T>
struct point2
{
    T x, y;

    point2() : x(0), y(0) { }
    explicit point2(T val) : x(val), y(val) { }
    point2(T x_, T y_) : x(x_), y(y_) { }

    vec2<T> operator-(const point2& other) const
    {
        return vec2<T>{ x - other.x, y - other.y };
    }

    point2& operator-()
    {
        x = -x;
        y = -y;
        return *this;
    }

    point2 operator+(const vec3<T>& vec) const
    {
        return {x + vec.x, y + vec.y};
    }

    point2 operator-(const vec3<T>& vec) const
    {
        return {x - vec.x, y - vec.y};
    }

    bool operator==(const point2& other) const
    {
        return x == other.x && y == other.y;
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
        std::cout << "( " << x << ", " << y << " )\n";
    }
};

typedef point2<float> Point2f;
typedef point2<int> Point2i;
typedef point2<double> Point2d;