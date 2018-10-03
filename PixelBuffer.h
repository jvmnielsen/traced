#pragma once
#include <vector>
#include <cstddef>
#include "Pixel.h"
#include "Vec3.h"

class PixelBuffer
{
public:
    PixelBuffer( const int channels, const int screenWidth, const int screenHeight );
    


    void add_pixel(Vec3f& pixel);
    //int channels() { return m_channels; }



    std::vector<unsigned char> m_pixel_data;

    //std::vector<Pixel> m_pixel_data;

    int m_counter = 0;

    const int m_channels;

    const int m_screenWidth;
    const int m_screenHeight;
};

