#include "PixelBuffer.h"

PixelBuffer::PixelBuffer( const int channels, 
                          const int screenWidth,
                          const int screenHeight )
    : m_channels( channels )
    , m_screenWidth( screenWidth )
    , m_screenHeight( screenHeight )
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

/*
void PixelBuffer::add_pixel( Pixel& pixel )
{
    m_pixel_data.push_back( pixel );
} */
