#include "Scene.h"
#include "Window.h"
#include "Sphere.h"
#include "Material.h"
#include "Triangle.h"
#include "Parser.h"
#include <memory>
#include "Utility.h"


void Scene::AddRenderable(std::unique_ptr<Renderable> renderPtr)
{
    m_simple_scene_objects.push_back(std::move(renderPtr));
}

auto Scene::AddMesh(std::unique_ptr<Mesh> meshPtr) -> void
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

inline Vec3f Reflect(const Vec3f &v, const Vec3f &n)
{
    return v - n * 2 * v.DotProduct(n);
}

Vec3f Scene::random_in_unit_sphere()
{
    Vec3f p;
    do
    {
        p = Vec3f(m_dist(m_gen), m_dist(m_gen), m_dist(m_gen)) * 2.0f - Vec3f(1, 1, 1);
    } while (p.LengthSquared() >= 1.0);
    return p;
}

Vec3f Refracted(const Vec3f &I, const Vec3f &N, const float ior)
{
    float cosi = Math::Clamp(-1.0f, 1.0f, I.DotProduct(N));
    float etai = 1, etat = ior;
    Vec3f n = N;
    if (cosi < 0) { cosi = -cosi; } else { std::swap(etai, etat); n= -N; }
    float eta = etai / etat;
    float k = 1 - eta * eta * (1 - cosi * cosi);
    return k < 0 ? Vec3f(0) : I * eta + n * (eta * cosi - sqrtf(k));
}

void Fresnel(const Vec3f &I, const Vec3f &N, const float &ior, float &kr)
{
    float cosi = Math::Clamp(-1.0f, 1.0f, I.DotProduct(N));
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

Color3f Scene::CastRay(const Rayf& ray, uint32_t depth)
{
    if (depth > m_max_depth)
        return m_backgroundColor;


	Color3f hit_color{};
    auto hit_data = Trace(ray);

    if (hit_data.HasBeenHit())
    {
        hit_data.CalculateNormal(ray);
		

        switch (hit_data.RenderablePtr()->Material())
        {
            
            case MaterialType::Diffuse:
            {
                
                for (const auto& light : m_scene_lights)
                {
                    LightingInfo light_info;
                    light->illuminate(hit_data.Point(), light_info);

                    auto shadow_intersect = Trace(
                            Rayf(hit_data.Point() + hit_data.Normal() * m_shadow_bias, -light_info.direction,
                                 RayType::ShadowRay));
                    hit_color +=
                         hit_data.RenderablePtr()->Albedo()
                        * light_info.intensity
                        * std::max(0.f, hit_data.Normal().DotProduct(-light_info.direction))
                        * static_cast<int>(!shadow_intersect.HasBeenHit());
                } 
                //hit_color = {100, 100, 100};
                break;
            }
            case MaterialType::Reflective:
            {
                const auto reflected = Reflect(ray.Direction(), hit_data.Normal());
                
                hit_color += 0.8f * CastRay(Rayf(hit_data.Point() + hit_data.Normal() * m_shadow_bias, reflected, RayType::PrimaryRay), depth + 1); // fuzzines: + 0.05 * random_in_unit_sphere()
                break;
            }
            case MaterialType::ReflectAndRefract:
            {
                Color3f reflectColor = {};
                Color3f refractColor = {};
                float kr;
                Fresnel(ray.Direction(), hit_data.Normal(), 0.9, kr);
                bool outside = ray.Direction().DotProduct(hit_data.Normal()) < 0;
                Vec3f bias = m_shadow_bias * hit_data.Normal();

                if (kr < 1)
                {
                    Vec3f refractionDirection = Refracted(ray.Direction(), hit_data.Normal(), 0.9).Normalize();
                    Point3f refractionRayOrig = outside ? hit_data.Point() - bias : hit_data.Point() + bias;
                    refractColor = CastRay(Rayf(refractionRayOrig, refractionDirection, RayType::PrimaryRay), depth + 1);
                }

                Vec3f reflectionDirection = Reflect(ray.Direction(), hit_data.Normal()).Normalize();
                Point3f reflectionRayOrig = outside ? hit_data.Point() + bias : hit_data.Point() - bias;
                reflectColor = CastRay(Rayf(reflectionRayOrig, reflectionDirection, RayType::PrimaryRay), depth + 1);

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
            Color3f color{ 0 };

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