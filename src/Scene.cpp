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
           
    return isect.m_hasBeenHit;
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
    m_boundingVolumes.push_back(boundingVolume);
}

void Scene::AddShape(std::unique_ptr<Shape> shape)
{
    const auto boundingBoxForShape = shape->GetBoundingVolume();
    boundingBoxForShape->SetShape(shape);
    m_boundingVolumes.emplace_back(boundingBoxForShape);
}

void AddPointLight(std::unique_ptr<Light> lightPtr);
void AddAreaLight(std::unique_ptr<BoundingVolume> lightPtr);


bool Scene::HasLineOfSight(const Point3f& p1, const Point3f& p2)
{
    const Vec3f offset = p2 - p1;
    const float distance = offset.Length();
    Rayf ray{ p1, offset, distance };
    return !IntersectsQuick(ray);
}

