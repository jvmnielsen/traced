#include "Window.h"
#include <vector>
#include "ImageBuffer.h"
#include "Vec3.h"
#include "Ray.h"
#include "Scene.h"
#include "Material.h"
#include "Sphere.h"
#include "Controller.h"

/*
Scene random_scene()
{
    int n = 500;
    const unsigned int SCREEN_WIDTH = 400;
    const unsigned int SCREEN_HEIGHT = 200;

    Scene scene{ SCREEN_WIDTH, SCREEN_HEIGHT };
    scene.m_scene_objects.push_back( new Sphere( Vec3f( 0, -1000, 0 ), 1000, new Lambertian( Vec3f( 0.5, 0.5, 0.5 ) ) ) );

    std::mt19937 gen{ std::random_device()() };
    std::uniform_real_distribution<> dist( 0, 1 );

    int i = 1;
    for (int a = -11; a < 11; a++)
    {
        for (int b = -11; b < 11; b++)
        {
            float choose_mat = dist( gen );
            Vec3f center( a + 0.9 * dist( gen ), 0.2, b + 0.9 * dist( gen ));
            if ((center - Vec3f( 4, 0.2, 0 )).Magnitude() > 0.9)
            {
                if (choose_mat < 0.8)
                {
                    scene.m_scene_objects.push_back( new Sphere( center, 0.2, new Lambertian( Vec3f( 
                        dist( gen ) * dist( gen ), 
                        dist( gen ) * dist( gen ), 
                        dist( gen ) * dist( gen ) ) ) ) );
                }
                else if (choose_mat < 0.95)
                {
                    scene.m_scene_objects.push_back( new Sphere( center, 0.2, new Metal( Vec3f( 0.5*(1 + dist( gen )),
                                                     0.5*(1 + dist( gen )), 0.5*(1 + dist( gen )) ) ) ) );
                }
                else
                {
                    scene.m_scene_objects.push_back( new Sphere(center, 0.2, new Dielectric(1.5)));
                }
            }
        }
    }


    scene.m_scene_objects.push_back( new Sphere( Vec3f( 0, 1, 0 ), 1.0, new Dielectric( 1.5 ) ) );
    scene.m_scene_objects.push_back( new Sphere( Vec3f( -4, 1, 0 ), 1.0, new Lambertian(Vec3f(0.4,0.2,0.1 ) ) ));
    scene.m_scene_objects.push_back( new Sphere( Vec3f( 4, 1, 0 ), 1.0, new Metal( Vec3f(0.7, 0.5, 0.5) ) ) );

    return scene;

    
} */

// arguments necessary for SDL to be multi-platform
int main( int argc, char * argv[] )
{
    const unsigned int SCREEN_WIDTH = 720;
    const unsigned int SCREEN_HEIGHT = 480;

    //Scene scene = random_scene();
    Scene scene{ SCREEN_WIDTH, SCREEN_HEIGHT };


    Controller controller{ scene };
    controller.run();
    //scene.render();

    return 0; 
}