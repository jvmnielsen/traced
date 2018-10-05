#include "Scene.h"
#include "Window.h"
#include "Sphere.h"
#include "Camera.h"
#include "Material.h"
#include "Matrix44.h"
#include "Polygon.h"
#include "Parser.h"
#include <memory>




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

inline Vecf Reflect(const Vecf &v, const Vecf &n)
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

Vecf Refracted(const Vecf &I, const Vecf &N, const float ior)
{
    float cosi = clamp(-1, 1, DotProduct(I, N));
    float etai = 1, etat = ior;
    Vecf n = N;
    if (cosi < 0) { cosi = -cosi; } else { std::swap(etai, etat); n= -1 * N; }
    float eta = etai / etat;
    float k = 1 - eta * eta * (1 - cosi * cosi);
    return k < 0 ? Vecf(0) : eta * I + (eta * cosi - sqrtf(k)) * n;
}

void Fresnel(const Vecf &I, const Vecf &N, const float &ior, float &kr)
{
    float cosi = clamp(-1, 1, I.DotProduct(N));
    float etai = 1, etat = ior;
    if (cosi > 0) { std::swap(etai, etat); }
    // Compute sini using Snell's law
    float sint = etai / etat * sqrtf(std::max(0.f, 1 - cosi * cosi));
    // Total internal reflection
    if (sint >= 1) {
        kr = 1;
    }
    else {
        float cost = sqrtf(std::max(0.f, 1 - sint * sint));
        cosi = fabsf(cosi);
        float Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
        float Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
        kr = (Rs * Rs + Rp * Rp) / 2;
    }
    // As a consequence of the conservation of energy, transmittance is given by:
    // kt = 1 - kr;
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

                    auto shadow_intersect = trace(Rayf(hit_data.m_point + hit_data.m_normal * m_shadow_bias, -1 * light_info.direction, ShadowRay));
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
                const auto reflected = Reflect(ray.direction(), hit_data.m_normal);
                
                hit_color += 0.8 * CastRay(Rayf(hit_data.point() + hit_data.normal() * m_shadow_bias, reflected, PrimaryRay), depth + 1); // fuzzines: + 0.05 * random_in_unit_sphere()
                break;
            }
            case ReflectAndRefract:
            {
                Vecf reflectColor = {};
                Vecf refractColor = {};
                float kr;
                Fresnel(ray.direction(), hit_data.m_normal, 0.9, kr);
                bool outside = ray.direction().DotProduct(hit_data.m_normal) < 0;
                Vecf bias = m_shadow_bias * hit_data.m_normal;

                if (kr < 1)
                {
                    Vecf refractionDirection = Refracted(ray.direction(), hit_data.m_normal, 0.9).Normalize();
                    Vecf refractionRayOrig = outside ? hit_data.point() - bias : hit_data.point() + bias;
                    refractColor = CastRay(Rayf(refractionRayOrig, refractionDirection, PrimaryRay), depth + 1);
                }

                Vecf reflectionDirection = Reflect(ray.direction(), hit_data.m_normal).Normalize();
                Vecf reflectionRayOrig = outside ? hit_data.point() + bias : hit_data.point() - bias;
                reflectColor = CastRay(Rayf(reflectionRayOrig, reflectionDirection, PrimaryRay), depth + 1);

                hit_color += reflectColor * kr + refractColor * (1 - kr);

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

void Scene::Render(ImageBuffer &buffer)
{
	m_scene_lights.emplace_back(std::make_unique<PointLight>(Vecf(0.2f, 0.9f, 0.5f), 300.0f, Vecf(-1.4, 2, -2.0)));
	m_scene_lights.emplace_back(std::make_unique<PointLight>(Vecf(0.9f, 0.5f, 0.1f), 400.0f, Vecf(2, 2, -1)));

    Camera camera = {Vecf(0, 2, 1), Vecf(0,0,-4), Vecf(0,1,0), 90, float(m_screen_width)/float(m_screen_height)};

    m_background_color = { 0.25f, 0.30f, 0.27f };

    /*
	load_objects_from_file("../assets/teapot.obj");
    
	Matrix44f objectToWorld = Matrix44f(0.15, 0, 0, 0,
										0, 0.15, 0, 0,
										0, 0, 0.15, 0,
										0, 0, -3.8f, 1);

	m_scene_meshes[0]->transform_object_to_world(objectToWorld); */

    load_objects_from_file("../assets/plane.obj");

    Matrix44f objectToWorld1 = Matrix44f(6, 0, 0, 0,
                                        0, 6, 0, 0,
                                        0, 0, 6, 0,
                                        0, 0.0f, -2.5f, 1);

    m_scene_meshes[0]->transform_object_to_world(objectToWorld1);
    //m_scene_meshes[0]->SetMaterialType(Reflective);

    const auto test = std::make_shared<Sphere>(Vecf(0.1f, 0.50f,-2.0f), 0.5f, Vecf(0.18f), ReflectAndRefract);
	const auto test2 = std::make_shared<Sphere>(Vecf(-0.5f, 0.5f, -3.0f), 0.5f, Vecf(0.18f), Diffuse);
    m_simple_scene_objects.emplace_back(test);
	m_simple_scene_objects.emplace_back(test2);
    m_simple_scene_objects.push_back(std::make_shared<Sphere>(Vecf(0.7f, 0.5f, -2.6f), 0.40f, Vecf(0.18f), Reflective));
	
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
}