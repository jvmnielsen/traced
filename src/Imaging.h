#pragma once
#include <vector>
#include <cstddef>
#include "MathUtil.h"

// ---------------------------------------------------------------------------
float clamp(const float &lo, const float &hi, const float &v);

void ConvertToRGB(Vecf& color);

// ---------------------------------------------------------------------------
// Class for RGB color
struct Color
{
    float  m_red;
    float  m_green;
    float  m_blue;
    float  m_alpha;

    Color();
	Color(const float arg) : m_red(arg), m_green(arg), m_blue(arg), m_alpha(arg) {}
    Color(float red, float green, float blue);
    Color(float red, float green, float blue, float alpha);

    void Validate() const; // ensure color values are non-negative
	//void ConvertToRGBA();

    Color& operator *= (float factor);
    Color& operator /= (float factor);

    Color& operator += (const Color& other);
    Color& operator -= (const Color& other);

	float operator [] (const uint8_t i) const;
	float& operator [] (const uint8_t i);

};

inline Color operator * (const Color& aColor, const Color& bColor)
{
	return
	{
		aColor.m_red   * bColor.m_red,
		aColor.m_green * bColor.m_green,
		aColor.m_blue  * bColor.m_blue
	};
}

inline Color operator * (float scalar, const Color &color)
{
	return
	{
		scalar * color.m_red,
		scalar * color.m_green,
		scalar * color.m_blue
	};
}

inline Color operator + (const Color& a, const Color& b)
{
    return 
    {
        a.m_red   + b.m_red,
        a.m_green + b.m_green,
        a.m_blue  + b.m_blue
    };
}

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

