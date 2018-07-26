#include "PixelBuffer.h"

PixelBuffer::PixelBuffer( const int channels, 
                          const int screenWidth,
                          const int screenHeight )
    : m_channels( channels )
    , m_screenWidth( screenWidth )
    , m_screenHeight( screenHeight )
{
    m_pixel_data.reserve( m_screenWidth * m_screenHeight * m_channels );
    //pixels = unsigned char[m_screenWidth * m_screenHeight * m_channels];
}

void PixelBuffer::add_pixel( Pixel& pixel )
{
    m_pixel_data.push_back( pixel );
}
