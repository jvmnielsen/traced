#include "Imaging.h"
// ---------------------------------------------------------------------------
Color::Color()
    : m_red(0.0)
    , m_green(0.0)
    , m_blue(0.0)
    , m_alpha(1.0)
{
}

Color::Color(
    const float red,
    const float green,
    const float blue)
    : m_red(red)
    , m_green(green)
    , m_blue(blue)
{
}

Color::Color(
    const float red,
    const float green,
    const float blue,
    const float alpha = 1.0)
    : m_red(red)
    , m_green(green)
    , m_blue(blue)
    , m_alpha(alpha)
{ 
}
// ---------------------------------------------------------------------------
void Color::Validate() const
{
    if ((m_red < 0.0) || (m_green < 0.0) || (m_blue < 0.0))
    {
        throw ImagerException("Negative color values not allowed.");
    }
}
// ---------------------------------------------------------------------------
Color& Color::operator *= (const double factor)
{
    m_red   *= factor;
    m_green *= factor;
    m_blue  *= factor;
    return *this;
}
// ---------------------------------------------------------------------------
Color& Color::operator /= (const double factor)
{
    m_red   /= factor;
    m_green /= factor;
    m_blue  /= factor;
    return *this;
}
// ---------------------------------------------------------------------------
Color& Color::operator += (const Color& other)
{
    m_red   += other.m_red;
    m_green += other.m_green;
    m_blue  += other.m_blue;
    return *this;
}
// ---------------------------------------------------------------------------
Color& Color::operator -= (const Color& other)
{
    m_red   -= other.m_red;
    m_green -= other.m_green;
    m_blue  -= other.m_blue;
    return *this;
}
// ---------------------------------------------------------------------------
ImageBuffer::ImageBuffer(
    const size_t screenWidth, 
    const size_t screenHeight)
    : m_screenWidth(screenWidth)
    , m_screenHeight(screenHeight)
    , m_channels(8) // default
{
    m_backBuffer.reserve(screenWidth * screenHeight); // reserve the total screen size for performance
}

Color& ImageBuffer::PixelAt(size_t i, size_t j)
{
    if ((i < m_screenWidth) && (j < m_screenHeight))
    {
        return m_backBuffer[(j * m_screenWidth) + i];
    }

    throw ImagerException("Pixel coordinate out of bounds.");
}
// ---------------------------------------------------------------------------
