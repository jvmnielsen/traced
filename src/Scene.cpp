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

inline Vecf reflect(const Vecf& v, const Vecf& n)
{
    return v - 2 * DotProduct(v, n) * n;
}

Vecf Scene::random_in_unit_sphere()
{
    Vecf p;
    do
    {
        p = 2.0 * Vecf(m_dist(m_gen), m_dist(m_gen), m_dist(m_gen)) - Vecf(1, 1, 1);
    } while (p.MagnitudeSquared() >= 1.0);
    return p;
} 

Vecf Scene::CastRay(const Rayf& ray, uint32_t depth)
{
    if (depth > m_max_depth)
        return m_background_color;

	Vecf hit_color = {};
    auto hit_data = trace(ray);

    if (hit_data.has_been_hit())
    {
        hit_data.set_normal(ray);
		

        switch (hit_data.ptr_to_rndrble()->m_material)
        {
            
            case Diffuse:
            {
                
                for (const auto& light : m_scene_lights)
                {
                    LightingInfo light_info;
                    light->illuminate(hit_data.m_point, light_info);

                    auto shadow_intersect = trace(Rayf(hit_data.m_point + hit_data.m_normal * m_shadow_bias, -1 * light_info.direction));
                    hit_color += !shadow_intersect.has_been_hit()
                        * hit_data.ptr_to_rndrble()->m_albedo
                        * light_info.intensity
                        * std::max(0.f, hit_data.m_normal.DotProduct(-1 * light_info.direction));
                } 
                //hit_color = {100, 100, 100};
                break;
            }
            case Reflective:
            {
                const auto reflected = reflect(ray.direction(), hit_data.point());
                
                hit_color += 0.8 * CastRay(Rayf(hit_data.point() + hit_data.normal() * m_shadow_bias, reflected), depth + 1); //  + 0.4 * random_in_unit_sphere()
                break;
            }
            case ReflectAndRefract:
            {
                break;
            }
            default:
            {
                break;
            }
        }
		
    	
    }
	else
	{
		hit_color = m_background_color;
	}

	return hit_color;
}

void Scene::render(ImageBuffer& buffer)
{

	m_scene_lights.emplace_back(std::make_unique<PointLight>(Vecf(0.5f, 0.6f, 0.8f), 200.0f, Vecf(-1.4, 2, -0.3)));
	//m_scene_lights.emplace_back(std::make_unique<PointLight>(Vecf(255, 69, 0), 250.0f, Vecf(2, 2, -1)));

    Camera camera = {Vecf(0, 2, 1), Vecf(0,0,-1), Vecf(0,1,0), 90, float(m_screen_width)/float(m_screen_height)};

	m_background_color = { 0.01f, 0.1f, 0.15f };

	
	load_objects_from_file("plane.obj");
    std::cout << "parsing done";

    
	Matrix44f objectToWorld = Matrix44f(2, 0, 0, 0,
										0, 2, 0, 0, 
										0, 0, 2, 0, 
										0, 0, -2.5, 1);  

	m_scene_meshes[0]->transform_object_to_world(objectToWorld);  

    m_simple_scene_objects.push_back(std::make_shared<Sphere>(Vecf(0.1f, 0.50f,-1.0f), 0.5f, Vecf(0.18f), Diffuse));
	//m_simple_scene_objects.push_back(std::make_shared<Sphere>(Vecf(-0.5f, 0.5f, -2.0f), 0.5f, Vecf(0.18f), Diffuse));
    //m_simple_scene_objects.push_back(std::make_shared<Sphere>(Vecf(-0.5f, -5.0f, -3.0f), 5.0f, Vecf(0.18f), Diffuse));
	
    //m_simple_scene_objects.push_back(std::make_shared<Plane>(Vecf(1, 0, 0), Vecf(0, 0, 0), 0.18f));

    const int aa_factor = 5;

    for (int j = m_screen_height - 1; j >= 0; j--) // size_t causes subscript out of range due to underflow
    {
        
        for (size_t i = 0; i < m_screen_width; ++i)
        {
            Vecf color(0, 0, 0);

            for (int s = 0; s < aa_factor; s++)  // AA loop
            {
                const auto u = float(i + m_dist(m_gen)) / float(m_screen_width); // maybe precompute
                const auto v = float(j + m_dist(m_gen)) / float(m_screen_height);

                const auto ray = camera.GetRay(u, v);

                color += CastRay(ray, 0);
            }
            
            color /= float(aa_factor);
		
            buffer.AddPixelAt(color, i, j);
        }
    }
    std::cout << "is done";

    
}