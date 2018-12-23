#pragma once
#include <vector>
#include <memory>
#include <optional>
#include "../acceleration/bvh.hpp"
#include "../imaging/light.hpp"
#include "../core/sampler.hpp"

class BSDF;

class Scene
{
public:

    Scene(std::vector<std::unique_ptr<Mesh>> meshes, std::vector<std::unique_ptr<Light>> lights);

    // Does a ray intersect any shapes in the scene?
    auto Intersects(const Rayf& ray, Intersection& isect) const -> bool;
    auto Intersects(const Rayf& ray)->std::optional<Intersection>;

    // Is there a clear line of sight between two points
    bool LineOfSightBetween(const Point3f& p1, const Point3f& p2) const;


    auto UniformSampleAllLights() const -> Color3f;
    //auto SampleOneLight(Sampler& sampler) const -> Color3f;
    auto SampleOneLight(const Intersection& isect, Sampler& sampler) const->Color3f;

    auto EstimateDirectLight(const Intersection& isect, Sampler& sampler, const Light& light) const -> Color3f;
    //auto EstimateDirectLight(const Intersection& isect) const -> Color3f;
    auto EstimateIndirectLight(const Intersection& isect) const -> Color3f;

    Color3f SamplePointLights(const Intersection& isect, const Rayf& ray) const;
    //Color3f SampleAreaLights(const Intersection& isect, const Rayf& ray);
    Color3f SampleIndirectLighting(const Intersection& isect, const Rayf& ray);

    void SetBackgroundColor(const Color3f& color);
    Color3f BackgroundColor() const;


private:
    Color3f m_backgroundColor;

    BVH m_meshes;
    std::vector<Light> m_lights;


    bool IntersectsQuick(const Rayf& ray) const;


};

