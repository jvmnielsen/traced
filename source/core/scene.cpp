#include "scene.hpp"

Scene::Scene(
        std::vector<std::unique_ptr<Mesh>> meshes,
        std::vector<std::unique_ptr<Mesh>> lights)
        : m_meshes(std::move(meshes))
        , m_lights(std::move(lights)) {

}

auto
Scene::intersects(const Rayf& ray) const -> std::optional<Intersection> {
    auto isect = m_meshes.Intersects(ray);

    for (int i = 0; i < m_lights.size(); ++i) {
        // the last overridden isect will always be the closest (ray max_param shrinks every time)
        auto tmp = m_lights[i]->Intersects(ray);
		if (tmp.has_value()) {
			isect = tmp;
			isect->m_lightID = i;
		}
    }

    return isect;
}

bool Scene::intersects_quick(const Rayf& ray) const {
    if (m_meshes.IntersectsFast(ray))
        return true;
    for (const auto& light : m_lights) {
        if (light->IntersectsFast(ray))
            return true;
    }
    return false;
}


bool Scene::line_of_sight_between(const Point3f& p1, const Point3f& p2) const {
    const Vec3f dir = p2 - p1;
    const auto distance = dir.length();
    Rayf ray{p1, normalize(dir), distance};
    return !intersects_quick(ray);
}


auto
Scene::sample_one_light(const Intersection& isect, const Vec3f& wo, Sampler& sampler) const -> Color3f {
    const auto nLights = m_lights.size();

    if (nLights == 0) return Color3f::Black(); // there are no lights

    if (nLights == 1) return estimate_direct_light(isect, wo, sampler, *m_lights[0]);

    int num;
    if (isect.m_lightID.has_value()) { // TODO: make lightID do something
        do {
            num = sampler.GetRandomInDistribution(nLights);
        } while (num == isect.m_lightID.value());
    } else {
        num = sampler.GetRandomInDistribution(nLights);
    }

    const auto& light = m_lights[num];

    // multiplying by the number of lights is the same as dividing with the fractional pdf 1/nLights
    return estimate_direct_light(isect, wo, sampler, *light) * nLights;
}


auto
Scene::sample_light_source(const Intersection& isect, const Vec3f& wo, Sampler& sampler, const Mesh& light) const -> Color3f {

    const auto [atLight, wi, lightPdf, li] = light.sample_as_light(isect, sampler);
    
    if (lightPdf > 0.0f && !li.IsBlack()) {
        const auto eval = isect.evaluate_material(wo, wi);
        const auto dots = std::abs(dot(wi, isect.get_shading_normal()));
        const auto f = eval * dots;
        const auto scatteringPdf = isect.material_pdf(wi);

        if (!f.IsBlack() && line_of_sight_between(isect.PointOffset(), atLight.PointOffset())) {
            const auto weight = Math::PowerHeuristic(1, lightPdf, 1, scatteringPdf);
            auto a =  f * li * weight / lightPdf;
            return a;
        }
    }
    return Color3f::Black();
}

auto
Scene::sample_bsdf(const Intersection& isect, const Vec3f& wo, Sampler& sampler, const Mesh& light) const -> Color3f {

    if (!isect.IsSpecular()) {

        auto [wi, scatteringPdf, f] = isect.sample_material(wo, sampler);
        f *= std::abs(dot(wi, isect.get_shading_normal()));

        if (!f.IsBlack() && scatteringPdf > 0) {

            auto lightPdf = light.Pdf(isect, wi);

            if (lightPdf == 0.0f) {
                return Color3f::Black();
            }

            auto lightIsect = intersects(Rayf{isect.PointOffset(), wi});

            auto li = Color3f::Black();
            
            if (lightIsect.has_value()) 
                li = lightIsect->emitted(-wi);
          
            if (!li.IsBlack()) {
                const auto weight = Math::PowerHeuristic(1, scatteringPdf, 1, lightPdf);
                auto directLight = f * li * weight / scatteringPdf;
                return directLight;
            }
        }
    }

    return Color3f::Black();
}


auto
Scene::estimate_direct_light(
    const Intersection& isect,
    const Vec3f& wo,
    Sampler& sampler,
    const Mesh& light) const -> Color3f {

    Color3f directLight = Color3f::Black();
   
    directLight += sample_light_source(isect, wo, sampler, light);
 
    directLight += sample_bsdf(isect, wo, sampler, light);
    

    return directLight;
}

void Scene::set_background_color(const Color3f& color) {
    m_background_color = color;
}

Color3f Scene::background_color() const {
    return m_background_color;
}



