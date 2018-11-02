#include "Scene.h"
#include <memory>

bool Scene::Intersects(const Rayf& ray, Intersection& isect) const
{
    for (const auto& volume : m_boundingVolumes)
        volume->Intersects(ray, isect);
           
    return isect.HasBeenHit();
}

bool Scene::IntersectsQuick(const Rayf& ray) const
{
    for (const auto& volume : m_boundingVolumes)
        if (volume->IntersectsQuick(ray))
            return true;

    return false;
}

void Scene::AddBoundingVolume(std::unique_ptr<BoundingVolume> boundingVolume)
{
    m_boundingVolumes.push_back(std::move(boundingVolume));
}

void Scene::AddShape(std::unique_ptr<Shape> shape)
{
    auto boundingBoxForShape = shape->GetBoundingVolume();
    boundingBoxForShape->SetShape(std::move(shape));
    m_boundingVolumes.emplace_back(std::move(boundingBoxForShape));
}

void AddPointLight(std::unique_ptr<Light> lightPtr);
void AddAreaLight(std::unique_ptr<BoundingVolume> lightPtr);


bool Scene::LineOfSightBetween(const Point3f& p1, const Point3f& p2) const
{
    const Vec3f offset = p2 - p1;
    const float distance = offset.Length();
    Rayf ray{ p1, offset, distance };
    return !IntersectsQuick(ray);
}


Color3f Scene::SamplePointLights(const Intersection& isect, const Rayf& ray) const
{
    Color3f color{0.0f};
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
    return color;
}

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
                        * isect.CalculateEmitted()
                        * std::max(0.0f, w_i.DotProduct(isect.GetShadingNormal()))
                        * std::max(0.0f, -w_i.DotProduct(lightIsect.GetGeometricNormal()) / distanceSquared);
        }
    }
    return color;
}

Color3f Scene::SampleIndirectLighting(const Intersection& isect, const Rayf& ray)
{
    return {0,0,0};
}

void Scene::SetBackgroundColor(const Color3f& color)
{
    m_backgroundColor = color;
}

Color3f Scene::BackgroundColor() const
{
    return m_backgroundColor;
}

auto Scene::GetPointLights() const -> const std::vector<std::unique_ptr<Light>>&
{
    return m_lights;
}

auto Scene::GetAreaLights() const -> const std::vector<std::unique_ptr<BoundingVolume>>&
{
    return m_areaLights;
}


