#pragma once

#include "../material/material.hpp"
#include "../geometry/ray.hpp"

#include <graphics-math.hpp>

#include <optional>

namespace tr {
    class Mesh;

    class Intersection {
    public:

        Intersection(gm::Point3f point, gm::Point2f uv_coord, gm::Vec3f geometric_normal, gm::Vec3f shading_normal);


        [[nodiscard]] auto offset_point() const -> gm::Point3f;


        [[nodiscard]] auto point() const -> gm::Point3f const &;

        [[nodiscard]] auto geometric_normal() const -> gm::Vec3f const &;

        [[nodiscard]] auto shading_normal() const -> gm::Vec3f const &;

        [[nodiscard]] auto is_specular() const -> bool;

        [[nodiscard]] auto get_shading_basis() const -> gm::ONB const & { return m_shading_basis; }
        //auto GetTransformedSampledVec(Sampler& sampler) const -> Normal3f;



        auto sample_material(gm::Vec3f const &wo, Sampler &sampler) const -> std::tuple<gm::Vec3f, FLOAT, gm::Color3f>;

        [[nodiscard]] auto emitted(gm::Vec3f const &dir) const -> gm::Color3f;

        [[nodiscard]] auto material_pdf(gm::Vec3f const &wi) const -> FLOAT;

        [[nodiscard]] auto evaluate_material(gm::Vec3f const &wo, gm::Vec3f const &wi) const -> gm::Color3f;

        [[nodiscard]] auto ray_towards(const gm::Vec3f &dir) const -> Rayf;


        auto SetMeshAndMaterial(const Mesh *mesh, const Material *material) -> void;

        std::optional<int> m_lightID;

    private:

        const Mesh *m_mesh;

        const Material *m_material;


        gm::ONB m_shading_basis;

        gm::Point3f m_point;
        gm::Point2f m_uv;
        gm::Vec3f m_geometric_normal;
        //Normal3f        m_shadingNormal;
        //Normal3f        m_tangent;
    };

}