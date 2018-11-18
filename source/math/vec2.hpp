#pragma once
#include <cmath>
#include <iostream>

template<typename T>
class vec2
{
public:
    T x, y;

    // Constructors
    vec2() {}
    explicit vec2(T val) : x(val), y(val) {}
    vec2(T x_, T y_, T z_) : x(x_), y(y_) {}

    auto LengthSquared() const -> T
    {
        return x * x + y * y;
    }

    auto Length() const -> T
    {
        return std::sqrt(LengthSquared());
    }

    auto operator*(const T factor) const -> vec2
    {
        return vec2{x * factor, y * factor};
    }

    auto operator*=(const T factor) -> vec2&
    {
        x *= factor;
        y *= factor;
        return *this;
    }

    auto operator/(const T factor) const -> vec2
    {
        return vec2{x / factor, y / factor};
    }

    auto operator/=(const T factor) -> vec2&
    {
        x /= factor;
        y /= factor;
        return *this;
    }

    auto operator+(const vec2 &other) const -> vec2
    {
        return vec2{x + other.x, y + other.y};
    }

    auto operator+=(const vec2 &other) -> vec2
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    constexpr vec2 operator-(const vec2 &other) const
    {
        return vec2{x - other.x, y - other.y};
    }

    constexpr vec2& operator-=(const vec2 &other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    constexpr T DotProduct(const vec2& other) const
    {
        return x * other.x + y * other.y;
    }

    constexpr vec2 CrossProduct(const vec2& other) const
    {
        return vec2<T>{ x * other.y - other.x * y };
    }

    constexpr vec2& Normalize()
    {
        T length = this->Length();
        if (length > 0) // avoid division by 0
        {
            T invertedLength = 1 / length;
            x *= invertedLength;
            y *= invertedLength;
        }
        return *this;
    }

    constexpr vec2 operator-() const
    {
        return {-x, -y};
    }

    constexpr bool operator==(const vec2& other)
    {
        return x == other.x && y == other.y;
    }

    constexpr bool operator!=(const vec2& other)
    {
        return x != other.x || y != other.y;
    }

    // Accessors
    constexpr T operator[](const uint8_t i) const
    {
        return (&x)[i];
    }

    constexpr T& operator[](const uint8_t i)
    {
        return (&x)[i];
    }

    void PrettyPrint() const
    {
        std::cout << "[ " << x << ", " << y << " ]\n";
    }
};

/*
template<typename T>
constexpr Vec2<T> Normalize(const Vec2<T>& vec)
{
    Vec2<T> tmp;

    T invertedLength = 1 / vec.Length();

    tmp.x *= invertedLength;
    tmp.y *= invertedLength;

    return tmp;
} */


template<typename T>
vec2<T> operator*(const T factor, const vec2<T>& vec)
{
    return {vec.x * factor, vec.y * factor};
}

template<typename T>
vec2<T> operator/(const T factor, const vec2<T>& vec)
{
    return {vec.x / factor, vec.y / factor,};
}


typedef vec2<float> Vec2f;
typedef vec2<int> Vec2i;
typedef vec2<double> Vec2d;