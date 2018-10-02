#include "PixelBuffer.h"

PixelBuffer::PixelBuffer( const int channels, 
                          const int screenWidth,
                          const int screenHeight )
    : m_channels( channels )
    , m_screenWidth( screenWidth )
    , m_screenHeight( screenHeight )
    , m_counter(0)
{
    int screen_size = m_screenWidth * m_screenHeight * channels;
    m_pixel_data.reserve( screen_size );

    //Pixel white_pixel{ 255, 255, 255, 255 };

    for (int i = 0; i < screen_size; ++i)
    {
        m_pixel_data.push_back( 255 );
    }
    //pixels = unsigned char[m_screenWidth * m_screenHeight * m_channels];
}

void color_clamp(Vec3f& color)
{
    for (auto i = 0; i < 3; i++)
    {
        if (color[i] > 255)
            color[i] = 255;

        if (color[i] < 0)
            color[i] = 0;
    }
}

void PixelBuffer::add_pixel(Vec3f& pixel)
{
    color_clamp(pixel);

    for (int i = 0; i < 3; i++)
    {
        m_pixel_data[m_counter++] = (unsigned char)pixel[i];
    }
    m_pixel_data[m_counter++] = 255; // alpha
} 
