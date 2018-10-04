#include "Imaging.h"
// ---------------------------------------------------------------------------
Color::Color()
    : m_red(0.0)
    , green(0.0)
    , blue(0.0)
{
}
// ---------------------------------------------------------------------------
Color::Color(
    const double _red,
    const double _green,
    const double _blue)
    : m_red(_red)
    , green(_green)
    , blue(_blue)
{ 
}
// ---------------------------------------------------------------------------
void Color::Validate() const
{
    if ((m_red < 0.0) || (green < 0.0) || (blue < 0.0))
    {
        throw ImagerException("Negative color values not allowed.");
    }
}
// ---------------------------------------------------------------------------
Color& Color::operator *= (const double factor)
{
    m_red   *= factor;
    green *= factor;
    blue  *= factor;
    return *this;
}
// ---------------------------------------------------------------------------
Color& Color::operator /= (const double factor)
{
    m_red   /= factor;
    green /= factor;
    blue  /= factor;
    return *this;
}
// ---------------------------------------------------------------------------
Color& Color::operator += (const Color& other)
{
    m_red   += other.m_red;
    green += other.green;
    blue  += other.blue;
    return *this;
}
// ---------------------------------------------------------------------------
Color& Color::operator -= (const Color& other)
{
    m_red   -= other.m_red;
    green -= other.green;
    blue  -= other.blue;
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
// ---------------------------------------------------------------------------
Color& ImageBuffer::PixelAt(size_t i, size_t j)
{
    if ((i < m_screenWidth) && (j < m_screenHeight))
    {
        return m_backBuffer[(j * m_screenWidth) + i];
    }

    throw ImagerException("Pixel coordinate out of bounds.");
}
// ---------------------------------------------------------------------------
