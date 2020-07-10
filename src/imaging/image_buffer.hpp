#pragma once
#include "color3.hpp"
#include <vector>
#include <mutex>
#include "../math/point2.hpp"

struct ScreenSegment {
    ScreenSegment(Point2i lower, Point2i upper, size_t _index) : lowerBound(std::move(lower)), upperBound(std::move(upper)), index(_index) {}
    Point2i lowerBound;
    Point2i upperBound;
    size_t index;
};

class ImageBuffer
{
public:
    ImageBuffer(size_t screen_width, size_t screen_height);

    void add_pixel_at(Color3f& color, int screen_x, int screen_y); // read/write for pixel at specified location

    //void AddPixel(const Color& color);

    //auto render_segment(const ScreenSegment& segment) -> void;

    int get_width() { return m_screen_width; }
    int get_height() { return m_screen_height; }

    int bits_per_byte() const { return m_bitsPerByte; }
    int channels() const { return m_channels; }

    auto ConvertToPixelBuffer(std::vector<Color3f> colors) -> void;

    std::vector<unsigned char>* PtrToBuffer() { return &m_buffer; }
    std::vector<unsigned char> m_buffer;   // flattened raw RGB array used by SDL
private:
    int m_screen_width;                      // width of screen in pixels
    int m_screen_height;                     // height of screen in pixels 
    // std::vector<Color>         m_backBuffer;    // array filled during rendering

    std::mutex m_mutex;
    const int m_channels;		// number of channels per pixel (4 for RGBA)
    const int m_bitsPerByte;	// bits per byte, used by SDL
};
