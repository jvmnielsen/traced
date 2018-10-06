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


Light::Light(const Vecf& color, const float intensity)
        : m_color(color)
        , m_intensity(intensity)
{
}

Camera::Camera(const float v_fov, const float aspect)
{
    const auto theta = v_fov * M_PI / 180; // degrees to radians
    const auto half_height = tan(theta / 2);
    const auto half_width = aspect * half_height;
    m_lowerLeftCorner = Vecf(-half_width, -half_height, -1.0);
    m_horizontal = Vecf(2 * half_width, 0.0f, 0.0f);
    m_vertical = Vecf(0.0f, 2 * half_height, 0.0f);
    m_origin = Vecf(0.0f);
}

Camera::Camera(
        const Vecf& look_from,
        const Vecf& look_at,
        const Vecf& v_up,
        const float v_fov,
        const float aspect)
{
    const auto theta = v_fov * M_PI / 180;
    const auto half_height = tan(theta / 2);
    const auto half_width = aspect * half_height;

    m_origin = look_from;

    const auto w = UnitVector(look_from - look_at);
    const auto u = UnitVector(CrossProduct(v_up, w));
    const auto v = CrossProduct(w, u);

    m_lowerLeftCorner = m_origin - half_width * u - half_height * v - w;
    m_horizontal = 2 * half_width * u;
    m_vertical = 2 * half_height * v;
}

Rayf Camera::GetRay(const float u, const float v) const
{
    return {m_origin,
            m_lowerLeftCorner + u * m_horizontal + v * m_vertical - m_origin, PrimaryRay};
}

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

