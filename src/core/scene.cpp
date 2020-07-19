#include "scene.hpp"
#include <future>

using namespace tr;
using namespace gm;

Scene::Scene(
        std::vector<std::unique_ptr<Mesh>> meshes,
        std::vector<std::unique_ptr<Mesh>> lights)
        : m_meshes(std::move(meshes))
        , m_lights(std::move(lights)) {
}

auto Scene::intersects(Rayf const& ray) const -> std::optional<Intersection> {
    auto isect = m_meshes.intersects(ray);
    for (int i = 0; i < m_lights.size(); ++i) {
        // the last overridden isect will always be the closest (ray max_param shrinks every time)
        auto tmp = m_lights[i]->intersects(ray);
		if (tmp.has_value()) {
			isect = tmp;
			isect->m_lightID = i;
		}
    }
    return isect;
}

auto Scene::line_of_sight_between(Point3f const& p1, Point3f const& p2) const -> bool {
    const Vec3f dir = p2 - p1;
    const auto distance = dir.length();
    Rayf ray{p1, dir, distance};
    return !intersects(ray).has_value();
}

auto Scene::sample_one_light(Intersection const& isect, Vec3f const& wo, Sampler& sampler) const -> Color3f {

    auto const  n_lights = m_lights.size();
    if (n_lights == 0) return Color3f::black(); // there are no lights
    if (n_lights == 1) return estimate_direct_light(isect, wo, sampler, *m_lights[0]);

    int num;
    if (isect.m_lightID.has_value()) { // TODO: make lightID do something
        do {
            num = sampler.get_random_in_distribution(n_lights);
        } while (num == isect.m_lightID.value());
    } else {
        num = sampler.get_random_in_distribution(n_lights);
    }
    auto const& light = m_lights[num];
    // multiplying by the number of lights is the same as dividing with the fractional pdf 1/nLights
    return estimate_direct_light(isect, wo, sampler, *light) * n_lights;
}

auto Scene::sample_light_source(
        Intersection const& isect,
        Vec3f const& wo,
        Sampler& sampler,
        Mesh const& light
    ) const -> Color3f {

    auto const [at_light, wi, light_pdf, li] = light.sample_as_light(isect, sampler);
    if (light_pdf > 0.0f && !li.is_black()) {
        const auto eval = isect.evaluate_material(wo, wi);
        const auto dots = dot(isect.shading_normal(), wi) > 0 ? dot(isect.shading_normal(), wi) : 0;
        const auto f = eval * dots;
        const auto scattering_pdf = isect.material_pdf(wi);
        //if (scattering_pdf > 0) std::cout << scattering_pdf << '\n';

        if (!f.is_black() && line_of_sight_between(isect.offset_point(), at_light.offset_point())) {
            const auto weight = gm::power_heuristic(1, light_pdf, 1, scattering_pdf);
            auto a = f * li * weight / light_pdf;
            return a;
        }
    }
    return Color3f::black();
}

auto Scene::sample_bsdf(Intersection const& isect, Vec3f const& wo, Sampler& sampler, Mesh const& light) const -> Color3f {

    if (!isect.is_specular()) {
        auto [wi, scatteringPdf, f] = isect.sample_material(wo, sampler);
        f *= std::abs(dot(isect.shading_normal(), wi));

        if (!f.is_black() && scatteringPdf > 0) {
            auto lightPdf = light.pdf(isect, wi);
            if (lightPdf == 0.0f) {
                return Color3f::black();
            }
            auto lightIsect = intersects(Rayf{isect.offset_point(), wi});
            auto li = Color3f::black();
            if (lightIsect.has_value()) 
                li = lightIsect->emitted(-wi);
            if (!li.is_black()) {
                const auto weight = gm::power_heuristic(1, scatteringPdf, 1, lightPdf);
                auto directLight = f * li * weight / scatteringPdf;
                return directLight;
            }
        }
    }
    return Color3f::black();
}

auto Scene::estimate_direct_light(
        Intersection const& isect,
        Vec3f const& wo,
        Sampler& sampler,
        Mesh const& light
    ) const -> Color3f {
    return sample_light_source(isect, wo, sampler, light) + sample_bsdf(isect, wo, sampler, light);
}




