#include "image_buffer.hpp"

#include <algorithm>

using namespace tr;
using namespace gm; 

ImageBuffer::ImageBuffer(
    const size_t screen_width,
    const size_t screen_height)
    : m_screen_width(screen_width)
    , m_screen_height(screen_height)
    , m_channels(4)
    , m_bitsPerByte(8)
{
    auto const total = screen_width * screen_height * m_channels;
    m_buffer.reserve(total);

    for (unsigned int i = 0; i < total; i++) {
        m_buffer.emplace_back(190);
    }
}

// note it's safe to write concurrently to distinct elements of a std::vector
auto ImageBuffer::add_pixel_at(Color3f& color, int screen_x, int screen_y) -> void {
    const auto corrected_y = std::abs(screen_y - m_screen_height) - 1; // to correct for j starting at screen_height and decrementing

    if (screen_x < m_screen_width && screen_y < m_screen_height) {
        color.convert_to_rgb();
        m_buffer.at((corrected_y * m_screen_width + screen_x) * 4    ) = static_cast<unsigned char>(color.r);
        m_buffer.at((corrected_y * m_screen_width + screen_x) * 4 + 1) = static_cast<unsigned char>(color.g);
        m_buffer.at((corrected_y * m_screen_width + screen_x) * 4 + 2) = static_cast<unsigned char>(color.b);
        m_buffer.at((corrected_y * m_screen_width + screen_x) * 4 + 3) = 255;
    }
}
