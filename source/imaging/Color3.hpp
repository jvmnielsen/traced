#pragma once
#include <cstdint>
#include "../math/math_util.hpp"

template<typename T>
class Color3
{
public:
    T r, g, b;

    Color3() : r(0), g(0), b(0) { }
    explicit Color3(T val) : r(val), g(val), b(val) { }
    Color3(T r_, T g_, T b_) : r(r_), g(g_), b(b_) { }

    static auto Black() -> Color3 { return Color3{0.0}; }

    Color3 operator*(T factor) const
    {
        return Color3{r * factor, g * factor, b * factor};
    }

    Color3& operator*=(T factor)
    {
        r *= factor;
        g *= factor;
        b *= factor;
        return *this;
    }

    Color3 operator/(T factor) const
    {
        return Color3{r / factor, g / factor, b / factor};
    }

    Color3& operator/=(T factor)
    {
        r /= factor;
        g /= factor;
        b /= factor;
        return *this;
    }

    Color3 operator+(const Color3& other) const
    {
        return Color3{r + other.r, g + other.g, b + other.b};
    }

    Color3& operator+=(const Color3& other)
    {
        r += other.r;
        g += other.g;
        b += other.b;
        return *this;
    }

    Color3 operator-(const Color3& other) const
    {
        return Color3{r - other.r, g - other.g, b - other.b};
    }

    Color3& operator-=(const Color3& other)
    {
        r -= other.r;
        g -= other.g;
        b -= other.b;
        return *this;
    }

    Color3 operator*(const Color3& other) const
    {
        return {r * other.r, g * other.g, b*other.b};
    }

    auto IsBlack() const -> bool {
        return r == 0 && g == 0 && b == 0;
    }

};

template<typename T>
Color3<T> operator*(T factor, const Color3<T>& color)
{
    return Color3<T>{ color.r * factor, color.g * factor, color.b * factor };
}

template<typename T>
Color3<T> operator/(T factor, const Color3<T>& color)
{
    return Color3<T>{ color.r * factor, color.g * factor, color.b * factor };
}

template<typename T>
auto ClampColor(Color3<T>& col) -> void
{
    col.r = Math::Clamp(1.0, 0.0, col.r);
    col.g = Math::Clamp(1.0, 0.0, col.g);
    col.b = Math::Clamp(1.0, 0.0, col.b);
}


typedef Color3<float> Color3f;
typedef Color3<uint8_t > Color3ui8;
typedef Color3<unsigned short> Color3ui16;