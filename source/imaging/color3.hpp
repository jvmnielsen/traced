#pragma once
#include <cstdint>

template<typename T>
class color3
{
public:
    T r, g, b;

    color3() : r(0), g(0), b(0) { }
    explicit color3(T val) : r(val), g(val), b(val) { }
    color3(T r_, T g_, T b_) : r(r_), g(g_), b(b_) { }

    color3 operator*(T factor) const
    {
        return color3{r * factor, g * factor, b * factor};
    }

    color3& operator*=(T factor)
    {
        r *= factor;
        g *= factor;
        b *= factor;
        return *this;
    }

    color3 operator/(T factor) const
    {
        return color3{r / factor, g / factor, b / factor};
    }

    color3& operator/=(T factor)
    {
        r /= factor;
        g /= factor;
        b /= factor;
        return *this;
    }

    color3 operator+(const color3& other) const
    {
        return color3{r + other.r, g + other.g, b + other.b};
    }

    color3& operator+=(const color3& other)
    {
        r += other.r;
        g += other.g;
        b += other.b;
        return *this;
    }

    color3 operator-(const color3& other) const
    {
        return color3{r - other.r, g - other.g, b - other.b};
    }

    color3& operator-=(const color3& other)
    {
        r -= other.r;
        g -= other.g;
        b -= other.b;
        return *this;
    }

    color3 operator*(const color3& other)
    {
        return {r * other.r, g * other.g, b*other.b};
    }

};

template<typename T>
color3<T> operator*(T factor, const color3<T>& color)
{
    return color3<T>{ color.r * factor, color.g * factor, color.b * factor };
}

template<typename T>
color3<T> operator/(T factor, const color3<T>& color)
{
    return color3<T>{ color.r * factor, color.g * factor, color.b * factor };
}


typedef color3<float> Color3f;
typedef color3<uint8_t > Color3ui8;
typedef color3<unsigned short> Color3ui16;