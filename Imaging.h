#pragma once
#include <vector>
#include <cstddef>
#include "Vec3.h"


// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// Class for RGB color
struct Color
{
    double  red;
    double  green;
    double  blue;

    Color();
    Color(double _red, double _green, double _blue);

    void Validate() const; // ensure color values are non-negative

    Color& operator *= (double factor);
    Color& operator /= (double factor);

    Color& operator += (const Color& other);
    Color& operator -= (const Color& other);

};

inline Color operator * (const Color& aColor, const Color& bColor)
{
    return 
    {
        aColor.red   * bColor.red,
        aColor.green * bColor.green,
        aColor.blue  * bColor.blue
    };
}

inline Color operator * (double scalar, const Color &color)
{
    return 
    {
        scalar * color.red,
        scalar * color.green,
        scalar * color.blue
    };
}

inline Color operator + (const Color& a, const Color& b)
{
    return 
    {
        a.red   + b.red,
        a.green + b.green,
        a.blue  + b.blue
    };
}

// ---------------------------------------------------------------------------
// Buffer for the image while still being rendered
class ImageBuffer
{
public:
    ImageBuffer(int _screenWidth, int _screenHeight);

    Color& PixelAt(size_t i, size_t j); // read/write for pixel at specified location

    size_t GetWidth() const { return screenWidth; }
    size_t GetHeight() const { return screenHeight; }

    std::vector<unsigned char> m_pixel_data;

private:
    size_t  screenWidth;         // width of screen in pixels
    size_t  screenHeight;        // height of screen in pixels 
    std::vector<Color>  storage; // flattened array
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

