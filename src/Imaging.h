#pragma once
#include <vector>
#include <cstddef>
#include "MathUtil.h"

// ---------------------------------------------------------------------------
float clamp(const float &lo, const float &hi, const float &v);

void ConvertToRGB(Vecf& color);



struct LightingInfo
{
    Vecf direction;
    Vecf intensity;
    float distance;
};

class Light
{
public:
    Light(const Vecf& color, float intensity = 1);

    virtual ~Light() = default;
    virtual void illuminate(const Vecf& point, LightingInfo& info) const = 0;

protected:
    Vecf m_color;
    float m_intensity;
};

class DistantLight : public Light
{
public:
    DistantLight(const Vecf& color, const float intensity, const Vecf& direction)
            : Light(color, intensity)
            , m_direction(direction)
    {}

    void illuminate(const Vecf& point, LightingInfo& info) const override
    {
        info.direction = m_direction;
        info.intensity = m_color * m_intensity;
        info.distance = -1;
    }

    Vecf m_direction;
};

class PointLight : public Light
{
public:
    PointLight() = default;

    PointLight(const Vecf& color, const float intensity, const Vecf& position)
            : Light(color, intensity)
            , m_position(position)
    {}

    void illuminate(const Vecf& point, LightingInfo& info) const override
    {
        info.direction = point - m_position;
        const auto r2 = info.direction.MagnitudeSquared();
        info.distance = sqrtf(r2);
        info.direction /= info.distance;
        info.intensity = m_intensity * m_color / (4 * M_PI * r2);
    }

    Vecf m_position;
};


class Camera
{
public:

	Camera() = default;
	~Camera() = default;

	Camera(float v_fov, float aspect);
	Camera(const Vecf& look_from, const Vecf& look_at, const Vecf& v_up, const float v_fov, const float aspect);

	Rayf GetRay(const float u, const float v) const;

private:
	Vecf m_origin;
	Vecf m_lowerLeftCorner;
	Vecf m_horizontal;
	Vecf m_vertical;
};

// ---------------------------------------------------------------------------
// Buffer for the image (back and front) whose job it is to transform 
// abstract buffer into raw RGB to be displayed
class ImageBuffer
{
public:
    ImageBuffer(size_t screenWidth, size_t screenHeight);

    void AddPixelAt(Vecf& color, size_t i, size_t j); // read/write for pixel at specified location

	//void AddPixel(const Color& color);

    size_t Width() const { return m_screenWidth; }
    size_t Height() const { return m_screenHeight; }

    int BitsPerByte() const { return m_bitsPerByte; }
	int Channels() const { return m_channels; }

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

