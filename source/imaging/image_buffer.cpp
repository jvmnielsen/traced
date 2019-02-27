#include <cmath>
#include "image_buffer.hpp"
#include "../math/math_util.hpp"

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

    color.r = 255 * Math::Clamp(0.0, 1.0, color.r);
    color.g = 255 * Math::Clamp(0.0, 1.0, color.g);
    color.b = 255 * Math::Clamp(0.0, 1.0, color.b);
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

    for (unsigned int i = 0; i < total; i++) {
        m_buffer.emplace_back(190);
    }
}


auto
ImageBuffer::ConvertToPixelBuffer(std::vector<Color3f> colors) -> void {
   
    for (int i = 0; i < colors.size(); ++i) {
        ConvertToRGB(colors[i]);
        m_buffer[i * 4    ] = colors[i].r;
        m_buffer[i * 4 + 1] = colors[i].g;
        m_buffer[i * 4 + 2] = colors[i].b;
        m_buffer[i * 4 + 3] = 255;
    }

}

auto
ImageBuffer::AddPixelAt(Color3f& color, size_t screenX, size_t screenY) -> void
{
    auto correctedY = std::abs((int)screenY - (int)m_screenHeight) - 1; // to correct for j starting at screen_height and decrementing

    if ((screenX < m_screenWidth) && (screenY < m_screenHeight)) {
        ConvertToRGB(color);
        m_buffer[(correctedY * m_screenWidth + screenX) * 4    ] = static_cast<unsigned char>(color.r);
        m_buffer[(correctedY * m_screenWidth + screenX) * 4 + 1] = static_cast<unsigned char>(color.g);
        m_buffer[(correctedY * m_screenWidth + screenX) * 4 + 2] = static_cast<unsigned char>(color.b);
        m_buffer[(correctedY * m_screenWidth + screenX) * 4 + 3] = 255;
    } // maybe add throw
}