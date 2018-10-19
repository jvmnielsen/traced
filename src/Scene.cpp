#include "Scene.h"
#include "Window.h"
#include "Sphere.h"
#include "Material.h"
#include "Triangle.h"
#include "Parser.h"
#include <memory>
#include "Utility.h"

bool Scene::Intersects(const Rayf& ray, Intersection& isect)
{
    for (const auto& volume : m_boundingVolumes)
        volume->Intersects(ray, isect);

    return isect.m_hasBeenHit;
}

bool Scene::IntersectsQuick(const Rayf& ray, Intersection& isec)
{
    return true;
}

void Scene::AddBoundingVolume(std::shared_ptr<BoundingVolume> boundingVolume)
{
    m_boundingVolumes.push_back(boundingVolume);
}

void Scene::AddLight(std::unique_ptr<Light> lightPtr)
{
    m_lights.push_back(std::move(lightPtr));
}
