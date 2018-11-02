#include "Scene.h"
#include "Window.h"
#include "Sphere.h"
#include "Material.h"
#include "Triangle.h"
#include "Parser.h"
#include <memory>
#include "Utility.h"

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
    const auto boundingBoxForShape = shape->GetBoundingVolume();
    boundingBoxForShape->SetShape(shape);
    m_boundingVolumes.emplace_back(boundingBoxForShape);
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
        if (LineOfSightBetween(isect.PointOffsetAlongNormal(), light->GetPosition()))
        {
            LightingAtPoint info;
            light->IlluminatePoint(isect.GetPoint(), info);

            const auto w_i = info.directionToLight;

            color +=
        }
    }
}

Color3f Scene::SampleAreaLights(const Intersection& isect, const Rayf& ray) const
{

}

Color3f Scene::SampleIndirectLighting(const Intersection& isect, const Rayf& ray) const
{

}


