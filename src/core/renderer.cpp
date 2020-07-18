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
        , m_running(true) {
}

auto Renderer::render(int samples_per_pixel) -> void {

    Timer timer{std::string{"Main render loop: "}};
    std::vector<ScreenSegment> segments;
    // careful about odd / even numbers depending on screen size -- find fix
    const auto num_segments = 2;
    const auto total_segments = num_segments * num_segments;

    std::cout << "Rendering " << total_segments << " total segments" << std::endl;
    segments.reserve(total_segments);
    const auto width_interval = m_buffer->width() / num_segments;
    const auto height_interval = m_buffer->height() / num_segments;

    for (int i = 0; i < num_segments; ++i) {
        for (int j = 0; j < num_segments; ++j) {
            segments.emplace_back(
                ScreenSegment(gm::Point2i(width_interval * j, m_buffer->height() - height_interval * (i + 1)),
                              gm::Point2i(width_interval * (j + 1), m_buffer->height() - height_interval * i), num_segments * i + j)
            );
        }
    }
    std::vector<std::future<std::vector<gm::Color3f>>> render_result;
    std::vector<std::thread> render_segments;

    for (auto& segment : segments) {
        render_segments.emplace_back(&Renderer::render_screen_segment, this, segment, samples_per_pixel);
    }

    for (auto& thread : render_segments)
        thread.join();
}

auto Renderer::render_screen_segment(const ScreenSegment& segment, int samples_per_pixel) -> void {
 
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
                const auto u = (i + sampler.get_random_real()) / static_cast<float>(m_buffer->width());
                const auto v = (j + sampler.get_random_real()) / static_cast<float>(m_buffer->height());

                auto ray = m_camera->get_ray(u, v, sampler);

                color += outgoing_light(ray, sampler);

                if (!m_running)
                    return;
            }

            color /= float(samples_per_pixel);
            m_buffer->add_pixel_at(color, i, j);
        }
    }
}

auto Renderer::outgoing_light(Rayf& ray, Sampler& sampler) -> gm::Color3f {

    auto last_bounce_specular = false;
    auto throughput = gm::Color3f(1.0f);
    auto color = gm::Color3f::black();

    for (int bounces = 0; bounces < 2; ++bounces) {
        auto isect = m_scene->intersects(ray);
		if (!isect.has_value()) {
            break;
        }
        auto wo = -ray.direction().normalise();
        if (bounces == 0 || last_bounce_specular) {
            color += throughput * isect->emitted(static_cast<Vec3f>(wo));
        }

        color += throughput * m_scene->sample_one_light(*isect, static_cast<Vec3f>(wo), sampler);
        auto [wi, pdf, f] = isect->sample_material(static_cast<Vec3f>(wo), sampler);

        if (f.is_black() || pdf == 0.0f) break;

        throughput = throughput * std::abs(dot(isect->shading_normal(), wi)) * f / pdf;
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

auto Renderer::shutdown() -> void {
    m_running = false;
}