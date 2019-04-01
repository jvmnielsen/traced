#pragma once
#include "scene.hpp"
#include "../imaging/camera.hpp"
#include "../imaging/image_buffer.hpp"

class Renderer
{
public:
    Renderer(
        std::unique_ptr<Camera> camera,
        std::unique_ptr<Scene> scene,
        std::shared_ptr<ImageBuffer> buffer);

    auto render(int samples_per_pixel) -> void;

    auto render_normals() -> void;
 
    auto shutdown() -> void;

private:

    bool m_running;

    auto render_screen_segment(const ScreenSegment& segment, int samples_per_pixel) -> void;

    std::unique_ptr<Camera>      m_camera;
    std::unique_ptr<Scene>       m_scene;
    std::shared_ptr<ImageBuffer> m_buffer;

    auto outgoing_light(Rayf& ray, Sampler& sampler) -> Color3f;

    static constexpr int m_maxBounces = 5;
};

/*
class WhittedRayTracer : Renderer
{
public:
    explicit WhittedRayTracer(int recursionDepth) : m_recursionDepth(recursionDepth) { }

    void Render(Scene& scene, Camera& camera, ImageBuffer& buffer) override;

private:

    Color3f TraceRay(const Rayf& ray, Scene& scene, int depth);

    int m_recursionDepth;
    float m_shadow_bias = 1e-4f;
};

class StochasticRayTracer : public Renderer
{
public:
    
}; */
