#pragma once
#include <vector>
#include <cstddef>

struct PixelBuffer
{
    PixelBuffer( const std::vector<unsigned char> pixelData, const int channels, const int screenWidth, const int screenHeight );
    
    ~PixelBuffer();

    std::vector<unsigned char> m_pixelData;
    unsigned char* pixels;
    
    // how many bits per pixel (24 or 32 most commonly for RGB and RGBA respectively)
    const int m_channels;

    const int m_screenWidth;
    const int m_screenHeight;
};

