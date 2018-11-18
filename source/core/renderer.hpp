#pragma once
#include <random>
#include "scene.hpp"
#include "../imaging/camera.hpp"
#include "../imaging/image_buffer.hpp"


//#include "Material.h"


class renderer
{
public:
    renderer(
        std::unique_ptr<camera> camera,
        std::unique_ptr<scene> scene,
        std::shared_ptr<image_buffer> buffer);

    void Render(int samplesPerPixel);

private:
    std::unique_ptr<camera>      m_camera;
    std::unique_ptr<scene>       m_scene;
    std::shared_ptr<image_buffer> m_buffer;
    //std::unique_ptr<Sampler>     m_sampler;

    // to generate random numbers [0,1]
    //std::random_device m_seed;
    std::mt19937 m_gen;
    std::uniform_real_distribution<float> m_dist;

    Color3f TraceRay(const Rayf& ray, int depth);

    //Color3f SamplePointLights(const Intersection& isect, const Rayf& ray) const;
    //Color3f SampleAreaLights(const Intersection& isect, const Rayf& ray) const;
    //Color3f SampleIndirectLighting(const Intersection& isect, const Rayf& ray) const;

    bool m_emit = true;
    bool m_direct = true;
    bool m_indirect = true;

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
