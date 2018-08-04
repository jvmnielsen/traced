#include "Scene.h"
#include "Window.h"
#include <float.h>
#include "Sphere.h"
#include "Camera.h"
#include "Material.h"
#include <future>

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
    // might be too expensive creating and destroying every call
	std::mt19937 generator{ std::random_device()() }; 
	std::uniform_real_distribution<> distribution( 0, 1 );

	Vec3f p;
	do
	{
		p = 2.0 * Vec3f( distribution(generator), distribution(generator), distribution(generator) ) - Vec3f(1,1,1);
	} while (p.length_squared() >= 1.0);
	return p;
}

Vec3f Scene::color( const Rayf& ray, int depth )
{
    hit_record record;
    if ( intercepts( ray, 0.001, FLT_MAX, record ) )
    {
		Rayf scattered;
		Vec3f attenuation;
		if (depth < 50 && record.mat_ptr->scatter(ray, record, attenuation, scattered))
		{
			return attenuation * color(scattered, depth + 1);
		}
		else
		{
			return Vec3f(0, 0, 0);
		}
    }
    else
    {
        Vec3f unit_direction = unit_vector( ray.direction() );
        float t = 0.5 * (unit_direction.m_y + 1.0);
        return (1.0 - t) * Vec3f( 1.0, 1.0, 1.0 ) + t * Vec3f( 0.5, 0.7, 1.0 );
    }
}


void Scene::render( PixelBuffer& buffer )
{
    m_scene_objects.push_back( new Sphere( Vec3f( 0,0,-1 ), 0.5, new Lambertian( Vec3f( 0.8, 0.3, 0.3 ) ) ) );
    m_scene_objects.push_back( new Sphere( Vec3f( 0, -100.5, -1 ), 100, new Lambertian( Vec3f( 0.8, 0.8, 0.0 ) ) ) );
	m_scene_objects.push_back( new Sphere( Vec3f( 1, 0, -1 ), 0.5, new Metal( Vec3f(0.8, 0.6, 0.2))));
	m_scene_objects.push_back( new Sphere( Vec3f( -1, 0, -1 ), 0.5, new Dielectric( 1.5 )));

    Vec3f lower_left_corner( -2.0, -1.0, -1.0 );
    Vec3f horizontal( 4.0, 0.0, 0.0 );
    Vec3f vertical( 0.0, 2.0, 0.0 );
    Vec3f origin( 0.0, 0.0, 0.0 );

	Vec3f look_from{ 3,3,2 };
	Vec3f look_at{ 0,0,-1 };
	float dist_to_focus = (look_from - look_at).length();
	float aperture = 2.0;
    Camera camera( look_from, look_at, Vec3f(0,1,0), 20, float(m_screen_width)/float(m_screen_height), aperture, dist_to_focus);

    

    int ns = 100;
    int counter = 0;
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
                col += color( ray, 0 );
            }
            
            col /= float( ns );

			// gamma correction
			col = Vec3f(sqrt(col.m_x), sqrt(col.m_y), sqrt(col.m_z));

            int ir = int( 255.99 * col.m_x );
            buffer.m_pixel_data[counter++] =  ir;
            int ig = int( 255.99 * col.m_y );
            buffer.m_pixel_data[counter++] = ig;
            int ib = int( 255.99 * col.m_z );
            buffer.m_pixel_data[counter++] = ib;
            buffer.m_pixel_data[counter++] = 255;

        }
    }

    
}