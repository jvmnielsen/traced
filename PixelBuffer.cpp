#include "PixelBuffer.h"

PixelBuffer::PixelBuffer( const std::vector<unsigned char> pixelData, 
                          const int channels, 
                          const int screenWidth,
                          const int screenHeight )
    : m_pixelData( pixelData )
    , m_channels( channels )
    , m_screenWidth( screenWidth )
    , m_screenHeight( screenHeight )
{
    m_pixelData.reserve( m_screenWidth * m_screenHeight * m_channels );
    //pixels = unsigned char[m_screenWidth * m_screenHeight * m_channels];
}

PixelBuffer::~PixelBuffer()
{
}
