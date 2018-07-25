#include "Window.h"
#include <iostream>
#include <vector>
#include "PixelBuffer.h"
#include "Vec3.h"

// arguments necessary for SDL to be multi-platform
int main( int argc, char * argv[] )
{

    Vec3f vec = { 1.0, 2.0, 3.0 };
    std::cout << vec[2];




    Window window = Window( 400, 400 );
    window.initializeWindow();

    std::vector<unsigned char> testVector;
    PixelBuffer buffer = PixelBuffer( testVector, 4, 400, 400 );

    int nx = 400;
    int ny = 400;

    for (int j = ny - 1; j >= 0; j--)
    {
        for (int i = 0; i < nx; i++)
        {
            float r = float( i ) / float( nx );
            float g = float( j ) / float( ny );
            float b = 0.2;
            int ir = int( 255.99 * r );
            buffer.m_pixelData.push_back( ir );
            int ig = int( 255.99 * g );
            buffer.m_pixelData.push_back( ig );
            int ib = int( 255.99 * b );
            buffer.m_pixelData.push_back( ib );
            buffer.m_pixelData.push_back( 255 );
        }
    }

    /*
    for (int i = 0; i < 400 * 400; ++i)
    {
        buffer.m_pixelData.push_back( 100 );
        buffer.m_pixelData.push_back( 200 );
        buffer.m_pixelData.push_back( 200 );
        buffer.m_pixelData.push_back( 255 );
    }
    */
    window.renderPixelBuffer( buffer );

    return 0;
}