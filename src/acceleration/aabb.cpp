#include "aabb.hpp"
#include <functional>
#include "../geometry/mesh.hpp"

using namespace tr; 
using namespace gm; 

AABB::AABB(std::unique_ptr<Mesh> mesh)
    : m_bounds(mesh->calculate_bounds()),
      m_center(m_bounds.center()),
      m_mesh(std::move(mesh)) {
}

AABB::AABB(Bounds bounds)
    : m_bounds(std::move(bounds)),
      m_center(bounds.center()) {
}

AABB::AABB(std::unique_ptr<Mesh> mesh, Bounds bounds)
    : m_bounds(std::move(bounds)),
      m_center(bounds.center()),
      m_mesh(std::move(mesh)) {
}

AABB::AABB(const AABB& other)
    : m_bounds(other.m_bounds),
      m_mesh(std::make_unique<Mesh>(*other.m_mesh)),
      m_center(other.m_center) {
}

auto AABB::operator=(const AABB& other) -> AABB& {
    if (&other == this)
        return *this;
    
    m_bounds = other.m_bounds;

    if (other.m_mesh)
        m_mesh = std::make_unique<Mesh>(*other.m_mesh);

    m_center = other.m_center;

    return *this;
}

auto AABB::center() const -> Point3f const& {
    return m_center;
}

auto AABB::intersects_bounds(const Rayf& ray) const -> bool {
    float tmin, tmax, tymin, tymax, tzmin, tzmax;

    tmin = (m_bounds[ray.inverse_signs().at(0)].x - ray.origin().x) * ray.inverse_direction().x;
    tmax = (m_bounds[1 - ray.inverse_signs().at(0)].x - ray.origin().x) * ray.inverse_direction().x;
    tymin = (m_bounds[ray.inverse_signs().at(1)].y - ray.origin().y) * ray.inverse_direction().y;
    tymax = (m_bounds[1 - ray.inverse_signs().at(1)].y - ray.origin().y) * ray.inverse_direction().y;

    if ((tmin > tymax) || (tymin > tmax))
        return false;

    if (tymin > tmin)
        tmin = tymin;
    if (tymax < tmax)
        tmax = tymax;

    tzmin = (m_bounds[ray.inverse_signs().at(2)].z - ray.origin().z) * ray.inverse_direction().z;
    tzmax = (m_bounds[1 - ray.inverse_signs().at(2)].z - ray.origin().z) * ray.inverse_direction().z;

    if ((tmin > tzmax) || (tzmin > tmax))
        return false;
    if (tzmin > tmin)
        tmin = tzmin;
    if (tzmax < tmax)
        tmax = tzmax;

    auto parameter = tmin;

    if (parameter < 0) {
        parameter = tmax;
        if (parameter < 0)
            return false;
    }
    return true;
}

auto AABB::intersects(const Rayf& ray) const -> std::optional<Intersection> {
    if (intersects_bounds(ray)) {
        return m_mesh->intersects(ray);
    }
    return std::nullopt;
}

auto AABB::intersects_mesh(const Rayf& ray) const->std::optional<Intersection> {
    return m_mesh->intersects(ray);
}

auto AABB::bounds() const -> const Bounds& {
    return m_bounds;
}
