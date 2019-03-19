#pragma once
#include "vec2.hpp"
#include "math_util.hpp"
#include "vec3.hpp"


template<typename T>
struct Point2
{
    T x, y;

    Point2() : x(0), y(0) { }
    explicit Point2(T val) : x(val), y(val) { }
    Point2(T x_, T y_) : x(x_), y(y_) { }

    Vec2<T> operator-(const Point2& other) const
    {
        return Vec2<T>{ x - other.x, y - other.y };
    }

    Point2& operator-()
    {
        x = -x;
        y = -y;
        return *this;
    }

    Point2 operator+(const Vec3<T>& vec) const
    {
        return {x + vec.x(), y + vec.y()};
    }

    Point2 operator-(const Vec3<T>& vec) const
    {
        return {x - vec.x(), y - vec.y()};
    }

    bool operator==(const Point2& other) const
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

typedef Point2<FLOAT> Point2f;
typedef Point2<int> Point2i;
