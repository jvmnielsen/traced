#pragma once
#include <cmath>
#include <iostream>

template<typename T>
class Vec2
{
public:
    T x, y;

    // Constructors
    Vec2() {}
    explicit Vec2(T val) : x(val), y(val) {}
    Vec2(T x_, T y_, T z_) : x(x_), y(y_) {}

    auto LengthSquared() const -> T
    {
        return x * x + y * y;
    }

    auto Length() const -> T
    {
        return std::sqrt(LengthSquared());
    }

    auto operator*(const T factor) const -> Vec2
    {
        return Vec2{x * factor, y * factor};
    }

    auto operator*=(const T factor) -> Vec2&
    {
        x *= factor;
        y *= factor;
        return *this;
    }

    auto operator/(const T factor) const -> Vec2
    {
        return Vec2{x / factor, y / factor};
    }

    auto operator/=(const T factor) -> Vec2&
    {
        x /= factor;
        y /= factor;
        return *this;
    }

    auto operator+(const Vec2 &other) const -> Vec2
    {
        return Vec2{x + other.x, y + other.y};
    }

    auto operator+=(const Vec2 &other) -> Vec2
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    constexpr Vec2 operator-(const Vec2 &other) const
    {
        return Vec2{x - other.x, y - other.y};
    }

    constexpr Vec2& operator-=(const Vec2 &other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    constexpr T DotProduct(const Vec2& other) const
    {
        return x * other.x + y * other.y;
    }

    constexpr Vec2 CrossProduct(const Vec2& other) const
    {
        return Vec2<T>{ x * other.y - other.x * y };
    }

    constexpr Vec2& Normalize()
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

    constexpr Vec2 operator-() const
    {
        return {-x, -y};
    }

    constexpr bool operator==(const Vec2& other)
    {
        return x == other.x && y == other.y;
    }

    constexpr bool operator!=(const Vec2& other)
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
Vec2<T> operator*(const T factor, const Vec2<T>& vec)
{
    return {vec.x * factor, vec.y * factor};
}

template<typename T>
Vec2<T> operator/(const T factor, const Vec2<T>& vec)
{
    return {vec.x / factor, vec.y / factor,};
}


typedef Vec2<float> Vec2f;
typedef Vec2<int> Vec2i;
typedef Vec2<double> Vec2d;