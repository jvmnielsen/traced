#pragma once
#include <vector>
#include <cstddef>
#include "Pixel.h"
#include "Vec3.h"

class PixelBuffer
{
public:
    PixelBuffer( const int channels, const int screenWidth, const int screenHeight );
    PixelBuffer();


    void add_pixel( Pixel& pixel );
    const int channels() { return m_channels; }

    std::vector<unsigned char> m_pixel_data;

    //std::vector<Pixel> m_pixel_data;
    
    const int m_channels;

    const int m_screenWidth;
    const int m_screenHeight;
};

