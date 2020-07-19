#pragma once

#include <graphics-math.hpp>

#include <array>

namespace tr {

    enum class RayType { PrimaryRay, ShadowRay };

    template<typename T>
    class Ray {
    public:
        Ray() = default;

        Ray(gm::Point3<T> origin,
            gm::Vec3<T> direction,
            const T max_param = gm::constants::max_float,
            const T min_param = 0.0001,
            RayType const& ray_type = RayType::PrimaryRay)
            : m_origin(std::move(origin))
            , m_direction(std::move(direction))
            , m_max_param(max_param)
            , m_min_param(min_param)
            , m_ray_type(ray_type)
            , m_inverse_dir(1 / direction.x, 1 / direction.y, 1 / direction.z) {

            m_inverse_signs[0] = m_inverse_dir.x < 0;
            m_inverse_signs[1] = m_inverse_dir.y < 0;
            m_inverse_signs[2] = m_inverse_dir.z < 0;
        }

        auto origin() const -> gm::Point3<T> const& { return m_origin; }
        auto direction() const -> gm::Vec3<T> const& { return m_direction; }
        
        auto point_at_parameter(const T t) const -> gm::Point3<T> { return m_origin + m_direction * t; }

        auto update_max_parameter(const T max_param) const -> void { m_max_param = max_param; }
        
        auto max_parameter() const -> T { return m_max_param; }
        auto min_parameter() const -> T { return m_min_param; }
        
        auto within_bounds(const T parameter) const -> bool { 
            return parameter < m_max_param && parameter > m_min_param; 
        }
        auto within_upper_bound(const T parameter) const -> bool { return parameter < m_max_param; }

        auto inverse_signs() const -> const std::array<int, 3>& { return m_inverse_signs; }
        auto inverse_direction() const -> gm::Vec3f const& { return m_inverse_dir; }

    private:
        gm::Point3<T> m_origin;
        gm::Vec3<T> m_direction;
        RayType m_ray_type;

        mutable T m_max_param;
        T m_min_param;

        // used in AABB intersection test
        gm::Vec3f m_inverse_dir;
        std::array<int, 3> m_inverse_signs; 
    };

    typedef Ray<FLOAT> Rayf;
    typedef Ray<int> Rayi;

}