#pragma once

#include <graphics-math.hpp>

#include <vector>
#include <mutex>

namespace tr {

    struct ScreenSegment {
        ScreenSegment(gm::Point2i lower, gm::Point2i upper, size_t _index) : lowerBound(std::move(lower)), upperBound(std::move(upper)), index(_index) {}
        gm::Point2i lowerBound;
        gm::Point2i upperBound;
        size_t index;
    };

    class ImageBuffer {
    public:
        ImageBuffer(size_t screen_width, size_t screen_height);

        void add_pixel_at(gm::Color3f& color, int screen_x, int screen_y); // read/write for pixel at specified location

        auto width()  const -> int { return m_screen_width; }
        auto height() const -> int { return m_screen_height; }

        std::vector<unsigned char> m_buffer;   // flattened raw RGB array used by SDL
    private:
        int m_screen_width;                      // width of screen in pixels
        int m_screen_height;                     // height of screen in pixels
        std::mutex m_mutex;
        const int m_channels;		// number of channels per pixel (4 for RGBA)
        const int m_bitsPerByte;	// bits per byte, used by SDL
    };

}