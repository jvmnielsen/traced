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

    if (nLights == 0) return Color3f::Black(); // there are no lights

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
Scene::SampleLightSource(
        const Intersection& isect,
        const Vec3f& wo,
        Sampler& sampler,
        const Mesh& light) const -> Color3f {
    

    Color3f directLight = Color3f::Black();

    // TODO: set max_param of ray in intersectfast

   
    auto [atLight, wi, lightPdf, li] = light.SampleAsLight(isect, sampler);

    
    
    if (lightPdf > 0.0f && !li.IsBlack()) {
        const auto f = isect.m_material->Evaluate(wo, wi) * std::abs(Dot(wi, isect.GetShadingNormal()));
        const auto scatteringPdf = isect.m_material->Pdf(wi, isect.GetOrthonormalBasis());

        const auto LoS = LineOfSightBetween(isect.OffsetGeometricPoint(), atLight.OffsetGeometricPoint());

        if (!f.IsBlack() && LoS) {
            float weight = Math::PowerHeuristic(1, lightPdf, 1, scatteringPdf);
            directLight += f * li * weight / lightPdf;
            //if (directLight.r < 0.3 || directLight.b < 0.3 || directLight.g < 0.3)
                //std::cout << "light_source\n";
        }
    }

    //ClampColor(directLight);

    return directLight;
}

auto
Scene::SampleBSDF(const Intersection& isect,
        const Vec3f& wo,
        Sampler& sampler,
        const Mesh& light) const -> Color3f 
{

    if (!isect.IsSpecular()) {
        auto [wi, scatteringPdf, f] = isect.m_material->Sample(wo, isect, sampler); // overwrites wi
        f *= std::abs(Dot(wi, isect.GetShadingNormal()));

        if (!f.IsBlack() && scatteringPdf > 0) {

            auto lightPdf = light.Pdf(isect.GetPoint(), wi);

            if (lightPdf == 0.0f) {
                return Color3f::Black();
            }

            auto lightIsect = Intersects(Rayf{isect.OffsetGeometricPoint(), wi});

            auto li = Color3f::Black();
            
            if (lightIsect.has_value()) 
                li = lightIsect->m_material->Emitted(*lightIsect, -wi);
          
            if (!li.IsBlack()) {
                float weight = Math::PowerHeuristic(1, scatteringPdf, 1, lightPdf);
                auto directLight = f * li * weight / scatteringPdf;
                ClampColor(directLight);
                return directLight;
            }
      
            //const auto li = light.GetMaterial().Emitted(*lightIsect, wi); //check sign of wi
        }
    }

    return Color3f::Black();
}


auto
Scene::EstimateDirectLight(
        const Intersection& isect,
        const Vec3f& wo,
        Sampler& sampler,
        const Mesh& light) const -> Color3f {

    Color3f directLight = Color3f::Black();
   
    directLight += SampleLightSource(isect, wo, sampler, light);
    //if (directLight.r > 0.0 || directLight.b > 0.0 || directLight.g > 0.0)
        //std::cout << "light_source\n";
    directLight += SampleBSDF(isect, wo, sampler, light);
    //if (directLight.r > 0.0 || directLight.b > 0.0 || directLight.g > 0.0)
        //std::cout << "bsdf\n";

    return directLight;
}

void Scene::SetBackgroundColor(const Color3f& color) {
    m_backgroundColor = color;
}

Color3f Scene::BackgroundColor() const {
    return m_backgroundColor;
}



