#include "Scene.h"
#include "Window.h"
#include "Sphere.h"
#include "Material.h"
#include "Polygon.h"
#include "Parser.h"
#include <memory>
#include "Utility.h"


void Scene::AddRenderable(std::unique_ptr<Renderable> renderPtr)
{
    m_simple_scene_objects.push_back(std::move(renderPtr));
}

void Scene::AddMesh(std::unique_ptr<Mesh> meshPtr)
{
    m_scene_meshes.push_back(std::move(meshPtr));
}


void Scene::AddLight(std::unique_ptr<Light> lightPtr)
{
    m_scene_lights.push_back(std::move(lightPtr));
}

void Scene::SetCamera(std::unique_ptr<Camera> camera)
{
    m_camera = std::move(camera);
}

void Scene::TraceSimpleObjects(const Rayf &ray, Intersection &hit_data)
{
    for (auto& renderable : m_simple_scene_objects)
    {
        if (renderable->Intersects(ray, hit_data))
        {
            hit_data.UpdateClosest(renderable.get(), ray);
        }
    }
}

void Scene::TraceMeshes(const Rayf &ray, Intersection &hit_data)
{
    for (auto& renderable : m_scene_meshes)
    {
        renderable->Intersects(ray, hit_data); // meshes handle it themselves internally for now, find a better solution
    }
}

Intersection Scene::Trace(const Rayf &ray)
{
    Intersection hit_data;

    if (!m_simple_scene_objects.empty())
        TraceSimpleObjects(ray, hit_data);

    if (!m_scene_meshes.empty())
        TraceMeshes(ray, hit_data);

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
        return m_backgroundColor;

	Vecf hit_color = {};
    auto hit_data = Trace(ray);

    if (hit_data.HasBeenHit())
    {
        hit_data.CalculateNormal(ray);
		

        switch (hit_data.RenderablePtr()->Material())
        {
            
            case Diffuse:
            {
                
                for (const auto& light : m_scene_lights)
                {
                    LightingInfo light_info;
                    light->illuminate(hit_data.Point(), light_info);

                    auto shadow_intersect = Trace(
                            Rayf(hit_data.Point() + hit_data.Normal() * m_shadow_bias, -1 * light_info.direction,
                                 ShadowRay));
                    hit_color += !shadow_intersect.HasBeenHit()
                        * hit_data.RenderablePtr()->Albedo()
                        * light_info.intensity
                        * std::max(0.f, hit_data.Normal().DotProduct(-1 * light_info.direction));
                } 
                //hit_color = {100, 100, 100};
                break;
            }
            case Reflective:
            {
                const auto reflected = Reflect(ray.direction(), hit_data.Normal());
                
                hit_color += 0.8 * CastRay(Rayf(hit_data.Point() + hit_data.Normal() * m_shadow_bias, reflected, PrimaryRay), depth + 1); // fuzzines: + 0.05 * random_in_unit_sphere()
                break;
            }
            case ReflectAndRefract:
            {
                Vecf reflectColor = {};
                Vecf refractColor = {};
                float kr;
                Fresnel(ray.direction(), hit_data.Normal(), 0.9, kr);
                bool outside = ray.direction().DotProduct(hit_data.Normal()) < 0;
                Vecf bias = m_shadow_bias * hit_data.Normal();

                if (kr < 1)
                {
                    Vecf refractionDirection = Refracted(ray.direction(), hit_data.Normal(), 0.9).Normalize();
                    Vecf refractionRayOrig = outside ? hit_data.Point() - bias : hit_data.Point() + bias;
                    refractColor = CastRay(Rayf(refractionRayOrig, refractionDirection, PrimaryRay), depth + 1);
                }

                Vecf reflectionDirection = Reflect(ray.direction(), hit_data.Normal()).Normalize();
                Vecf reflectionRayOrig = outside ? hit_data.Point() + bias : hit_data.Point() - bias;
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
		hit_color = m_backgroundColor;
	}

	return hit_color;
}

void Scene::Render(ImageBuffer& buffer)
{
    const int aa_factor = 5; 

    Timer timer = {"Rendering took: "};

    for (int j = (int)buffer.Height() - 1; j >= 0; j--) // size_t causes subscript out of range due to underflow
    {
        for (size_t i = 0; i < buffer.Width(); ++i)
        {
            Vecf color(0, 0, 0);

            for (int s = 0; s < aa_factor; s++)  // AA loop
            {
                const auto u = float(i + m_dist(m_gen)) / float(buffer.Width()); // maybe precompute
                const auto v = float(j + m_dist(m_gen)) / float(buffer.Height());

                const auto ray = m_camera->GetRay(u, v);

                color += CastRay(ray, 0);
            }
            
            color /= float(aa_factor);
		
            buffer.AddPixelAt(color, i, j);
        }
    }
}