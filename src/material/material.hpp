#pragma once

#include "../core/sampler.hpp"

#include <graphics-math.hpp>

namespace tr {

    class Intersection;

    class Material {
    public:
        Material() = default;
        virtual auto sample(
            gm::Vec3f const& wo,
            Intersection const& isect,
            Sampler &sampler
        ) const -> std::tuple<gm::Vec3f, FLOAT, gm::Color3f>;
        virtual auto evaluate(gm::Vec3f const& wo, gm::Vec3f const& wi, Intersection const& isect) const -> gm::Color3f = 0;
        virtual auto pdf(gm::Vec3f const& dir, Intersection const& isect) const -> FLOAT;
        virtual auto emitted(Intersection const& isect, gm::Vec3f const& dir) const -> gm::Color3f;
    };

    class Matte : public Material {
    public:
        explicit Matte(gm::Color3f attenuation = gm::Color3f(0.18)) : m_attenuation(attenuation) {}
        auto evaluate(gm::Vec3f const& wo, gm::Vec3f const& wi, const Intersection &isect) const -> gm::Color3f override;
    private:
        gm::Color3f m_attenuation;
    };

    class Emissive : public Matte {
    public:
        auto emitted(Intersection const& isect, gm::Vec3f const& dir) const -> gm::Color3f override;
        gm::Color3f m_radiance = gm::Color3f(1.0);
    };


    class Glossy : public Material {
    public:
        auto sample(
            gm::Vec3f const& wo,
            Intersection const& isect,
            Sampler &sampler
        ) const -> std::tuple<gm::Vec3f, FLOAT, gm::Color3f> override;

        auto evaluate(
            gm::Vec3f const& wo,
            gm::Vec3f const&wi,
            Intersection const&isect
        ) const -> gm::Color3f override;

        auto pdf(gm::Vec3f const& dir, Intersection const& isect) const -> FLOAT override;

    private:
        const gm::Color3f m_attenuation = gm::Color3f{0.1f, 0.2, 0.6f};
        FLOAT m_ks = 0.7;
        FLOAT m_exp = 100.0f;
    };
}