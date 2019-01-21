#pragma once
#include <random>
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

    auto Render(int samplesPerPixel) -> void;
    auto RenderProgressive() -> void;


 

private:

    
    struct ScreenSegment {
        ScreenSegment(Point2i lower, Point2i upper) : lowerBound(std::move(lower)), upperBound(std::move(upper)) {}
        Point2i lowerBound;
        Point2i upperBound;
    };

    auto RenderScreenSegment(const ScreenSegment& segment, int samplesPerPixel)->std::vector<Color3f>;
    

    std::unique_ptr<Camera>      m_camera;
    std::unique_ptr<Scene>       m_scene;
    std::shared_ptr<ImageBuffer> m_buffer;
    //std::unique_ptr<Sampler>     m_sampler;

    // to generate random numbers [0,1]
    //std::random_device m_seed;
    std::mt19937 m_gen;
    std::uniform_real_distribution<float> m_dist;

    auto TracePath(Rayf& ray, Sampler& sampler) -> Color3f;

    //Color3f SamplePointLights(const Intersection& isect, const Rayf& ray) const;
    //Color3f SampleAreaLights(const Intersection& isect, const Rayf& ray) const;
    //Color3f SampleIndirectLighting(const Intersection& isect, const Rayf& ray) const;

    bool m_emit = true;
    bool m_direct = true;
    bool m_indirect = true;
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
