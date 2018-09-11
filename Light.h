#pragma once
#include "Vec3.h"

class Light
{
public:
    explicit Light(const Vec3f& color, const float intensity = 1);

    virtual ~Light() = default;
    virtual void illuminate() const = 0;

    Vec3f m_color;
    float m_intensity;

};

class DistantLight : public Light
{
    
};

class PointLight : public Light
{
    
};