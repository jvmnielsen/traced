#pragma once
#include <vector>
#include <cstddef>

// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// Class for RGB color
struct Color
{
    double  m_red;
    double  green;
    double  blue;
    double  alpha;

    Color();
    Color(double red, double green, double blue);

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
        aColor.m_red   * bColor.m_red,
        aColor.green * bColor.green,
        aColor.blue  * bColor.blue
    };
}

inline Color operator * (double scalar, const Color &color)
{
    return 
    {
        scalar * color.m_red,
        scalar * color.green,
        scalar * color.blue
    };
}

inline Color operator + (const Color& a, const Color& b)
{
    return 
    {
        a.m_red   + b.m_red,
        a.green + b.green,
        a.blue  + b.blue
    };
}

// ---------------------------------------------------------------------------
// Buffer for the image (back and front) whose job it is to transform 
// abstract buffer into raw RGB to be displayed
class ImageBuffer
{
public:
    ImageBuffer(size_t screenWidth, size_t screenHeight);

    Color& PixelAt(size_t i, size_t j); // read/write for pixel at specified location

    size_t Width() const { return m_screenWidth; }
    size_t Height() const { return m_screenHeight; }

    int BitsPerByte() const { return m_channels; }


private:
    size_t  m_screenWidth;                      // width of screen in pixels
    size_t  m_screenHeight;                     // height of screen in pixels 
    std::vector<Color>         m_backBuffer;    // array filled during rendering
    std::vector<unsigned char> m_screenBuffer;   // flattened raw RGB array used by SDL

    const int m_channels; // bits per byte, used by SDL
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

