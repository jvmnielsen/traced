#pragma once
#include "point3.hpp"
#include "vec3.hpp"
#include <array>
#include "math_util.hpp"

enum class RayType { PrimaryRay, ShadowRay };

template< typename T >
class Ray
{
public:
    constexpr Ray() = default;

    Ray(const Point3<T>& origin,
        const Vec3<T>& direction,
        const T maxParam = Math::Infinity,
        const T minParam = 0.0001,
        const RayType& rayType = RayType::PrimaryRay)
        : m_origin(origin)
        , m_direction(direction)
        , m_maxParam(maxParam)
        , m_minParam(minParam)
        , m_rayType(rayType)
        , m_reciprocDir(1 / direction.x, 1 / direction.y, 1 / direction.z)
    {
        m_sign[0] = m_reciprocDir.x < 0;
        m_sign[1] = m_reciprocDir.y < 0;
        m_sign[2] = m_reciprocDir.z < 0;
    }

    const Point3<T>& GetOrigin() const { return m_origin; }
    const Vec3<T>& GetDirection() const { return m_direction; }
    Point3<T> PointAtParameter(const float t) const { return m_origin + m_direction * t; }
    const RayType& GetRayType() const { return m_rayType; }

    void NewMaxParameter(const T maxParam) const { m_maxParam = maxParam; }
    T GetMaxParameter() const { return m_maxParam; }
    T GetMinParameter() const { return m_minParam; }
    bool ParameterWithinBounds(const T parameter) const { return parameter < m_maxParam && parameter > m_minParam; }
    bool ParameterWithinUpperBound(const T parameter) const { return parameter < m_maxParam; }


    const std::array<int, 3>& GetReciprocSigns() const { return m_sign; }
    const Vec3f& GetReciprocDirection() const { return m_reciprocDir; }

    bool IsPrimaryRay() const { return m_rayType == RayType::PrimaryRay; }

private:
    Point3<T> m_origin;
    Vec3<T> m_direction;
    RayType m_rayType;

    mutable T m_maxParam;
    T m_minParam;

    Vec3f m_reciprocDir;
    std::array<int, 3> m_sign; // used in AABB intersection test
};

typedef Ray<float> Rayf;
typedef Ray<int> Rayi;
typedef Ray<double> Rayd;