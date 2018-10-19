#include "Integrator.h"
#include "Utility.h"

Color3f WhittedRayTracer::TraceRay(const Rayf& ray, Scene& scene, int depth)
{

}

void WhittedRayTracer::Render(Scene& scene, Camera& camera, ImageBuffer& buffer)
{
    for (int j = (int)buffer.Height() - 1; j >= 0; j--) // size_t causes subscript out of range due to underflow
    {
        for (size_t i = 0; i < buffer.Width(); ++i)
        {

            Color3f color{ 0 };

            const auto ray = camera.GetRay(i, j);

            color = TraceRay(ray, scene, 0);

            buffer.AddPixelAt(color, i, j);
        }
    }
}
}


Color3f StochasticRayTracer::TraceRay(const Rayf& ray, Scene& scene, int depth)
{
    Intersection isect;
    if (scene.Intersects(ray, isect))
    {
        Rayf scattered;
        Color3f attenuation;
        if (depth < m_depth && isect.m_matPtr->Scatter(ray, isect, attenuation, scattered))
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
    }

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