#pragma once

#include "../geometry/mesh.hpp"

#include <graphics-math.hpp>

#include <vector>
#include <memory>
#include <optional>


namespace tr {

class Scene
{
public:

    Scene(std::vector<std::unique_ptr<Mesh>> meshes, std::vector<std::unique_ptr<Mesh>> lights);

    auto intersects(const Rayf& ray) const -> std::optional<Intersection>;

    bool line_of_sight_between(const gm::Point3f& p1, const gm::Point3f& p2) const;

    auto sample_one_light(const Intersection& isect, const gm::Vec3f& wo, Sampler& sampler) const -> gm::Color3f;
    auto estimate_direct_light(const Intersection& isect, const gm::Vec3f& wo, Sampler& sampler, const Mesh& light) const -> gm::Color3f;

private:

    BVH m_meshes;
    std::vector<std::unique_ptr<Mesh>> m_lights;

    auto sample_light_source(const Intersection& isect, const gm::Vec3f& wo, Sampler& sampler, const Mesh& light) const -> gm::Color3f;
    auto sample_bsdf(const Intersection& isect, const gm::Vec3f& wo, Sampler& sampler, const Mesh& light) const -> gm::Color3f;
};

}