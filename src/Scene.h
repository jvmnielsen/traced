#pragma once
#include <vector>
//#include "BoundingVolume.h"
#include <memory>
//#include "Shape.h"
#include "BoundingVolume.h"
//#include "../BVH.h"

//class Intersection;

class Scene
{
public:
    //Scene() = default;
    //~Scene() = default;

    // Does a ray intersect any shapes in the scene?
    bool Intersects(const Rayf& ray, Intersection& isect) const;
    
    // Adding shapes and lights to the scene
    void AddShape(std::unique_ptr<Mesh> shape);
    void AddPointLight(std::unique_ptr<Light> lightPtr);
    void AddAreaLight(std::unique_ptr<Mesh> lightPtr);

    // Is there a clear line of sight between two points
    bool LineOfSightBetween(const Point3f& p1, const Point3f& p2) const;

    //auto EstimateDirectLight(const Intersection& isect) -> Color3f;


    Color3f SamplePointLights(const Intersection& isect, const Rayf& ray) const;
    //Color3f SampleAreaLights(const Intersection& isect, const Rayf& ray);
    Color3f SampleIndirectLighting(const Intersection& isect, const Rayf& ray);

    void SetBackgroundColor(const Color3f& color);
    Color3f BackgroundColor() const;

    const std::vector<std::unique_ptr<Light>>& GetPointLights() const;
    const std::vector<std::unique_ptr<BoundingVolume>>& GetAreaLights() const;

    std::vector<Mesh> m_meshes;
   
private:
    Color3f m_backgroundColor;

    std::vector<std::unique_ptr<Light>> m_lights;
    std::vector<std::unique_ptr<BoundingVolume>> m_areaLights;
    std::vector<std::unique_ptr<BoundingVolume>> m_boundingVolumes;

    //std::unique_ptr<BVH> m_bvh;

    void AddBoundingVolume(std::unique_ptr<BoundingVolume> boundingVolume);

    bool IntersectsQuick(const Rayf& ray) const;

    float m_shadow_bias = 1e-4f;
};

