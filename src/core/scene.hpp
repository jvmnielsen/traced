#pragma once

#include "../geometry/mesh.hpp"

#include <graphics-math.hpp>

#include <vector>
#include <memory>
#include <optional>

namespace tr {

    class Scene {
    public:
        Scene(std::vector<std::unique_ptr<Mesh>> meshes, std::vector<std::unique_ptr<Mesh>> lights);
        [[nodiscard]] auto intersects(Rayf const& ray) const -> std::optional<Intersection>;
        [[nodiscard]] auto line_of_sight_between(gm::Point3f const& p1, gm::Point3f const& p2) const -> bool;
        auto sample_one_light(Intersection const& isect, gm::Vec3f const& wo, Sampler& sampler) const -> gm::Color3f;
        auto estimate_direct_light(Intersection const& isect, gm::Vec3f const& wo, Sampler& sampler, Mesh const& light) const -> gm::Color3f;
    private:
        BVH m_meshes;
        std::vector<std::unique_ptr<Mesh>> m_lights;

        auto sample_light_source(Intersection const& isect, gm::Vec3f const& wo, Sampler& sampler, Mesh const& light) const -> gm::Color3f;
        auto sample_bsdf(Intersection const& isect, gm::Vec3f const& wo, Sampler& sampler, Mesh const& light) const -> gm::Color3f;
    };
}