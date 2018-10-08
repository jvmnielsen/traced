#pragma once
#include <vector>
#include "Renderable.h"
#include <random>
#include "Imaging.h"
#include <memory>
#include <limits>
#include "Imaging.h"
#include "MathUtil.h"
#include "Mesh.h"

class Scene
{
public:
    Scene()
        : m_gen(std::random_device()())
        , m_dist(0, 1)
        , m_max_depth(5)
    {
	}

	//Scene(const Scene& other) = delete; // no copy-constructor for now

    void Render(ImageBuffer &buffer);

	void AddRenderable(std::unique_ptr<Renderable> renderPtr);
    void AddMesh(std::unique_ptr<Mesh> meshPtr);
    void AddLight(std::unique_ptr<Light> lightPtr);

    void SetCamera(std::unique_ptr<Camera> camera);

    void SetBackgroundColor(const Vecf& color) { m_backgroundColor = color; }
    const Vecf& BackgroundColor() { return m_backgroundColor; }

private:
    Vecf m_backgroundColor;
    uint32_t m_max_depth;

	std::vector<std::shared_ptr<Light>> m_scene_lights;
    
    // to generate random numbers [0,1]
    //std::random_device m_seed;
    std::mt19937 m_gen;
    std::uniform_real_distribution<> m_dist;

	std::vector<std::unique_ptr<Renderable>> m_simple_scene_objects;
    std::vector<std::unique_ptr<Mesh>> m_scene_meshes;
    const float m_infinity = std::numeric_limits<float>::max();

    Intersection Trace(const Rayf &ray);

    void TraceSimpleObjects(const Rayf &ray, Intersection &hit_data);
    void TraceMeshes(const Rayf &ray, Intersection &hit_data);
	
    Vecf CastRay(const Rayf& ray, uint32_t depth);

    Vecf random_in_unit_sphere();

    std::unique_ptr<Camera> m_camera;

	float m_shadow_bias = 1e-4f;
};

