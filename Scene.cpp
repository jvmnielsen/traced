#include "Scene.h"
#include "Window.h"
#include <float.h>
#include "Sphere.h"
#include "Camera.h"


/*
void Scene::add_object_to_scene( Hitable& hitable )
{
    m_scene_objects.push_back( hitable );
} */

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
        if (hitable->hit( ray, t_min, closest_so_far, temp_rec ))
        {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            record = temp_rec;
        }
    }
    return hit_anything;
}

Vec3f Scene::random_in_unit_sphere()
{
	Vec3f p;
	do
	{
		p = 2.0 * Vec3f( m_distribution(m_generator), m_distribution(m_generator), m_distribution(m_generator) ) - Vec3f(1,1,1);
	} while (p.length_squared() >= 1.0);
	return p;
}

Vec3f Scene::color( const Rayf& ray )
{
    hit_record record;
    if ( intercepts( ray, 0.0, FLT_MAX, record ) )
    {
		Vec3f target = record.p + record.normal + random_in_unit_sphere();
		return 0.5 * color( Rayf(record.p, target - record.p) );
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

	PixelBuffer buffer = PixelBuffer(4, m_screen_width, m_screen_height);
    
    m_scene_objects.push_back( new Sphere( Vec3f( 0,0,-1 ), 0.5) );
    m_scene_objects.push_back( new Sphere( Vec3f( 0, -100.5, -1 ), 100 ) );
    
    Vec3f lower_left_corner( -2.0, -1.0, -1.0 );
    Vec3f horizontal( 4.0, 0.0, 0.0 );
    Vec3f vertical( 0.0, 2.0, 0.0 );
    Vec3f origin( 0.0, 0.0, 0.0 );

	Camera camera;

    

    int ns = 100;

    for (int j = m_screen_height - 1; j >= 0; j--)
    {
        for (int i = 0; i < m_screen_width; i++)
        {
            Vec3f col( 0, 0, 0 );
            
            for (int s = 0; s < ns; s++)
            {
                float u = float( i + m_distribution(m_generator) ) / float( m_screen_width );
                float v = float( j + m_distribution(m_generator) ) / float( m_screen_height );
                Rayf ray = camera.get_ray( u, v );
                Vec3f p = ray.point_at_parameter( 2.0 );
                col += color( ray );
            }
            
            col /= float( ns );

			// gamma correction
			col = Vec3f(sqrt(col.m_x), sqrt(col.m_y), sqrt(col.m_z));

            int ir = int( 255.99 * col.m_x );
            buffer.m_pixel_data.push_back( ir );
            int ig = int( 255.99 * col.m_y );
            buffer.m_pixel_data.push_back( ig );
            int ib = int( 255.99 * col.m_z );
            buffer.m_pixel_data.push_back( ib );
            buffer.m_pixel_data.push_back( 255 );
        }
    }

    window.renderPixelBuffer( buffer );
}