#include "Scene.h"
#include "Window.h"
//#include <float.h>
#include "Sphere.h"
#include "Camera.h"
#include "Material.h"
#include "Matrix44.h"


inline float deg_to_rad(const float deg)
{
    return deg * M_PI / 280;
}


HitData Scene::trace( const Rayf& ray )
{
    HitData hit_data = { };

    for ( auto& renderable : m_scene_objects )
    {
        if ( renderable->is_hit_by(ray, hit_data.m_t) )
        {
            hit_data.update_closest_and_assign( renderable );
        }
    }

    return hit_data;
}



Vec3f Scene::cast_ray( const Rayf& ray )
{
    auto hit_data = trace( ray );

    if ( hit_data.has_been_hit() )
    {
        return hit_data.m_renderable_ptr->m_surface_color;
    }

    return {250, 110, 10};
}


void Scene::render( PixelBuffer& buffer )
{
    Matrix44f camera_to_world;

    const float fov = 90;

    const auto scale = tan( deg_to_rad( fov * 0.5 ) );

    const auto image_aspect_ratio = m_screen_width / m_screen_height;

    const auto origin = camera_to_world.multiply_with_point( Vec3f(0) );

    int counter = 0;

    for (size_t j = 0; j < m_screen_height; ++j)
    {
        for (size_t i = 0; i < m_screen_width; ++i)
        {
            const float x = (2 * (i + 0.5) / m_screen_width - 1) * image_aspect_ratio * scale;
            const float y = (1 - 2 * (j + 0.5) / m_screen_height) * scale;

            auto dir = camera_to_world.multiply_with_point( Vec3f( x, y, -1 ) );
            dir.normalize();

            auto color = cast_ray( Rayf( origin, dir ) );

            buffer.m_pixel_data[counter++] = color[0];
            buffer.m_pixel_data[counter++] = color[1];
            buffer.m_pixel_data[counter++] = color[2];
            buffer.m_pixel_data[counter++] = 255;
        }
    }


    
}