#include "Integrator.h"
#include "Utility.h"

Color3f WhittedRayTracer::TraceRay(const Rayf& ray, Scene& scene, int depth)
{
    Intersection isect;
    Color3f hitColor;

    if (scene.Intersects(ray, isect))
    //if (scene.m_boundingVolumes[0]->m_shape->Intersects(ray, isect) || scene.m_boundingVolumes[1]->m_shape->Intersects(ray, isect))
    {
        hitColor = isect.m_matPtr->CalculateSurfaceColor(ray, isect, scene, 0);
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
    for (int j = (int)buffer.Height() - 1; j >= 0; j--) // size_t causes subscript out of range due to underflow
    {
        for (size_t i = 0; i < buffer.Width(); ++i)
        {
            Color3f color{ 0 };

            const auto ray = camera.GetRay(static_cast<float>(i)/buffer.Width(), static_cast<float>(j)/buffer.Height());

            color = TraceRay(ray, scene, 0);

            buffer.AddPixelAt(color, i, j);
        }
    }
}

Color3f StochasticRayTracer::TraceRay(const Rayf& ray, Scene& scene, int depth)
{
    /*
    Intersection isect;
    if (scene.Intersects(ray, isect))
    {
        Rayf scattered;
        Color3f attenuation;
        if (depth < m_depth && isect.m_matPtr->CalculateSurfaceColor(ray, isect, attenuation, scattered))
        {
            return attenuation * TraceRay(scattered, scene, depth + 1);
        }
        else
        {
            return Color3f{ 0, 0, 0 };
        }
    }
    else
    {
        return scene.BackgroundColor();
    } */
    return {0,0,0};
}

void StochasticRayTracer::Render(Scene& scene, Camera& camera, ImageBuffer& buffer)
{
    Timer timer = {"Rendering took: "};

    const int aa_factor = 5;

    for (int j = (int)buffer.Height() - 1; j >= 0; j--) // size_t causes subscript out of range due to underflow
    {
        for (size_t i = 0; i < buffer.Width(); ++i)
        {

            Color3f color{ 0 };

            for (int s = 0; s < aa_factor; s++)  // AA loop
            {
                const auto u = float(i + m_dist(m_gen)) / float(buffer.Width()); // maybe precompute
                const auto v = float(j + m_dist(m_gen)) / float(buffer.Height());

                const auto ray = camera.GetRay(u, v);

                color += TraceRay(ray, scene, 0);
            }

            color /= float(aa_factor);
            buffer.AddPixelAt(color, i, j);
        }
    }
}