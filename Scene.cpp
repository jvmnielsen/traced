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
    return deg * M_PI / 180;
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
            hit_data.update_closest(renderable.get(), ray);
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
	Vec3f hit_color;
    auto hit_data = trace(ray);

    if (hit_data.has_been_hit())
    {
        hit_data.set_normal(ray);

		for (const auto& light : m_scene_lights)
		{
			LightingInfo light_info;
			light->illuminate(hit_data.m_point, light_info);

			auto shadow_intersect = trace(Rayf(hit_data.m_point + hit_data.m_normal * m_shadow_bias, -1*light_info.direction));
			hit_color += !shadow_intersect.has_been_hit() 
							* hit_data.ptr_to_rndrble()->m_albedo
							* light_info.intensity
							* std::max(0.f, hit_data.m_normal.dot(-1*light_info.direction));
		}
    	
    }
	else
	{
		hit_color = m_background_color;
	}


	return hit_color;
}

void Scene::render(PixelBuffer& buffer)
{

	m_scene_lights.emplace_back(std::make_unique<PointLight>(Vec3f(102, 204, 255), 200.0f, Vec3f(-2, 2, -1)));
	m_scene_lights.emplace_back(std::make_unique<PointLight>(Vec3f(200, 50, 100), 250.0f, Vec3f(2, 2, -1)));

    Camera camera = {Vec3f(0, 3, 1), Vec3f(0,0,-1), Vec3f(0,1,0), 90, float(m_screen_width)/float(m_screen_height)};

	m_background_color = { 30, 30, 30 };

	
	load_objects_from_file("plane.obj");
    std::cout << "parsing done";

    
	Matrix44f objectToWorld = Matrix44f(2, 0, 0, 0,
										0, 2, 0, 0, 
										0, 0, 2, 0, 
										0, 0, -2.5, 1);  

	m_scene_meshes[0]->transform_object_to_world(objectToWorld);  

    m_simple_scene_objects.push_back(std::make_shared<Sphere>(Vec3f(-0.6, 0.50f,-2.0f), 0.5f, Vec3f(0.18f)));
	m_simple_scene_objects.push_back(std::make_shared<Sphere>(Vec3f(0.6f, 0.5f, -2.0f), 0.5f, Vec3f(0.18f)));
	//m_simple_scene_objects.push_back(std::make_shared<Sphere>(Vec3f(1, 7, -30), 3.0f, 0.5f));
    //m_simple_scene_objects.push_back(std::make_shared<Plane>(Vec3f(1, 0, 0), Vec3f(0, 0, 0), 0.18f));

    int aa_factor = 10;

    for (int j = m_screen_height - 1; j >= 0; j--) // size_t causes subscript out of range due to underflow
    {
        
        for (size_t i = 0; i < m_screen_width; ++i)
        {
            Vec3f color(0, 0, 0);

            for (int s = 0; s < aa_factor; s++)  // AA loop
            {
                const auto u = float(i + m_dist(m_gen)) / float(m_screen_width); // maybe precompute
                const auto v = float(j + m_dist(m_gen)) / float(m_screen_height);

                const auto ray = camera.get_ray(u, v);

                color += cast_ray(ray);
            }
            
            color /= float(aa_factor);
		
            buffer.add_pixel(color);
        }
    }
    std::cout << "is done";

    
}