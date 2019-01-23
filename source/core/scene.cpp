#include "scene.hpp"

Scene::Scene(
        std::vector<std::unique_ptr<Mesh>> meshes,
        std::vector<Mesh> lights)
        : m_meshes(std::move(meshes))
        , m_lights(std::move(lights)) {

}

auto
Scene::Intersects(const Rayf& ray) const -> std::optional<Intersection> {
    auto isect = m_meshes.Intersects(ray);

    for (int i = 0; i < m_lights.size(); ++i) {
        // the last overridden isect will always be the closest (ray max_param shrinks every time)
        auto tmp = m_lights[i].Intersects(ray);
		if (tmp.has_value()) {
			isect = tmp;
			isect->m_lightID = i;
		}
    }

    return isect;
}

bool Scene::IntersectsQuick(const Rayf& ray) const {
    if (m_meshes.IntersectsFast(ray))
        return true;
    for (const auto& light : m_lights) {
        if (light.IntersectsFast(ray))
            return true;
    }
    return false;
}


bool Scene::LineOfSightBetween(const Point3f& p1, const Point3f& p2) const {
    const Vec3f dir = p2 - p1;
    const float distance = dir.Length();
    Rayf ray{p1, Normalize(dir), distance};
    return !IntersectsQuick(ray);
}


auto
Scene::SampleOneLight(const Intersection& isect, const Vec3f& wo, Sampler& sampler) const -> Color3f {
    const auto nLights = m_lights.size();

    if (nLights == 0) return Color3f{0.0f}; // there are no lights

    if (nLights == 1) return EstimateDirectLight(isect, wo, sampler, m_lights[0]);

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
    return EstimateDirectLight(isect, wo, sampler, light) * nLights;
}


auto
Scene::EstimateDirectLight(
        const Intersection& isect,
        const Vec3f& wo,
        Sampler& sampler,
        const Mesh& light) const -> Color3f {

    Color3f directLight = Color3f{0.0f};
    float lightPdf = 0, scatteringPdf = 0;

    // TODO: set max_param of ray in intersectfast

    //SamplingInfo info;
    Intersection atLight = light.SampleSurface(lightPdf, sampler);

    bool LoS = LineOfSightBetween(isect.OffsetGeometricPoint(), atLight.OffsetGeometricPoint());
    Vec3f wi = Normalize(atLight.GetPoint() - isect.GetPoint());

    float distanceSquared = (atLight.GetPoint() - isect.GetPoint()).LengthSquared();
    const auto radiance = light.GetMaterial().Emitted(atLight.GetShadingNormal(), -wi, distanceSquared);

    if (lightPdf > 0.0f && !radiance.IsBlack()) {
        Color3f f = isect.m_material->Evaluate(wo, wi) * std::abs(Dot(wi, isect.GetShadingNormal()));
        scatteringPdf = isect.m_material->Pdf(wi, isect.GetOrthonormalBasis());
        if (!f.IsBlack() && LoS) {
            float weight = Math::PowerHeuristic(1, lightPdf, 1, scatteringPdf);
            directLight += f * radiance * weight / lightPdf;
            if (directLight.r > 1.0f && directLight.g > 1.0f && directLight.b > 1.0f) {
                std::cout << "Greater than, direct1!\n";
            }
        }
    }

    if (!isect.IsSpecular()) {
        Color3f f = isect.m_material->Sample(wo, wi, scatteringPdf, isect, sampler); // overwrites wi
        f *= std::abs(Dot(wi, isect.GetShadingNormal()));

        if (!f.IsBlack() && scatteringPdf > 0) {

            auto lightIsect = Intersects(Rayf{isect.OffsetGeometricPoint(), wi});
            if (lightIsect.has_value()) {

                lightPdf = light.Pdf(isect.GetPoint(), wi);

                if (lightPdf == 0.0f) {
                    return directLight;
                }

                float distSqrd = (lightIsect->GetPoint() - isect.GetPoint()).LengthSquared();
                const auto li = light.GetMaterial().Emitted(lightIsect->GetShadingNormal(), wi, distSqrd); //check sign of wi
                float weight = Math::PowerHeuristic(1, scatteringPdf, 1, lightPdf);
                directLight += f * li * weight / scatteringPdf;
                if (directLight.r > 1.0f && directLight.g > 1.0f && directLight.b > 1.0f) {
                    std::cout << "Greater than 1, direct2!\n";
                }
            }
        }
    }
    return directLight;
}

void Scene::SetBackgroundColor(const Color3f& color) {
    m_backgroundColor = color;
}

Color3f Scene::BackgroundColor() const {
    return m_backgroundColor;
}



