#include "Imaging.h"
#include "MathUtil.h"
#include <algorithm>
#include <cmath>
// ---------------------------------------------------------------------------
inline
float clamp(const float &lo, const float &hi, const float &v)
{
	return std::max(lo, std::min(hi, v));
}

void ConvertToRGB(Vecf& color)
{
	color.r() = 255 * clamp(0, 1, color.r());
	color.g() = 255 * clamp(0, 1, color.g());
	color.b() = 255 * clamp(0, 1, color.b());
}

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
	const auto total = screenWidth * screenHeight * m_channels;
    m_buffer.reserve(total);

	for (unsigned int i = 0; i < total; i++)
	{
		m_buffer.emplace_back(255);
	}
}

void ImageBuffer::AddPixelAt(Vecf& color, size_t i, size_t j)
{
	auto corrected_j = std::abs((int)j - (int)m_screenHeight) - 1; // to correct for j starting at screen_height and decrementing

    if ((i < m_screenWidth) && (j < m_screenHeight))
    {
		ConvertToRGB(color);
        m_buffer[(corrected_j * m_screenWidth + i) * 4    ] = color.r();
		m_buffer[(corrected_j * m_screenWidth + i) * 4 + 1] = color.g();
		m_buffer[(corrected_j * m_screenWidth + i) * 4 + 2] = color.b();
		m_buffer[(corrected_j * m_screenWidth + i) * 4 + 3] = 255;
    }
	else
	{
		throw ImagerException("Pixel coordinate out of bounds.");
	}
}
// ---------------------------------------------------------------------------




float Color::operator [] (const uint8_t i) const
{
	return (&m_red)[i];
}

float& Color::operator [] (const uint8_t i)
{
	return (&m_red)[i];
}