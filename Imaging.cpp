#include "Imaging.h"
// ---------------------------------------------------------------------------
Color::Color()
    : red(0.0)
    , green(0.0)
    , blue(0.0)
{
}
// ---------------------------------------------------------------------------
Color::Color(
    const double _red,
    const double _green,
    const double _blue)
    : red(_red)
    , green(_green)
    , blue(_blue)
{ 
}
// ---------------------------------------------------------------------------
void Color::Validate() const
{
    if ((red < 0.0) || (green < 0.0) || (blue < 0.0))
    {
        throw ImagerException("Negative color values not allowed.");
    }
}
// ---------------------------------------------------------------------------
Color& Color::operator *= (const double factor)
{
    red   *= factor;
    green *= factor;
    blue  *= factor;
    return *this;
}
// ---------------------------------------------------------------------------
Color& Color::operator /= (const double factor)
{
    red   /= factor;
    green /= factor;
    blue  /= factor;
    return *this;
}
// ---------------------------------------------------------------------------
Color& Color::operator += (const Color& other)
{
    red   += other.red;
    green += other.green;
    blue  += other.blue;
    return *this;
}
// ---------------------------------------------------------------------------
Color& Color::operator -= (const Color& other)
{
    red   -= other.red;
    green -= other.green;
    blue  -= other.blue;
    return *this;
}
// ---------------------------------------------------------------------------
ImageBuffer::ImageBuffer(
    const int _screenWidth, 
    const int _screenHeight)
    : screenWidth(_screenWidth)
    , screenHeight(_screenHeight)
{
    storage.reserve(screenWidth * screenHeight); // reserve the total screen size for performance
}
// ---------------------------------------------------------------------------
Color& ImageBuffer::PixelAt(size_t i, size_t j)
{
    if ((i < screenWidth) && (j < screenHeight))
    {
        return storage[(j * screenWidth) + i];
    }

    throw ImagerException("Pixel coordinate out of bounds.");
}
// ---------------------------------------------------------------------------
