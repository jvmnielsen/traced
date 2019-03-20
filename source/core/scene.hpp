#pragma once
#include <vector>
#include <memory>
#include <optional>
#include "../geometry/mesh.hpp"

class Scene
{
public:

    Scene(std::vector<std::unique_ptr<Mesh>> meshes, std::vector<std::unique_ptr<Mesh>> lights);

    auto intersects(const Rayf& ray) const -> std::optional<Intersection>;

    bool line_of_sight_between(const Point3f& p1, const Point3f& p2) const;

    auto sample_one_light(const Intersection& isect, const Vec3f& wo, Sampler& sampler) const -> Color3f;
    auto estimate_direct_light(const Intersection& isect, const Vec3f& wo, Sampler& sampler,
                               const Mesh& light) const -> Color3f;

    void set_background_color(const Color3f& color);
    Color3f background_color() const;


private:
    Color3f m_background_color;
    BVH m_meshes;
    std::vector<std::unique_ptr<Mesh>> m_lights;
    
    bool intersects_quick(const Rayf& ray) const;

    auto sample_light_source(const Intersection& isect, const Vec3f& wo, Sampler& sampler,
                             const Mesh& light) const -> Color3f;
    auto sample_bsdf(const Intersection& isect, const Vec3f& wo, Sampler& sampler, const Mesh& light) const -> Color3f;
};
