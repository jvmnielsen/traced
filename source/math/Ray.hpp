#pragma once
#include "point3.hpp"
#include "normal3.hpp"
#include <array>
#include "math_util.hpp"

enum class RayType { PrimaryRay, ShadowRay };

template<typename T>
class Ray
{
public:
    Ray() = default;

    Ray(const Point3<T>& origin,
        const Normal3<T>& direction,
        const T maxParam = Math::Constants::MaxFloat,
        const T minParam = 0.0001,
        const RayType& rayType = RayType::PrimaryRay)
        : m_origin(origin)
        , m_direction(direction)
        , m_max_param(maxParam)
        , m_min_param(minParam)
        , m_ray_type(rayType)
        , m_inverse_dir(1 / direction.x(), 1 / direction.y(), 1 / direction.z())
    {
        m_inverse_signs[0] = m_inverse_dir.x() < 0;
        m_inverse_signs[1] = m_inverse_dir.y() < 0;
        m_inverse_signs[2] = m_inverse_dir.z() < 0;
    }

    auto origin() const -> const Point3<T>& { return m_origin; }
    auto direction() const -> const Normal3<T>& { return m_direction; }
    auto point_at_parameter(const T t) const -> Point3<T> { return m_origin + m_direction * t; }

    auto update_max_parameter(const T maxParam) const -> void   { m_max_param = maxParam; }
    auto max_parameter()                        const -> T      { return m_max_param; }
    auto min_parameter()                        const -> T      { return m_minParam; }
    
    auto within_bounds(const T parameter)      const -> bool    { return parameter < m_max_param && parameter > m_min_param; }
    auto within_upper_bound(const T parameter) const -> bool    { return parameter < m_max_param; }

    auto inverse_signs()     const -> const std::array<int, 3>& { return m_inverse_signs; }
    auto inverse_direction() const -> const Vec3f&              { return m_inverse_dir; }

    //bool IsPrimaryRay() const { return m_ray_type == RayType::PrimaryRay; }

private:
    Point3<T> m_origin;
    Normal3<T> m_direction;
    RayType m_ray_type;

    mutable T m_max_param;
    T m_min_param;

    // used in AABB intersection test
    Vec3f m_inverse_dir;
    std::array<int, 3> m_inverse_signs; 
};

typedef Ray<FLOAT> Rayf;
typedef Ray<int> Rayi;
//typedef Ray<double> Rayd;