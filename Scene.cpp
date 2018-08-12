#include "Scene.h"
#include "Window.h"
//#include <float.h>
#include "Sphere.h"
#include "Camera.h"
#include "Material.h"
#include "Matrix44.h"
#include "Polygon.h"


inline float deg_to_rad(const float deg)
{
    return deg * M_PI / 280;
}


HitData Scene::trace( const Rayf& ray )
{
    HitData hit_data = { };

    for ( auto& renderable : m_scene_objects )
    {
        if ( renderable->intersects(ray, hit_data.m_t) )
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
    Matrix44f camera_to_world;// { 0.945519f, 0.0f, -0.325569f, 0.0f, -0.179534f, 0.834209f, -0.521403f, 0.0f, 0.271593f, 0.551447f, 0.78876f, 0.0f, 4.208271f, 8.374532f, 17.932925f, 1.0f };

    const float fov = 90;

    const auto scale = tan( deg_to_rad( fov * 0.5 ) );

    const auto image_aspect_ratio = m_screen_width / static_cast<float>(m_screen_height); // without cast the decimals are discarded resulting in distortion

    const auto origin = camera_to_world.multiply_with_point( Vec3f(0) );

    int counter = 0;

    m_scene_objects.push_back( std::make_shared<Polygon>( Polygon( Vec3f{ -0.1f, -0.1f, -2.0f }, Vec3f{ 0.1f, -0.1f, -2.0f }, Vec3f{ 0.0f, 0.1f, -2.0f}) ) );

    for (size_t j = 0; j < m_screen_height; ++j)
    {
        for (size_t i = 0; i < m_screen_width; ++i)
        {
            /* We first convert from raster space to NDC space by dividing the relevant coordinate
             * with either length or width of the screen. This results in all coordinates being 
             * in range [0,1]. The 0.5 is added to shift the ray to the center of each pixel.
             *
             * We then remap to screen space by: 2 * NDC - 1
             * This shifts the range to [-1,1]. The expression is subtracted from 1 in y's case
             * to flip the y-axis (due to (0,0) being in the top left hand corner in raster space).
             * 
             * For the x-coordinate we multiply with the aspect ratio to correct for the width of the screen.
             * 
             * Lastly the screen is scaled by the FOV, resulting in a uniform zoom.
             */

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