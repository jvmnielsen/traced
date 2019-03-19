#pragma once
#include <vector>
#include <memory>
#include <optional>
#include "../geometry/mesh.hpp"

class Scene
{
public:

    Scene(std::vector<std::unique_ptr<Mesh>> meshes, std::vector<std::unique_ptr<Mesh>> lights);

    auto Intersects(const Rayf& ray) const -> std::optional<Intersection>;

    bool LineOfSightBetween(const Point3f& p1, const Point3f& p2) const;

    auto SampleOneLight(const Intersection& isect, const Normal3f& wo, Sampler& sampler) const -> Color3f;
    auto EstimateDirectLight(const Intersection& isect, const Normal3f& wo, Sampler& sampler, const Mesh& light) const -> Color3f;

    void SetBackgroundColor(const Color3f& color);
    Color3f BackgroundColor() const;


private:
    Color3f m_backgroundColor;
    BVH m_meshes;
    std::vector<std::unique_ptr<Mesh>> m_lights;
    bool IntersectsQuick(const Rayf& ray) const;

    auto SampleLightSource(const Intersection& isect, const Normal3f& wo, Sampler& sampler, const Mesh& light) const -> Color3f;
    auto SampleBSDF(const Intersection& isect, const Normal3f& wo, Sampler& sampler, const Mesh& light) const->Color3f;

};

