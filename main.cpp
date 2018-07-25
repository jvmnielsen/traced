#include "Window.h"
#include <vector>
#include "PixelBuffer.h"
#include "Vec3.h"
#include "Ray.h"

Vec3f color( const Rayf& ray )
{
    Vec3f unit_direction = unit_vector( ray.direction() );
    float t = 0.5 * ( unit_direction.m_y + 1.0 );
    return Vec3f( 1.0, 1.0, 1.0 ) * (1.0 - t) + Vec3f( 0.5, 0.7, 1.0 ) * t;
}


// arguments necessary for SDL to be multi-platform
int main( int argc, char * argv[] )
{

    Window window = Window( 480, 720 );
    window.initializeWindow();

    std::vector<unsigned char> testVector;
    PixelBuffer buffer = PixelBuffer( testVector, 4, 480, 720 );

    /*
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
    } */

    int nx = 720;
    int ny = 480;

    Vec3f lower_left_corner{ -2.0, -1.0, -1.0 };
    Vec3f horizontal{ 4.0, 0.0, 0.0 };
    Vec3f vertical{ 0.0,2.0,0.0 };
    Vec3f origin{ 0.0, 0.0, 0.0 };

    for (int j = ny - 1; j >= 0; j--)
    {
        for (int i = 0; i < nx; i++)
        {
            float u = float( i ) / float( nx );
            float v = float( j ) / float( ny );
            Rayf r( origin, lower_left_corner + u * horizontal + v * vertical );
            Vec3f col = color( r );
            int ir = int( 255.99 * col[0] );
            buffer.m_pixelData.push_back( ir );
            int ig = int( 255.99 * col[1] );
            buffer.m_pixelData.push_back( ig );
            int ib = int( 255.99 * col[2] );
            buffer.m_pixelData.push_back( ib );
            buffer.m_pixelData.push_back( 255 );
        }
    }
    
    window.renderPixelBuffer( buffer );

    return 0;
}