

#ifndef RAYTRACER_INTEGRATOR_H
#define RAYTRACER_INTEGRATOR_H


#include <random>
#include "Scene.h"
#include "Imaging.h"
#include "Shape.h"
#include "Intersection.h"
#include "Material.h"

class RenderMode
{
public:
    RenderMode() = default;
    virtual ~RenderMode() = default;

    virtual void Render(Scene& scene, Camera& camera, ImageBuffer& buffer) = 0;
};

class WhittedRayTracer : RenderMode
{
public:
    explicit WhittedRayTracer(int recursionDepth) : m_recursionDepth(recursionDepth) { }

    void Render(Scene& scene, Camera& camera, ImageBuffer& buffer) override;

private:

    Color3f TraceRay(const Rayf& ray, Scene& scene, int depth);

    int m_recursionDepth;
    float m_shadow_bias = 1e-4f;
};

class StochasticRayTracer : public RenderMode
{
public:
    explicit StochasticRayTracer(int depth = 5, size_t raysPerPixel = 5) 
        : m_depth(depth)
        , m_raysPerPixel(raysPerPixel)
        , m_gen(std::random_device()())
        , m_dist(0, 1) 
    { }
    
    void Render(Scene& scene, Camera& camera, ImageBuffer& buffer) override;
    Color3f TraceRay(const Rayf& ray, Scene& scene, int depth);

    // to generate random numbers [0,1]
    //std::random_device m_seed;
    std::mt19937 m_gen;
    std::uniform_real_distribution<> m_dist;

    int m_depth;
    size_t m_raysPerPixel;
};


#endif //RAYTRACER_INTEGRATOR_H
