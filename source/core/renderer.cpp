#include "renderer.hpp"
#include "../utility/utility.hpp"

Renderer::Renderer(
    std::unique_ptr<Camera> camera,
    std::unique_ptr<Scene> scene,
    std::shared_ptr<ImageBuffer> buffer)
    : m_camera(std::move(camera))
    , m_scene(std::move(scene))
    , m_buffer(buffer)
    //, m_sampler(std::move(sampler))
    , m_gen(std::random_device()())
    , m_dist(0.0f, 1.0f)
{
}

void Renderer::Render(int samplesPerPixel)
{
    Timer<std::milli> timer = {"Rendering took: "};

    const int height = m_buffer->GetHeight();
    const int width = m_buffer->GetWidth();

    
    for (int j = height - 1; j >= 0; j--) // size_t causes subscript out of range due to underflow
    {
        for (size_t i = 0; i < width; ++i)
        {

            Color3f color{0};

            for (size_t s = 0; s < samplesPerPixel; s++)
            {
                const auto u = float(i ) / float(width); // + m_dist(m_gen)
                const auto v = float(j ) / float(height); // + m_dist(m_gen)

                const auto ray = m_camera->GetRay(u, v);

                color += TraceRay(ray, 0);
            }

            color /= float(samplesPerPixel);
            m_buffer->AddPixelAt(color, i, j);
        }
    }
}

Color3f Renderer::TraceRay(const Rayf& ray, int depth)
{
    //Intersection isect;
    Color3f color{0.3f};

    const auto isect = m_scene->Intersects(ray);

    if (!isect.has_value() || depth > m_maxBounces)
        return m_scene->BackgroundColor();
        
    // Contribution from object self-emitting
    if (ray.IsPrimaryRay() && m_emit)
        color += isect.value().Emitted();

    // Contribution from direct lighting
    if (m_direct) // !ray.IsPrimaryRay() ||
    {
        //color += estiamteDirectLightFromPointLights(surfel, ray);
        //color += m_scene->SampleAreaLights(isect, ray);
    }

    // Contribution from indirect lighting
    if (!ray.IsPrimaryRay() || m_indirect)
        ;
        //color += estimateIndirectLight(sufel, ray, isEyeRay);
        

    
    //color = m_scene->BackgroundColor();
    

    return color;
}



/*
Color3f WhittedRayTracer::TraceRay(const Rayf& ray, Scene& scene, int depth)
{
    Intersection isect;
    Color3f hitColor;

    if (scene.Intersects(ray, isect))
    //if (scene.m_boundingVolumes[0]->m_mesh->Intersects(ray, isect) || scene.m_boundingVolumes[1]->m_mesh->Intersects(ray, isect))
    {
        //hitColor = isect.m_material->CalculateSurfaceColor(ray, isect, scene, 0);
    }
    else
    {
        hitColor = scene.BackgroundColor();
    }

    return hitColor;

}

void WhittedRayTracer::Render(Scene& scene, Camera& camera, ImageBuffer& buffer)
{
    Timer timer { "Rendering took: " };
    for (int j = (int)buffer.GetHeight() - 1; j >= 0; j--) // size_t causes subscript out of range due to underflow
    {
        for (size_t i = 0; i < buffer.GetWidth(); ++i)
        {
            Color3f color{ 0 };

            const auto ray = camera.GetRay(static_cast<float>(i)/buffer.GetWidth(), static_cast<float>(j)/buffer.GetHeight());

            color = TraceRay(ray, scene, 0);

            buffer.AddPixelAt(color, i, j);
        }
    }
}

Color3f StochasticRayTracer::TraceRay(const Rayf& ray, Scene& scene, int depth)
{
    Intersection isect;
    Color3f hitColor{0};

    if (scene.Intersects(ray, isect))
    //if (scene.m_boundingVolumes[0]->m_mesh->Intersects(ray, isect) || scene.m_boundingVolumes[1]->m_mesh->Intersects(ray, isect) || scene.m_boundingVolumes[2]->m_mesh->Intersects(ray, isect))
    {
        Rayf scattered;
        Color3f attenuation;
        const auto emitted = isect.m_material->Emitted(isect.m_uv.x, isect.m_uv.y, isect.m_point);

        if (depth < m_depth && isect.m_material->Scatter(ray, isect, attenuation, scattered)) 
        {
             hitColor += emitted + attenuation * TraceRay(scattered, scene, depth + 1);
        }
        else
        {
            hitColor += emitted;
        }
    }
    else
    {
        hitColor = scene.BackgroundColor();
    }

    return hitColor;
}

void StochasticRayTracer::Render(Scene& scene, Camera& camera, ImageBuffer& buffer)
{
    Timer timer = {"Rendering took: "};


    for (int j = (int)buffer.GetHeight() - 1; j >= 0; j--) // size_t causes subscript out of range due to underflow
    {
        for (size_t i = 0; i < buffer.GetWidth(); ++i)
        {

            Color3f color{ 0 };

            for (size_t s = 0; s < m_raysPerPixel; s++) 
            {
                const auto u = float(i + m_dist(m_gen)) / float(buffer.GetWidth()); // maybe precompute
                const auto v = float(j + m_dist(m_gen)) / float(buffer.GetHeight());

                const auto ray = camera.GetRay(u, v);

                color += TraceRay(ray, scene, 0);
            }

            color /= float(m_raysPerPixel);
            buffer.AddPixelAt(color, i, j);
        }
    }
}

*/