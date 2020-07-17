#include "renderer.hpp"
#include "../utility/utility.hpp"
#include <thread>
#include <future>
#include <numeric>

using namespace tr;
using namespace gm;

Renderer::Renderer(
    std::unique_ptr<Camera> camera,
    std::unique_ptr<Scene> scene,
    std::shared_ptr<ImageBuffer> buffer)
        : m_camera(std::move(camera))
        , m_scene(std::move(scene))
        , m_buffer(std::move(buffer))
        , m_running(true)
{
}


/*
auto
Renderer::RenderProgressive() -> void {
    
    int samplesPerPixel = 1;

    for (;;) {
        Timer timer{std::string{"Main render loop: "}};

        //std::vector<std::future<std::vector<Color3f>>> futures;
        //std::size_t numCores = std::thread::hardware_concurrency();
        std::vector<ScreenSegment> segments;

        constexpr int numSegments = 3;
        constexpr int totalSegments = numSegments * numSegments;

        segments.reserve(totalSegments);

        const auto widthInterval = m_buffer->GetWidth() / numSegments;
        const auto heightInterval = m_buffer->GetHeight() / numSegments;

        for (int i = 0; i < numSegments; ++i) {
            for (int j = 0; j < numSegments; ++j) {
                segments.emplace_back(
                    ScreenSegment(Point2i(widthInterval * j, m_buffer->GetHeight() - heightInterval * (i + 1)),
                    Point2i(widthInterval * (j + 1), m_buffer->GetHeight() - heightInterval * i), 2)
                    //ScreenSegment(Point2i(widthInterval * j, heightInterval * i), Point2i(widthInterval * (j + 1), heightInterval * (i+1)))
                );
            }
        }

        std::vector<std::future<std::vector<bool>>> futures;
        std::vector<std::vector<Color3f>> renderResult;
        //renderResult.reserve(totalSegments);

        for (auto& segment : segments) {
            futures.emplace_back(std::async([this, &segment, samplesPerPixel] { return RenderScreenSegment(segment, samplesPerPixel); })); // parallelize
        }

        for (auto& future : futures) {
            renderResult.emplace_back(future.get());
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

        samplesPerPixel *= 2;

    }

}*/


auto
Renderer::render(int samples_per_pixel) -> void {

    Timer timer{std::string{"Main render loop: "}};

    std::vector<ScreenSegment> segments;

    // careful about odd / even numbers depending on screen size -- find fix
    const auto num_segments = 4;// std::thread::hardware_concurrency() / 2; // experiment further to determine proper values
    const auto total_segments = num_segments * num_segments;

    std::cout << "Rendering " << total_segments << " total segments" << std::endl;

    segments.reserve(total_segments);

    const auto width_interval = m_buffer->get_width() / num_segments;
    const auto height_interval = m_buffer->get_height() / num_segments;

    for (int i = 0; i < num_segments; ++i) {
        for (int j = 0; j < num_segments; ++j) {
            segments.emplace_back(
                ScreenSegment(gm::Point2i(width_interval * j, m_buffer->get_height() - height_interval * (i + 1)),
                gm::Point2i(width_interval * (j + 1), m_buffer->get_height() - height_interval * i), num_segments * i + j)
                //ScreenSegment(Point2i(widthInterval * j, heightInterval * i), Point2i(widthInterval * (j + 1), heightInterval * (i+1)))
            );
        }
    }

    //std::vector<std::future<bool>> futures;
    std::vector<std::future<std::vector<gm::Color3f>>> render_result;
    //renderResult.reserve(totalSegments);
    std::vector<std::thread> render_segments;

    for (auto& segment : segments) {
        //render_result.emplace_back(std::async(&Renderer::RenderScreenSegment, this, segment, samples_per_pixel));
        render_segments.emplace_back(&Renderer::render_screen_segment, this, segment, samples_per_pixel);
    }

    for (auto& thread : render_segments)
        thread.join();
   
}

auto
Renderer::render_screen_segment(const ScreenSegment& segment, int samples_per_pixel) -> void {
 
    Timer timer{std::string{"Segment " + std::to_string(segment.index) + " took: "}};

    Sampler sampler;
    const int num_pixels = (segment.upperBound.y - segment.lowerBound.y) * (segment.upperBound.x - segment.lowerBound.x);

    std::vector<gm::Color3f> result;
    result.reserve(num_pixels);

    // size_t causes subscript out of range due to underflow
    for (int j = segment.upperBound.y - 1; j >= segment.lowerBound.y; j--) { // start in the top left
        for (int i = segment.lowerBound.x; i < segment.upperBound.x; ++i) {

            auto color = Color3f(0);
            for (size_t s = 0; s < samples_per_pixel; ++s) {
                const auto u = (i + sampler.get_random_real()) / static_cast<float>(m_buffer->get_width());
                const auto v = (j + sampler.get_random_real()) / static_cast<float>(m_buffer->get_height());

                auto ray = m_camera->get_ray(u, v, sampler);

                color += outgoing_light(ray, sampler);

                if (!m_running)
                    return;
            }

            color /= float(samples_per_pixel);
            m_buffer->add_pixel_at(color, i, j);
            //result.push_back(color);
        }
    }
}

auto
Renderer::outgoing_light(Rayf& ray, Sampler& sampler) -> gm::Color3f {

    bool lastBounceSpecular = false;
    auto throughput = gm::Color3f(1.0f);

    auto color = gm::Color3f::black();

    for (int bounces = 0; bounces < 2; ++bounces) { 

        auto isect = m_scene->intersects(ray);

		if (!isect.has_value()) {
            break;
        }

        auto wo = -ray.direction().normalise();

        if (bounces == 0 || lastBounceSpecular) {
            color += throughput * isect->emitted(static_cast<Vec3f>(wo));
        }

        color += throughput * m_scene->sample_one_light(*isect, static_cast<Vec3f>(wo), sampler);


        auto [wi, pdf, f] = isect->sample_material(static_cast<Vec3f>(wo), sampler);

        if (f.is_black() || pdf == 0.0f) break;

        throughput = throughput * std::abs(dot(isect->shading_normal(), wi)) * f / pdf; // TODO: overload *=


        ray = Rayf{ isect->point(), wi };
        
        if (bounces > 3) {
            float q = std::max(color.r, std::max(color.g, color.b));
            if (sampler.get_random_real() > q) // generate number [0.0, 1.0)
                break;
            throughput /= q;
        } 
    }

    return color;
}

auto 
Renderer::render_normals() -> void
{
    for (int j = (int)m_buffer->get_height() - 1; j >= 0; j--) {
        for (size_t i = 0; i < m_buffer->get_width(); ++i) {

            Sampler sampler;
            const auto ray = m_camera->get_ray(i, j, sampler);
            auto isect = m_scene->intersects(ray);

            auto color = Color3f::black();

            Color3f{std::abs(isect->shading_normal().x()), std::abs(isect->shading_normal().y()), std::abs(isect->shading_normal().z()) };

            if (!isect.has_value()) {
                color = Color3f::black();
            }

            m_buffer->add_pixel_at(color, i, j);
        }
    }
}

auto
Renderer::shutdown() -> void
{
    m_running = false;
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