#pragma once
#include <vector>
#include <cstddef>
#include "MathUtil.h"

template<typename T>
class Color3
{
public:
    T r, g, b;

    constexpr Color3() : r(0), g(0), b(0) { }
    constexpr explicit Color3(T val) : r(val), g(val), b(val) { }
    constexpr Color3(T r_, T g_, T b_) : r(r_), g(g_), b(b_) { }

    constexpr Color3 operator*(T factor) const
    {
        return Color3{ r * factor, g * factor, b * factor };
    }

    constexpr Color3& operator*=(T factor)
    {
        r *= factor;
        g *= factor;
        b *= factor;
        return *this;
    }

    constexpr Color3 operator/(T factor) const
    {
        return Color3{ r * factor, g * factor, b * factor };
    }

    constexpr Color3& operator/=(T factor)
    {
        r /= factor;
        g /= factor;
        b /= factor;
        return *this;
    }

    constexpr Color3 operator+(const Color3& other) const
    {
        return Color3{ r + other.r, g + other.g, b + other.b };
    }

    constexpr Color3& operator+=(const Color3& other)
    {
        r += other.r;
        g += other.g;
        b += other.b;
        return *this;
    }

    constexpr Color3 operator-(const Color3& other) const
    {
        return Color3{ r - other.r, g - other.g, b - other.b };
    }

    constexpr Color3& operator-=(const Color3& other)
    {
        r -= other.r;
        g -= other.g;
        b -= other.b;
        return *this;
    }

    constexpr Color3 operator*(const Color3& other)
    {
        return { r * other.r, g * other.g, b*other.b };
    }

};

template<typename T>
constexpr Color3<T> operator*(T factor, const Color3<T>& color)
{
    return Color3<T>{ color.r * factor, color.g * factor, color.b * factor };
}

template<typename T>
constexpr Color3<T> operator/(T factor, const Color3<T>& color)
{
    return Color3<T>{ color.r * factor, color.g * factor, color.b * factor };
}


typedef Color3<float> Color3f;
typedef Color3<uint8_t > Color3ui8;
typedef Color3<unsigned short> Color3ui16;


struct LightingInfo
{
    Vec3f direction;
    Color3f intensity;
    float distance;
};

class Light
{
public:
    Light(const Color3f& color, float intensity = 1);

    virtual ~Light() = default;
    virtual void illuminate(const Point3f& point, LightingInfo& info) const = 0;

protected:
    Color3f m_color;
    float m_intensity;
};

class PointLight : public Light
{
public:
    PointLight() = default;

    PointLight(const Color3f& color, const float intensity, const Point3f& position)
            : Light(color, intensity)
            , m_position(position)
    {}

    void illuminate(const Point3f& point, LightingInfo& info) const override
    {
        info.direction = point - m_position;
        const auto r2 = info.direction.LengthSquared();
        info.distance = sqrtf(r2);
        info.direction /= info.distance;
        //info.intensity = m_intensity * m_color / (4 * (float)M_PI * r2);
    }

    Point3f m_position;
};


class Camera
{
public:

	Camera() = default;
	~Camera() = default;

	Camera(float v_fov, float aspect);
	Camera(const Point3f& look_from, const Point3f& look_at, const Vec3f& v_up, const float v_fov, const float aspect);

	Rayf GetRay(const float u, const float v) const;

private:
	Point3f m_origin;
	Point3f m_lowerLeftCorner;
	Vec3f m_horizontal;
	Vec3f m_vertical;
};

// ---------------------------------------------------------------------------
// Buffer for the image (back and front) whose job it is to transform 
// abstract buffer into raw RGB to be displayed
class ImageBuffer
{
public:
    ImageBuffer(size_t screenWidth, size_t screenHeight);

    void AddPixelAt(Color3f& color, size_t i, size_t j); // read/write for pixel at specified location

	//void AddPixel(const Color& color);

    constexpr size_t Width() { return m_screenWidth; }
    constexpr size_t Height() { return m_screenHeight; }

    constexpr int BitsPerByte() { return m_bitsPerByte; }
	constexpr int Channels() { return m_channels; }

	std::vector<unsigned char>* PtrToBuffer() { return &m_buffer; }
	std::vector<unsigned char> m_buffer;   // flattened raw RGB array used by SDL
private:
    size_t  m_screenWidth;                      // width of screen in pixels
    size_t  m_screenHeight;                     // height of screen in pixels 
    // std::vector<Color>         m_backBuffer;    // array filled during rendering
    

    const int m_channels;		// number of channels per pixel (4 for RGBA)
	const int m_bitsPerByte;	// bits per byte, used by SDL
};

// ---------------------------------------------------------------------------
// Exception thrown by Imager classes
class ImagerException
    : public std::exception
{
public:
    explicit ImagerException(const char* _message)
        : message(_message)
    {
    }

    const char* GetMessage() const { return message; }

private:
    const char* message;
};
// ---------------------------------------------------------------------------

