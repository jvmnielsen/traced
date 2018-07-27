#include "Window.h"
#include <vector>
#include "PixelBuffer.h"
#include "Vec3.h"
#include "Ray.h"
#include "Scene.h"

// arguments necessary for SDL to be multi-platform
int main( int argc, char * argv[] )
{
    const unsigned int SCREEN_WIDTH = 500;
    const unsigned int SCREEN_HEIGHT = 250;

    Scene scene{ Pixel( 255, 255, 255, 255 ), SCREEN_WIDTH, SCREEN_HEIGHT };
    scene.render();

    return 0; 
}