#pragma once
#include "../math/vec3.hpp"
#include "color3.hpp"
#include "../math/point3.hpp"
#include "../math/math_util.hpp"


struct LightingAtPoint
{
    Vec3f directionToLight;
    Color3f intensityAtPoint;
};

class Light
{
public:
    Light(const Color3f intensity, const Point3f& position)
        : m_intensity(intensity)
        , m_position(position)
    {}

    virtual ~Light() = default;
    virtual void IlluminatePoint(const Point3f& point, LightingAtPoint& info) const = 0;

    const Color3f& GetItensity() const { return m_intensity; }
    const Point3f& GetPosition() const { return m_position; }

protected:
    Color3f m_intensity;
    Point3f m_position;


    //Color3f m_color;
    //float m_intensity;
};

class PointLight : public Light
{
public:
    PointLight() = default;

    PointLight(const Color3f intensity, const Point3f& position)
        : Light(intensity, position)
    {}


    void IlluminatePoint(const Point3f& point, LightingAtPoint& info) const override
    {
        info.directionToLight = m_position - point;
        const auto lengthSquared = info.directionToLight.LengthSquared();
        info.directionToLight.Normalize();
        info.intensityAtPoint = m_intensity / (4 * Math::Pi * lengthSquared);
    }


};

