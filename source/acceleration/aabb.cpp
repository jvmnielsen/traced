//
// Created by Jacob Vesti Moeslund Nielsen on 18/11/2018.
//
#include "aabb.hpp"
#include <functional>
#include "../geometry/mesh.hpp"
#include "../geometry/bounds.hpp"

AABB::AABB(std::unique_ptr<Mesh> mesh)
    : m_bounds(mesh->calculate_bounds()),
      m_center(m_bounds.center()),
      m_mesh(std::move(mesh))
{
}

AABB::AABB(Bounds bounds)
    : m_bounds(std::move(bounds)),
      m_center(bounds.center())
{
}

AABB::AABB(std::unique_ptr<Mesh> mesh, Bounds bounds)
    : m_bounds(std::move(bounds)),
      m_center(bounds.center()),
      m_mesh(std::move(mesh))
{ }


AABB::AABB(const AABB& other)
    : m_bounds(other.m_bounds),
      m_mesh(std::make_unique<Mesh>(*other.m_mesh)),
      m_center(other.m_center)
{
}

auto AABB::operator=(const AABB& other) -> AABB&
{
    if (&other == this)
        return *this;
    
    m_bounds = other.m_bounds;

    if (other.m_mesh)
        m_mesh = std::make_unique<Mesh>(*other.m_mesh);

    m_center = other.m_center;

    return *this;
}


auto
AABB::center() const -> const Point3f&
{
    return m_center;
}

bool AABB::IntersectsFast(const Rayf& ray) const {
    return m_mesh->IntersectsFast(ray);
}

auto AABB::intersects_bounds(const Rayf& ray) const -> bool {

    auto min = 0.0;
    auto max = Math::Constants::MaxFloat;

    for (int axis = 0; axis < 3; ++axis) {
        // use the precomputed sign of the ray to avoid having to swap t_near and t_far
        const auto t_near = (m_bounds[ray.inverse_signs()[axis]][axis] - ray.origin()[axis]) * ray.inverse_direction()[axis];
        const auto t_far  = (m_bounds[1 - ray.inverse_signs()[axis]][axis] - ray.origin()[axis]) * ray.inverse_direction()[axis];

        // we want the smallest max and largest min
        min = t_near > min ? t_near : min;
        max = t_far < max ? t_far : max;

    }

    // is the box behind us or are we inside it == false
    return min >= 0;
}


auto AABB::Intersects(const Rayf& ray) const -> std::optional<Intersection> {

    if (intersects_bounds(ray))
        return m_mesh->Intersects(ray);
    
    return std::nullopt;
}


auto 
AABB::IntersectsMesh(const Rayf& ray) const->std::optional<Intersection> {
    return m_mesh->Intersects(ray);
}

auto
AABB::IntersectsMeshFast(const Rayf& ray) const -> bool
{
    return m_mesh->IntersectsFast(ray);
}

auto AABB::bounds() const -> const Bounds&
{
    return m_bounds;
}
