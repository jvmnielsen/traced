#include "renderer.hpp"
#include "../utility/utility.hpp"
#include <thread>
#include <future>

Renderer::Renderer(
    std::unique_ptr<Camera> camera,
    std::unique_ptr<Scene> scene,
    std::shared_ptr<ImageBuffer> buffer)
        : m_camera(std::move(camera))
        , m_scene(std::move(scene))
        , m_buffer(std::move(buffer))
        , m_gen(std::random_device()())
        , m_dist(0.0f, 1.0f) {
}



auto
Renderer::Render() -> void {
    
    //std::vector<std::future<std::vector<Color3f>>> futures;
    std::size_t numCores = std::thread::hardware_concurrency();
    std::vector<ScreenSegment> segments;

    constexpr int numSegments = 3;
    constexpr int totalSegments = numSegments * numSegments;

    segments.reserve(totalSegments);

    const auto widthInterval = m_buffer->GetWidth() / numSegments;
    const auto heightInterval = m_buffer->GetHeight() / numSegments;

    for (int i = 0; i < numSegments; ++i) {
        for (int j = 0; j < numSegments; ++j) {
            segments.emplace_back(
                ScreenSegment(Point2i(widthInterval * j, m_buffer->GetHeight() - heightInterval * (i + 1)), Point2i(widthInterval * (j + 1), m_buffer->GetHeight() - heightInterval * i))
                //ScreenSegment(Point2i(widthInterval * j, heightInterval * i), Point2i(widthInterval * (j + 1), heightInterval * (i+1)))
            );
        }
    }

    const int samplesPerPixel = 1;

    std::vector<std::vector<Color3f>> renderResult;

    renderResult.reserve(totalSegments);

    for (const auto& segment : segments) {
        renderResult.emplace_back(RenderScreenSegment(segment, samplesPerPixel)); // parallelize
    }

    std::vector<Color3f> flattened;
    for (int i = 0; i < m_buffer->GetWidth() * m_buffer->GetHeight(); ++i) {
        flattened.emplace_back(Color3f{0});
    }

    for (int v = 0; v < segments.size(); ++v) {
        for (int i = 0; i < heightInterval; ++i) {
            for (int j = 0; j < widthInterval; ++j) {
                const int yComponent = (int)m_buffer->GetWidth() * std::abs(segments.at(v).upperBound.y - i - (int)m_buffer->GetHeight());
                const int xComponent = j + segments.at(v).lowerBound.x;
                flattened[yComponent + xComponent] = renderResult.at(v).at(i * widthInterval + j);
            }
        }
    }

    m_buffer->ConvertToPixelBuffer(flattened);

}

auto
Renderer::RenderScreenSegment(const ScreenSegment& segment, int samplesPerPixel) -> std::vector<Color3f> {
 
    Sampler sampler;
    int numPixels = (segment.upperBound.y - segment.lowerBound.y) * (segment.upperBound.x - segment.lowerBound.x);

    std::vector<Color3f> result;
    result.reserve(numPixels);

    // size_t causes subscript out of range due to underflow
    for (int j = segment.upperBound.y - 1; j >= segment.lowerBound.y; j--) { // start in the top left
        for (int i = segment.lowerBound.x; i < segment.upperBound.x; ++i) {

            Color3f color{0};
            for (size_t s = 0; s < samplesPerPixel; ++s) {
                const auto u = (i + sampler.GetRandomReal()) / static_cast<float>(m_buffer->GetWidth());
                const auto v = (j + sampler.GetRandomReal()) / static_cast<float>(m_buffer->GetHeight());

                auto ray = m_camera->GetRay(u, v);

                color += TracePath(ray, sampler);
            }

            color /= float(samplesPerPixel);
            result.push_back(color);
        }
    }
    return result;
}

/*
void Renderer::Render(int samplesPerPixel) {

    Timer timer { std::string("Rendering took: ") };

    const int height = m_buffer->GetHeight();
    const int width = m_buffer->GetWidth();

    Sampler sampler;

    
    std::cout << "Entering main render-loop\n";
    // size_t causes subscript out of range due to underflow
    for (int j = height - 1; j >= 0; j--) { // start in the top left
        for (size_t i = 0; i < width; ++i) {

            Color3f color{0};
            for (size_t s = 0; s < samplesPerPixel; s++) {
                const auto u = (i + sampler.GetRandomReal()) / static_cast<float>(width);
                const auto v = (j + sampler.GetRandomReal()) / static_cast<float>(height); 

                auto ray = m_camera->GetRay(u, v);

                color += TracePath(ray, sampler);
            }

            color /= float(samplesPerPixel);
            m_buffer->AddPixelAt(color, i, j);
        }
    }
}
*/

auto
Renderer::TracePath(Rayf& ray, Sampler& sampler) -> Color3f {

    bool lastBounceSpecular = false;
    Color3f throughput{ 1.0f };
    Color3f color{ 0.0f };

    for (int bounces = 0; bounces < m_maxBounces; ++bounces) { 

        auto isect = m_scene->Intersects(ray);

		if (!isect.has_value()) {
            color += throughput * m_scene->BackgroundColor();
            break;
        }

        auto wo = -ray.GetDirection();

        float distanceSquared = (isect->GetPoint() - ray.GetOrigin()).LengthSquared();

        if (bounces == 0 || lastBounceSpecular) {
            color += throughput * isect->m_material->Emitted(isect->GetGeometricNormal(), wo, distanceSquared);
        }

        auto directLight = m_scene->SampleOneLight(*isect, wo, sampler);

        color += throughput * directLight;


        Vec3f wi;
        float pdf;
        Color3f f = isect->m_material->Sample(wo, wi, pdf, sampler);

        if (f.IsBlack() || pdf == 0.0f) break;
        throughput = throughput * std::abs(Dot(wi, isect->GetShadingNormal())) * f / pdf; // TODO: overload *=

        ray = Rayf{ isect->GetPoint(), wi };

        //<<Possibly terminate the path with Russian roulette>>=
        if (bounces > 3) {
            float q = std::max(.05f, 1.0f - throughput.g);
            if (sampler.GetRandomReal() < q) // generate number [0.0, 1.0)
                break;
            throughput /= 1 - q;
        }
    }
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