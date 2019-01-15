#include "scene.hpp"

Scene::Scene(
        std::vector<std::unique_ptr<Mesh>> meshes,
        std::vector<std::unique_ptr<Mesh>> lights)
        : m_meshes(std::move(meshes))
//, m_lights(std::move(lights))
{
    m_lights.reserve(lights.size());

    //for (auto& light : lights)
    //    m_lights.emplace_back(std::move(light));

}

auto
Scene::Intersects(const Rayf& ray) -> std::optional<Intersection> {
    return m_meshes.Intersects(ray);
}

bool Scene::IntersectsQuick(const Rayf& ray) const {
    return m_meshes.IntersectsFast(ray);
}


bool Scene::LineOfSightBetween(const Point3f& p1, const Point3f& p2) const {
    const Vec3f offset = p2 - p1;
    const float distance = offset.Length();
    Rayf ray{p1, offset, distance};
    return !IntersectsQuick(ray);
}


auto
Scene::UniformSampleAllLights() const -> Color3f {
    for (const auto& light : m_lights) {

    }
    return Color3f{0.0f};
}


auto
Scene::SampleOneLight(const Intersection& isect, Sampler& sampler) const -> Color3f {
    const auto nLights = m_lights.size();

    if (nLights == 0) return Color3f{0.0f}; // there are no lights

    auto dist = std::uniform_int_distribution<>(0, static_cast<int>(nLights));

    const auto& light = m_lights[sampler.GetRandomInDistribution(dist)];

    //while (light == hitLight) {
    // implement check to ensure we don't double dip
    //}

    // multiplying by the number of lights is the same as dividing with the fractional pdf 1/nLights
    return EstimateDirectLight(isect, sampler, light) * nLights;
}


auto
Scene::EstimateDirectLight(
        const Intersection& isect,
        Sampler& sampler,
        const Mesh& light) const -> Color3f {

    Color3f directLight = Color3f{0.0f};

    if (isect.IsSpecular()) {
        return Color3f{0.0f};
    }

    SamplingInfo info;
    Intersection atLight = light.SampleSurface(info, sampler);

    if (!LineOfSightBetween(isect.GetPoint(), atLight.GetPoint())) {
        return Color3f{0.0f};
    }

    info.toLight = Normalize(atLight.GetPoint() - isect.GetPoint());

    const auto radiance = light.GetMaterial().Emitted(atLight, info);

    if (info.pdf != 0.0f && !radiance.IsBlack()) {
        directLight += isect.m_material->Evaluate(info) * radiance / info.pdf;
    }

    return directLight;


    /*
    Color3f directLight = Color3f{0.0f};
    Color3f f;
    float lightPdf, scatterPdf;
    Normal3f wi;
    Intersection atLight;

    if (isect.m_material->m_bsdf.GetType() != Specular) {
        Color3f li = light.Sample(isect, wi, lightPdf, atLight);

        if (lightPdf != 0.0f && !li.IsBlack()) {
            f = bsdf.Evaluate(isect.m_wo, wi);
            scatterPdf = bsdf.Pdf(isect.m_wo, wi);

            if (scatterPdf != 0.0f && !f.IsBlack()) {
                const auto weight = Math::PowerHeuristic(1, lightPdf, 1, scatterPdf);
                directLight += f * li * weight / lightPdf;
            }
        }
    }

    // update isect.inputDir
    f = bsdf.Sample(isect.m_wo, wi, scatterPdf, sampler);
    //f = bsdf.Evaluate(isect);
    //scatterPdf = bsdf.Pdf(isect);
    if (scatterPdf != 0.0f && !f.IsBlack()) {
        lightPdf = light.PdfLi(isect, wi);
        if (lightPdf == 0.0f) {
            return directLight;
        }

        const auto weight = Math::PowerHeuristic(1, scatterPdf, 1, lightPdf);
        Color3f li = light.m_radiance;
        directLight += f * li * weight / scatterPdf;
    }

    return directLight; */
}

auto
Scene::EstimateIndirectLight(const Intersection& isect) const -> Color3f {

    return Color3f{0.3f};
}


Color3f Scene::SamplePointLights(const Intersection& isect, const Rayf& ray) const {
    Color3f color{0.0f};
    /*
    for (auto& light : m_lights)
    {
        if (LineOfSightBetween(isect.OffsetShadingPoint(), light->GetPosition()))
        {
            LightingAtPoint info;
            light->IlluminatePoint(isect.GetPoint(), info);

            const auto w_i = info.directionToLight;
            const auto w_o = -ray.GetDirection();

            color += isect.GetMaterial()->EvaluateBSDF(w_o, w_i) * info.intensityAtPoint * std::max(0.0f, w_i.DotProduct(isect.GetShadingNormal()));
        }
    }
     */
    return color;
}

/*
Color3f Scene::SampleAreaLights(const Intersection& isect, const Rayf& ray)
{
    Color3f color{0.0f};
    for (auto& light : m_areaLights)
    {
        auto lightIsect = light->GetShape().GetRandomSurfaceIntersection();
        if (LineOfSightBetween(isect.OffsetGeometricPoint(), lightIsect.OffsetGeometricPoint()))
        {
            //LightingAtPoint info;
            //light->IlluminatePoint(isect.GetPoint(), info);

            auto w_i = lightIsect.GetPoint() - isect.GetPoint();
            const float distanceSquared = w_i.LengthSquared();
            w_i /= sqrt(distanceSquared);

            color += isect.GetMaterial()->EvaluateBSDF(w_i, -ray.GetDirection())
                        * light->GetShape().GetMaterial().Emitted(Point2f{0}, Point3f{0})
                        * std::max(0.0f, w_i.DotProduct(isect.GetShadingNormal()))
                        * std::max(0.0f, -w_i.DotProduct(lightIsect.GetGeometricNormal()) / distanceSquared);
        }
    }
    return color;
} */

Color3f Scene::SampleIndirectLighting(const Intersection& isect, const Rayf& ray) {
    return {0, 0, 0};
}

void Scene::SetBackgroundColor(const Color3f& color) {
    m_backgroundColor = color;
}

Color3f Scene::BackgroundColor() const {
    return m_backgroundColor;
}



