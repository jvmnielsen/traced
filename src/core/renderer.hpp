#pragma once
#include "scene.hpp"
#include "../imaging/camera.hpp"
#include "../imaging/image_buffer.hpp"

#include <graphics-math.hpp>

namespace tr {

    class Renderer {
    public:
        Renderer(
            std::unique_ptr<Camera> camera,
            std::unique_ptr<Scene> scene,
            std::shared_ptr<ImageBuffer> buffer);

        auto render(int samples_per_pixel) -> void;
        auto shutdown() -> void;
    private:
        bool m_running;
        auto render_screen_segment(ScreenSegment const& segment, int samples_per_pixel) -> void;

        std::unique_ptr<Camera>      m_camera;
        std::unique_ptr<Scene>       m_scene;
        std::shared_ptr<ImageBuffer> m_buffer;

        auto outgoing_light(Rayf& ray, Sampler& sampler) -> gm::Color3f;
    };

}
