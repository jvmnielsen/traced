#include "Scene.h"
#include "Window.h"
#include <float.h>
#include "Sphere.h"


void Scene::add_object_to_scene( Hitable& hitable )
{
    m_scene_objects.push_back( hitable );
}

bool Scene::intercepts( 
    const Rayf& ray,
    const float t_min,
    const float t_max,
    hit_record& record )
{
    hit_record temp_rec;
    bool hit_anything = false;
    double closest_so_far = t_max;
    for ( auto& hitable : m_scene_objects )
    {
        if (hitable.hit( ray, t_min, closest_so_far, temp_rec ))
        {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            record = temp_rec;
        }
    }
    return hit_anything;
}

Vec3f Scene::color( const Rayf& ray )
{
    hit_record record;
    if ( intercepts( ray, 0.0, FLT_MAX, record ) )
    {
        return 0.5 * Vec3f(
            record.normal.m_x + 1,
            record.normal.m_y + 1,
            record.normal.m_z + 1 );
    }
    else
    {
        Vec3f unit_direction = unit_vector( ray.direction() );
        float t = 0.5 * (unit_direction.m_y + 1.0);
        return (1.0 - t) * Vec3f( 1.0, 1.0, 1.0 ) + t * Vec3f( 0.5, 0.7, 1.0 );
    }
}


void Scene::render()
{
    Window window = Window( m_screen_width, m_screen_height );
    window.initializeWindow();

    PixelBuffer buffer = PixelBuffer( 4, m_screen_width, m_screen_height );

    Vec3f lower_left_cornor( -2.0, -1.0, -1.0 );
    Vec3f horizontal( 4.0, 0.0, 0.0 );
    Vec3f vertical( 0.0, 2.0, 0.0 );
    Vec3f origin( 0.0, 0.0, 0.0 );
    
    add_object_to_scene( Sphere( Vec3f( 0,0,-1 ), 0.5) );
    add_object_to_scene( Sphere( Vec3f( 0, -100.5, -1 ), 100 ) );
    
    for (int j = m_screen_height - 1; j >= 0; j--)
    {
        for (int i = 0; i < m_screen_width; i++)
        {
            
        }
    }



    window.renderPixelBuffer( buffer );
}