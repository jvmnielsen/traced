//
// Created by Jacob Vesti Moeslund Nielsen on 18/10/2018.
//

#ifndef RAYTRACER_INTEGRATOR_H
#define RAYTRACER_INTEGRATOR_H


#include <random>
#include "Scene.h"
#include "Imaging.h"
#include "Shape.h"
#include "Intersection.h"
#include "Material.h"

class Integrator
{
public:
    Integrator() = default;
    virtual ~Integrator() = default;

    virtual void Render(Scene& scene, Camera& camera, ImageBuffer& buffer) = 0;
};

class WhittedRayTracer : Integrator
{
public:
    explicit WhittedRayTracer(int recursionDepth) : m_recursionDepth(recursionDepth) { }

    void Render(Scene& scene, Camera& camera, ImageBuffer& buffer) override;

private:

    Color3f TraceRay(const Rayf& ray, Scene& scene, int depth);

    int m_recursionDepth;
};

class StochasticRayTracer : public Integrator
{
public:
    explicit StochasticRayTracer(int depth = 5) : m_depth(depth), m_gen(std::random_device()()), m_dist(0, 1) { }
    ~StochasticRayTracer() override = default;

    void Render(Scene& scene, Camera& camera, ImageBuffer& buffer) override;
    Color3f TraceRay(const Rayf& ray, Scene& scene, int depth);

    // to generate random numbers [0,1]
    //std::random_device m_seed;
    std::mt19937 m_gen;
    std::uniform_real_distribution<> m_dist;

    int m_depth;
};


#endif //RAYTRACER_INTEGRATOR_H
