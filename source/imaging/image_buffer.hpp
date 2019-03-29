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
    ImageBuffer(size_t screenWidth, size_t screenHeight);

    void AddPixelAt(Color3f& color, size_t screenX, size_t screenY); // read/write for pixel at specified location

    //void AddPixel(const Color& color);

    //auto render_segment(const ScreenSegment& segment) -> void;

    constexpr size_t GetWidth() { return m_screenWidth; }
    constexpr size_t GetHeight() { return m_screenHeight; }

    constexpr int BitsPerByte() { return m_bitsPerByte; }
    constexpr int Channels() { return m_channels; }

    auto ConvertToPixelBuffer(std::vector<Color3f> colors) -> void;

    std::vector<unsigned char>* PtrToBuffer() { return &m_buffer; }
    std::vector<unsigned char> m_buffer;   // flattened raw RGB array used by SDL
private:
    size_t  m_screenWidth;                      // width of screen in pixels
    size_t  m_screenHeight;                     // height of screen in pixels 
    // std::vector<Color>         m_backBuffer;    // array filled during rendering

    std::mutex m_mutex;
    const int m_channels;		// number of channels per pixel (4 for RGBA)
    const int m_bitsPerByte;	// bits per byte, used by SDL
};
