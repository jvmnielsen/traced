#include "Window.h"
#include <vector>
#include "PixelBuffer.h"
#include "Vec3.h"
#include "Ray.h"
#include "Scene.h"

float hit_sphere( const Vec3f& center, const float radius, const Rayf& ray )
{
    Vec3f oc = ray.origin() - center;
    float a = dot( ray.direction(), ray.direction() );
    float b = 2.0 * dot( oc, ray.direction() );
    float c = dot( oc, oc ) - radius * radius;
    float discriminant = b * b - 4 * a * c;
    if (discriminant < 0)
    {
        return -1.0;
    }
    else
    {
        return (-b - sqrt( discriminant )) / (2.0 * a);
    }

}


Vec3f color( const Rayf& ray )
{
    float t = hit_sphere( Vec3f( 0, 0, -1 ), 0.5, ray );
    
    if (t > 0.0) {
        Vec3f N = unit_vector( ray.point_at_parameter( t ) - Vec3f( 0, 0, -1 ) );
        return 0.5 * Vec3f( N.m_x + 1, N.m_y + 1, N.m_z + 1 );
    }

    Vec3f unit_direction = unit_vector( ray.direction() );
    t = 0.5 * ( unit_direction.m_y + 1.0 );
    return (1.0 - t) * Vec3f( 1.0, 1.0, 1.0 ) + t * Vec3f( 0.5, 0.7, 1.0 );
}


// arguments necessary for SDL to be multi-platform
int main( int argc, char * argv[] )
{
    
    const int SCREEN_WIDTH = 1000;
    const int SCREEN_HEIGHT = 500;

    Scene scene{ Pixel( 255,255,255,255 ), 1000, 500 };
    scene.render();

    return 0; 
}