#include "Imaging.h"
#include "MathUtil.h"
#include <algorithm>
#include <cmath>
// ---------------------------------------------------------------------------

void GammaEncode(Color3f& color, float gamma)
{
    float gammaExponent = 1 / gamma;
    color.r = std::pow(color.r, gammaExponent);
    color.g = std::pow(color.g, gammaExponent);
    color.b = std::pow(color.b, gammaExponent);
}


void ConvertToRGB(Color3f& color)
{
    GammaEncode(color, 2.2f);

	color.r = 255 * Math::Clamp(0.0f, 1.0f, color.r);
	color.g = 255 * Math::Clamp(0.0f, 1.0f, color.g);
	color.b = 255 * Math::Clamp(0.0f, 1.0f, color.b);
}

Camera::Camera(const float v_fov, const float aspect)
{
    const auto theta = v_fov * M_PI / 180; // degrees to radians
    const auto half_height = static_cast<float>(tan(theta / 2));
    const auto half_width = aspect * half_height;
    m_lowerLeftCorner = Point3f(-half_width, -half_height, -1.0f);
    m_horizontal = Vec3f(2.0f * half_width, 0.0f, 0.0f);
    m_vertical = Vec3f(0.0f, 2.0f * half_height, 0.0f);
    m_origin = Point3f(0.0f);
}

Camera::Camera(
        const Point3f& look_from,
        const Point3f& look_at,
        const Vec3f& v_up,
        const float v_fov,
        const float aspect)
{
    const auto theta = Math::DegreeToRadian(v_fov);
    const auto halfHeight = tan(theta / 2);
    const auto halfWidth = aspect * halfHeight;

    m_origin = look_from;

    const auto w = (look_from - look_at).Normalize();
    const auto u = (v_up.CrossProduct(w)).Normalize();
    const auto v = w.CrossProduct(u);

    m_lowerLeftCorner = m_origin - u*halfWidth  - v*halfHeight - w;
    m_horizontal = u * 2 * halfWidth;
    m_vertical = v * 2 * halfHeight;
}

Rayf Camera::GetRay(const float u, const float v) const
{
    return {m_origin, // origin of the camera
            (m_lowerLeftCorner + m_horizontal * u +  m_vertical * v) - m_origin // scale from lower left - origin for vector pointing to this point
            };
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

auto ImageBuffer::AddPixelAt(Color3f& color, size_t i, size_t j) -> void
{
	auto corrected_j = std::abs((int)j - (int)m_screenHeight) - 1; // to correct for j starting at screen_height and decrementing

    if ((i < m_screenWidth) && (j < m_screenHeight))
    {
		ConvertToRGB(color);
        m_buffer[(corrected_j * m_screenWidth + i) * 4    ] = static_cast<unsigned char>(color.r);
		m_buffer[(corrected_j * m_screenWidth + i) * 4 + 1] = static_cast<unsigned char>(color.g);
		m_buffer[(corrected_j * m_screenWidth + i) * 4 + 2] = static_cast<unsigned char>(color.b);
		m_buffer[(corrected_j * m_screenWidth + i) * 4 + 3] = 255;
    }
	else
	{
		throw ImagerException("Pixel coordinate out of bounds.");
	}
}
// ---------------------------------------------------------------------------

