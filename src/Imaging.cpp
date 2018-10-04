#include "Imaging.h"
#include "MathUtil.h"
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
    const float alpha)
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
Color& Color::operator *= (const float factor)
{
    m_red   *= factor;
    m_green *= factor;
    m_blue  *= factor;
    return *this;
}
// ---------------------------------------------------------------------------
Color& Color::operator /= (const float factor)
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
    , m_channels(4)
	, m_bitsPerByte(8)
{
    m_buffer.reserve(screenWidth * screenHeight * m_channels);
}

void ImageBuffer::AddPixelAt(Color& color, size_t i, size_t j)
{
    if ((i < m_screenWidth) && (j < m_screenHeight))
    {
		color.ConvertToRGBA();
        m_buffer[(j * m_screenWidth) + i    ] = color.m_red;
		m_buffer[(j * m_screenWidth) + i + 1] = color.m_green;
		m_buffer[(j * m_screenWidth) + i + 2] = color.m_blue;
		m_buffer[(j * m_screenWidth) + i + 3] = color.m_blue;
    }

    throw ImagerException("Pixel coordinate out of bounds.");
}
// ---------------------------------------------------------------------------
void Color::ConvertToRGBA()
{
	m_red	= 255 * clamp(0, 1, m_red);
	m_green	= 255 * clamp(0, 1, m_green);
	m_blue	= 255 * clamp(0, 1, m_blue);
	m_alpha = 255 * clamp(0, 1, m_alpha);
}


float Color::operator [] (const uint8_t i) const
{
	return (&m_red)[i];
}

float& Color::operator [] (const uint8_t i)
{
	return (&m_red)[i];
}