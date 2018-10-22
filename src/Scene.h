#pragma once
#include <vector>
#include "MathUtil.h"
#include "BoundingVolume.h"
#include "Imaging.h"

class Scene
{
public:
    Scene() = default;
    ~Scene() = default;

    bool Intersects(const Rayf& ray, Intersection& isect) const;
    bool IntersectsQuick(const Rayf& ray) const;

	void AddBoundingVolume(std::shared_ptr<BoundingVolume> boundingVolume);

    void AddShape(std::shared_ptr<Shape> shape);

    void AddLight(std::unique_ptr<Light> lightPtr);

    //void SetCamera(std::unique_ptr<Camera> camera);

    void SetBackgroundColor(const Color3f& color) { m_backgroundColor = color; }
    Color3f BackgroundColor() const { return m_backgroundColor; }

    std::vector<std::shared_ptr<Light>> m_lights;

    std::vector<std::shared_ptr<BoundingVolume>> m_boundingVolumes;

private:
    Color3f m_backgroundColor;




    //std::vector<std::unique_ptr<Mesh>> m_scene_meshes;
    //const float m_infinity = std::numeric_limits<float>::max();

    //Intersection Trace(const Rayf &ray);

    //void TraceSimpleObjects(const Rayf &ray, Intersection &hit_data);
    //void TraceMeshes(const Rayf &ray, Intersection &hit_data);
	
    //Color3f CastRay(const Rayf& ray, uint32_t depth);

    //Vec3f random_in_unit_sphere();

    //std::unique_ptr<Camera> m_camera;

	float m_shadow_bias = 1e-4f;
};

