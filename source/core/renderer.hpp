#pragma once
#include "scene.hpp"
#include "../imaging/camera.hpp"
#include "../imaging/image_buffer.hpp"


//#include "Material.h"


class Renderer
{
public:
    Renderer(
        std::unique_ptr<Camera> camera,
        std::unique_ptr<Scene> scene,
        std::shared_ptr<ImageBuffer> buffer);

    auto render(int samples_per_pixel) -> void;
    //auto RenderProgressive() -> void;


 

private:

    struct ScreenSegment {
        ScreenSegment(Point2i lower, Point2i upper, size_t _index) : lowerBound(std::move(lower)), upperBound(std::move(upper)), index(_index) {}
        Point2i lowerBound;
        Point2i upperBound;
        size_t index;
    };

    auto RenderScreenSegment(const ScreenSegment& segment, int samplesPerPixel) -> bool;
    

    std::unique_ptr<Camera>      m_camera;
    std::unique_ptr<Scene>       m_scene;
    std::shared_ptr<ImageBuffer> m_buffer;
    //std::unique_ptr<Sampler>     m_sampler;

    auto OutgoingLight(Rayf& ray, Sampler& sampler) -> Color3f;

    //Color3f SamplePointLights(const Intersection& isect, const Rayf& ray) const;
    //Color3f SampleAreaLights(const Intersection& isect, const Rayf& ray) const;
    //Color3f SampleIndirectLighting(const Intersection& isect, const Rayf& ray) const;
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
