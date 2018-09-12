#include "Scene.h"
#include "Window.h"
//#include <float.h>
#include "Sphere.h"
#include "Camera.h"
#include "Material.h"
#include "Matrix44.h"
#include "Polygon.h"
#include "Parser.h"


inline float deg_to_rad(const float deg)
{
    return deg * M_PI / 280;
}

void Scene::add_object_to_scene(const std::shared_ptr<Renderable>& render_ptr)
{
	m_simple_scene_objects.emplace_back(render_ptr);
}

void Scene::load_objects_from_file(const std::string& file_name)
{
	Parser parser;
	const std::shared_ptr<Renderable> renderable_ptr = parser.parse(file_name);
	//add_object_to_scene(renderable_ptr);
    m_scene_meshes.push_back(renderable_ptr);
}

void Scene::trace_simple_objects(const Rayf& ray, HitData& hit_data)
{
    for (auto& renderable : m_simple_scene_objects)
    {
        if (renderable->intersects(ray, hit_data))
        {
            hit_data.update_closest(renderable);
        }
    }
}

void Scene::trace_meshes(const Rayf& ray, HitData& hit_data)
{
    for (auto& renderable : m_scene_meshes)
    {
        renderable->intersects(ray, hit_data); // meshes handle it themselves internally for now, find a better solution
    }
}

HitData Scene::trace(const Rayf& ray)
{
    HitData hit_data;

    if (!m_simple_scene_objects.empty())
        trace_simple_objects(ray, hit_data);

    if (!m_scene_meshes.empty())
        trace_meshes(ray, hit_data);

    return hit_data;
}

Vec3f Scene::cast_ray(const Rayf& ray)
{
    auto hit_data = trace(ray);

    if (hit_data.has_been_hit())
    {
        
        auto normal = hit_data.m_closest_ptr->get_surface_properties( hit_data );

		auto L = -1*m_light.m_direction;
		//const auto ratio = std::max(0.0f, normal.dot(-1*ray.direction()));
    	//return { 255*ratio, 255*ratio, 255*ratio }; 
        
		const auto hit_color = hit_data.m_closest_ptr->m_albedo / M_PI * m_light.m_intensity * m_light.m_color * std::max(0.f, normal.dot(L));
    	return hit_color;
    }

    return {0, 0, 0};
}


void Scene::render(PixelBuffer& buffer)
{
    Matrix44f camera_to_world;/* { 1, 0, 0, 0.0f,
	                            0, 1,  0,  0.0f, 
	                            0, 0,   1,   0.0f, 
	                            0, 0,  -30000.0f,  1.0f };*/

    const float fov = 90;

    const auto scale = tan( deg_to_rad( fov * 0.5 ) );

    const auto image_aspect_ratio = m_screen_width / static_cast<float>(m_screen_height); // without cast the decimals are discarded resulting in distortion

    const auto origin = camera_to_world.multiply_with_point( Vec3f(0) );

    int counter = 0;

    
	load_objects_from_file("teapot.obj");
    std::cout << "parsing done";
	
	Matrix44f objectToWorld = Matrix44f(1, 0, 0, 0,
										0, 1, 0.5, 0, 
										0, 0, 1, 0, 
										0, 0, -30, 1); 

	m_scene_meshes[0]->transform_object_to_world(objectToWorld);  

    //m_simple_scene_objects.push_back(std::make_shared<Sphere>(Vec3f(0,0,0.3f), 0.1f, 0.18f));
    //m_simple_scene_objects.push_back(std::make_shared<Plane>(Vec3f(0, 0, 1), Vec3f(0, 0, 0)));

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


			//figure out if this can be moved to a matrix partially
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